#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5555
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char send_msg[] = "Hello from the client!";
    char recv_msg[BUFFER_SIZE];
    int bytes_sent, bytes_recv;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        printf("Test client_connection_and_communication: FAIL\n");
        return 1;
    }
    printf("Passed");
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connect failed");
        printf("Test client_connection_and_communication: FAIL\n");
        close(sock);
        return 1;
    }
    printf("Passed");

    bytes_sent = send(sock, send_msg, strlen(send_msg), 0);
    if (bytes_sent < 0) {
        perror("Send failed");
        printf("Test client_connection_and_communication: FAIL\n");
        close(sock);
        return 1;
    }
    printf("Passed");

    bytes_recv = recv(sock, recv_msg, BUFFER_SIZE - 1, 0);
    if (bytes_recv < 0) {
        perror("Receive failed");
        printf("Test client_connection_and_communication: FAIL\n");
        close(sock);
        return 1;
    }
    recv_msg[bytes_recv] = '\0';

    printf("Received from server: %s\n", recv_msg);

    // نتحقق هل الرسالة المستلمة تحتوي على نص الإرسال (أو على الأقل ليست فارغة)
    if (strstr(recv_msg, "Hello") != NULL) {
        printf("Test client_connection_and_communication: PASS\n");
    } else {
        printf("Test client_connection_and_communication: FAIL\n");
    }

    close(sock);
    return 0;
}
