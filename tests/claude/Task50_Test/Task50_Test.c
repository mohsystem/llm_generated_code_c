#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>
#include <bits/signum-generic.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Original server code (do not modify)
void handle_upload(int client_socket) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;
    FILE *file;
    char filename[256];

    bytes_received = recv(client_socket, filename, sizeof(filename), 0);
    if (bytes_received <= 0) {
        perror("Error receiving filename");
        return;
    }
    filename[bytes_received] = '\0';

    // Store files under absolute path to match your test uploads directory
    const char* upload_dir = "/home/kali/CLionProjects/llm_generated_code_c/tests/claude/Task50_Test/uploads";
    mkdir(upload_dir, 0755); // Ensure directory exists

    char filepath[512];
    snprintf(filepath, sizeof(filepath), "%s/%s", upload_dir, filename);

    file = fopen(filepath, "wb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    while ((bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, 1, bytes_received, file);
    }

    fclose(file);

    const char *response = "File uploaded successfully\n";
    send(client_socket, response, strlen(response), 0);
}

int file_exists(const char* path) {
    return access(path, F_OK) == 0;
}

// Test client to upload file via socket (simulates Python test)
int client_upload_file(const char* filepath) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket error");
        return 1;
    }

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connect error");
        close(sock);
        return 1;
    }

    // Extract filename from filepath
    const char* filename = strrchr(filepath, '/');
    filename = (filename) ? filename + 1 : filepath;

    // Send filename first
    send(sock, filename, strlen(filename), 0);

    // Small pause to separate filename and content
    usleep(100000);

    FILE *file = fopen(filepath, "rb");
    if (!file) {
        perror("File open error");
        close(sock);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    size_t n;
    while ((n = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        send(sock, buffer, n, 0);
    }
    fclose(file);

    shutdown(sock, SHUT_WR);

    char response[BUFFER_SIZE] = {0};
    recv(sock, response, sizeof(response) - 1, 0);
    printf("Server response: %s\n", response);

    close(sock);
    return 0;
}

// Run test cases from python code strictly
void run_tests() {
    const char* test_file_path = "/home/kali/CLionProjects/llm_generated_code_c/tests/claude/Task50_Test/testFile.txt";
    const char* uploaded_file_path = "/home/kali/CLionProjects/llm_generated_code_c/tests/claude/Task50_Test/uploads/testFile.txt";

    // Make sure uploads directory exists
    mkdir("/home/kali/CLionProjects/llm_generated_code_c/tests/claude/Task50_Test/uploads", 0755);

    printf("Test 1: Upload testFile.txt... ");
    int res = client_upload_file(test_file_path);
    if (res != 0) {
        printf("FAIL (client upload failed)\n");
        return;
    }

    if (file_exists(uploaded_file_path)) {
        printf("PASS\n");
    } else {
        printf("FAIL (file not found in uploads directory)\n");
    }
}

int main() {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process: run server
        int server_socket, client_socket;
        struct sockaddr_in server_addr, client_addr;
        socklen_t client_len = sizeof(client_addr);

        server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket == -1) {
            perror("Error creating socket");
            exit(EXIT_FAILURE);
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(PORT);

        if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("Error binding socket");
            exit(EXIT_FAILURE);
        }

        if (listen(server_socket, 5) < 0) {
            perror("Error listening");
            exit(EXIT_FAILURE);
        }

        printf("Server listening on port %d\n", PORT);

        while (1) {
            client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
            if (client_socket < 0) {
                perror("Error accepting connection");
                continue;
            }
            handle_upload(client_socket);
        }

        close(server_socket);
        exit(0);
    } else if (pid > 0) {
        // Parent process: wait for server to start
        sleep(1);
        run_tests();

        // Kill child server after test
        kill(pid, SIGKILL);
    } else {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}
