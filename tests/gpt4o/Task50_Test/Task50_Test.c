#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/stat.h>   // mkdir
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFSIZE 1024
#define UPLOAD_DIR "uploads/"
#define FILE_PATH "/home/kali/CLionProjects/llm_generated_code_c/tests/gpt4o/Task50_Test/uploads/testFile.txt"

// رسالة خطأ وانهاء
void error_handling(const char *message) {
    perror(message);
    exit(1);
}

// ====== السيرفر ======
void run_server() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFSIZE] = {0};

    mkdir(UPLOAD_DIR, 0777);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        error_handling("socket failed");
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        error_handling("bind failed");
    }

    if (listen(server_fd, 3) < 0) {
        error_handling("listen failed");
    }

    printf("Server started on port %d\n", PORT);

    while ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) >= 0) {
        ssize_t valread = read(new_socket, buffer, BUFSIZE);
        if (valread > 0) {
            FILE *fp;
            char filepath[BUFSIZE];
            snprintf(filepath, BUFSIZE, "%suploaded_file", UPLOAD_DIR);

            fp = fopen(filepath, "wb");
            if (fp != NULL) {
                fwrite(buffer, sizeof(char), valread, fp);
                fclose(fp);
                send(new_socket, "File successfully uploaded", strlen("File successfully uploaded"), 0);
                printf("File saved: %s (%ld bytes)\n", filepath, (long)valread);
            } else {
                send(new_socket, "Error saving file", strlen("Error saving file"), 0);
            }
        } else {
            send(new_socket, "Error reading data", strlen("Error reading data"), 0);
        }

        close(new_socket);
        memset(buffer, 0, BUFSIZE);
    }
}

// ====== العميل (التست كيس) ======
void run_client() {
    int sockfd;
    struct sockaddr_in serv_addr;
    FILE *fp;
    char buffer[BUFSIZE];
    char response[BUFSIZE];
    size_t bytes_read;
    int bytes_sent;
    int bytes_received;

    // فتح الملف
    fp = fopen(FILE_PATH, "rb");
    if (fp == NULL) {
        perror("Failed to open file");
        printf("Test case: FAIL\n");
        return;
    }

    // قراءة أول 1024 بايت فقط (كما سيرفرك يستقبل مرة وحدة 1024 بايت)
    bytes_read = fread(buffer, 1, BUFSIZE, fp);
    fclose(fp);

    // إنشاء سوكت TCP
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation error");
        printf("Test case: FAIL\n");
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address");
        close(sockfd);
        printf("Test case: FAIL\n");
        return;
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        close(sockfd);
        printf("Test case: FAIL\n");
        return;
    }

    // إرسال البيانات (1024 بايت أو أقل)
    bytes_sent = send(sockfd, buffer, bytes_read, 0);
    if (bytes_sent != bytes_read) {
        perror("Failed to send file data");
        close(sockfd);
        printf("Test case: FAIL\n");
        return;
    }

    // استقبال رد السيرفر
    bytes_received = recv(sockfd, response, BUFSIZE - 1, 0);
    if (bytes_received <= 0) {
        perror("Failed to receive response");
        close(sockfd);
        printf("Test case: FAIL\n");
        return;
    }

    response[bytes_received] = '\0';

    if (strcmp(response, "File successfully uploaded") == 0) {
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

