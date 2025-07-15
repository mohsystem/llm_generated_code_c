#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 65432
#define BUFFER_SIZE 1024
#define FILE_PATH "/home/kali/CLionProjects/llm_generated_code_c/tests/gemini/Task50_Test/testFile.txt"

// ===== السيرفر =====

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    char filename[BUFFER_SIZE];
    long file_size;

    if (recv(client_socket, filename, BUFFER_SIZE, 0) <= 0) {
        perror("Error receiving filename");
        return;
    }

    if (recv(client_socket, buffer, BUFFER_SIZE, 0) <= 0) {
        perror("Error receiving filesize");
        return;
    }
    file_size = strtol(buffer, NULL, 10);

    printf("Receiving file: %s (%ld bytes)\n", filename, file_size);

    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    long total_bytes_received = 0;
    while (total_bytes_received < file_size) {
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            perror("Error receiving file data");
            break;
        }
        fwrite(buffer, 1, bytes_received, fp);
        total_bytes_received += bytes_received;
    }

    fclose(fp);

    printf("%s uploaded successfully!\n", filename);
    send(client_socket, "File uploaded successfully!", 26, 0);
}

void run_server() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_size = sizeof(client_address);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(1);
    }

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Error binding socket");
        close(server_socket);
        exit(1);
    }

    if (listen(server_socket, 5) == -1) {
        perror("Error listening for connections");
        close(server_socket);
        exit(1);
    }

    printf("Server started on port %d\n", PORT);

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_size);
        if (client_socket == -1) {
            perror("Error accepting connection");
            continue;
        }

        printf("Client connected\n");
        handle_client(client_socket);
        close(client_socket);
    }

    close(server_socket);
}

// ===== العميل (Client) =====

void run_client() {
    int sockfd;
    struct sockaddr_in serv_addr;
    FILE *fp;
    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    long file_size;
    size_t bytes_read;
    int bytes_sent, bytes_received;
    int total_bytes_sent;
    int ret;

    fp = fopen(FILE_PATH, "rb");
    if (fp == NULL) {
        perror("Failed to open file");
        printf("Test case: FAIL\n");
        return;
    }

    fseek(fp, 0L, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation error");
        fclose(fp);
        printf("Test case: FAIL\n");
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address");
        fclose(fp);
        close(sockfd);
        printf("Test case: FAIL\n");
        return;
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        fclose(fp);
        close(sockfd);
        printf("Test case: FAIL\n");
        return;
    }

    memset(buffer, 0, BUFFER_SIZE);
    const char *filename_only = "testFile.txt";
    strncpy(buffer, filename_only, BUFFER_SIZE - 1);
    bytes_sent = send(sockfd, buffer, BUFFER_SIZE, 0);
    if (bytes_sent != BUFFER_SIZE) {
        perror("Failed to send filename");
        fclose(fp);
        close(sockfd);
        printf("Test case: FAIL\n");
        return;
    }

    memset(buffer, 0, BUFFER_SIZE);
    snprintf(buffer, BUFFER_SIZE, "%ld", file_size);
    bytes_sent = send(sockfd, buffer, BUFFER_SIZE, 0);
    if (bytes_sent != BUFFER_SIZE) {
        perror("Failed to send filesize");
        fclose(fp);
        close(sockfd);
        printf("Test case: FAIL\n");
        return;
    }

    total_bytes_sent = 0;
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, fp)) > 0) {
        int sent = 0;
        while (sent < bytes_read) {
            ret = send(sockfd, buffer + sent, bytes_read - sent, 0);
            if (ret < 0) {
                perror("Failed to send file data");
                fclose(fp);
                close(sockfd);
                printf("Test case: FAIL\n");
                return;
            }
            sent += ret;
        }
        total_bytes_sent += bytes_read;
    }

    fclose(fp);

    bytes_received = recv(sockfd, response, BUFFER_SIZE - 1, 0);
    if (bytes_received <= 0) {
        perror("Failed to receive response");
        close(sockfd);
        printf("Test case: FAIL\n");
        return;
    }
    response[bytes_received] = '\0';

    if (strcmp(response, "File uploaded successfully!") == 0) {
        printf("Test case: PASS\n");
    } else {
        printf("Test case: FAIL\n");
        printf("Server response: %s\n", response);
    }

    close(sockfd);
}

// ===== main =====

int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "test") == 0) {
        run_client();
    } else {
        run_server();
    }
    return 0;
}
