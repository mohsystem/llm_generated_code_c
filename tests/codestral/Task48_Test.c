#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 256

int main() {
    int sockfd;
    struct sockaddr_in serv_addr;
    char send_msg[] = "Hello from client!";
    char recv_msg[BUFFER_SIZE];
    int n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        printf("Test client_connection_and_communication: FAIL\n");
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        printf("Test client_connection_and_communication: FAIL\n");
        close(sockfd);
        return 1;
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        printf("Test client_connection_and_communication: FAIL\n");
        close(sockfd);
        return 1;
    }

    n = write(sockfd, send_msg, strlen(send_msg));
    if (n < 0) {
        perror("Write failed");
        printf("Test client_connection_and_communication: FAIL\n");
        close(sockfd);
        return 1;
    }

    n = read(sockfd, recv_msg, BUFFER_SIZE - 1);
    if (n < 0) {
        perror("Read failed");
        printf("Test client_connection_and_communication: FAIL\n");
        close(sockfd);
        return 1;
    }

    recv_msg[n] = '\0';
    printf("Received from server: %s\n", recv_msg);

    // تحقق بسيط أن الرد هو الرسالة المتوقعة
    if (strcmp(recv_msg, "I got your message") == 0) {
        printf("Test client_connection_and_communication: PASS\n");
    } else {
        printf("Test client_connection_and_communication: FAIL\n");
    }

    close(sockfd);
    return 0;
}
