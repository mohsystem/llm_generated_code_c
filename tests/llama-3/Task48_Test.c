#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9090
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    char send_msg[] = "Test message from client";
    char recv_msg[BUFFER_SIZE];
    int bytes_sent, bytes_recv;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        printf("Test client_connection_and_communication: FAIL\n");
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address");
        printf("Test client_connection_and_communication: FAIL\n");
        close(sock);
        return 1;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connect failed");
        printf("Test client_connection_and_communication: FAIL\n");
        close(sock);
        return 1;
    }

    bytes_sent = send(sock, send_msg, strlen(send_msg), 0);
    if (bytes_sent < 0) {
        perror("Send failed");
        printf("Test client_connection_and_communication: FAIL\n");
        close(sock);
        return 1;
    }

    bytes_recv = recv(sock, recv_msg, BUFFER_SIZE - 1, 0);
    if (bytes_recv < 0) {
        perror("Receive failed");
        printf("Test client_connection_and_communication: FAIL\n");
        close(sock);
        return 1;
    }

    recv_msg[bytes_recv] = '\0';
    printf("Received from server: %s\n", recv_msg);

    // تحقق أن الرد ليس فارغًا
    if (bytes_recv > 0) {
        printf("Test client_connection_and_communication: PASS\n");
    } else {
        printf("Test client_connection_and_communication: FAIL\n");
    }

    close(sock);
    return 0;
}
