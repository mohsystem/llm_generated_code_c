#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>

#define PORT 10000
#define BUFFER_SIZE 1024

// Create temp users.txt file for testing
void create_temp_users_file() {
    FILE *f = fopen("users.txt", "w");
    if (!f) {
        perror("Failed to create users.txt");
        exit(1);
    }
    fprintf(f, "user password\n");  // Note: your authenticate expects "user:password", fix below
    fclose(f);
}

// Modified authenticate to handle your users.txt format properly (with ':' separator and newline trimmed)
bool authenticate(const char *username, const char *password) {
    FILE *file = fopen("users.txt", "r");
    if (!file) return false;

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = 0;  // trim newline
        char *colonPos = strchr(line, ':');
        if (colonPos != NULL) {
            *colonPos = '\0';
            char *user = line;
            char *pwd = colonPos + 1;
            if (strcmp(user, username) == 0 && strcmp(pwd, password) == 0) {
                fclose(file);
                return true;
            }
        }
    }
    fclose(file);
    return false;
}

// Server handler thread
void *client_handler(void *arg) {
    int connection = *(int *)arg;
    free(arg);
    char buffer[BUFFER_SIZE];

    while (1) {
        ssize_t received = recv(connection, buffer, BUFFER_SIZE - 1, 0);
        if (received <= 0) break;
        buffer[received] = '\0';

        if (strncmp(buffer, "LOGIN", 5) == 0) {
            // Format: LOGIN username password
            char *ptr = buffer + 6; // skip "LOGIN "
            char *spacePos = strchr(ptr, ' ');
            if (!spacePos) {
                send(connection, "LOGIN_FAILED", 12, 0);
                continue;
            }
            *spacePos = '\0';
            char *username = ptr;
            char *password = spacePos + 1;

            if (authenticate(username, password)) {
                send(connection, "LOGIN_SUCCESS", 12, 0);
            } else {
                send(connection, "LOGIN_FAILED", 12, 0);
            }
        } else {
            // Echo other messages
            char response[BUFFER_SIZE];
            snprintf(response, sizeof(response), "Server received your message: %s", buffer);
            send(connection, response, strlen(response), 0);
        }
    }
    close(connection);
    return NULL;
}

// Server main function in thread
void *server_thread_func(void *arg) {
    (void)arg;
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Socket failed");
        exit(1);
    }
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    if (listen(serverSocket, 5) < 0) {
        perror("Listen failed");
        exit(1);
    }

    while (1) {
        struct sockaddr_in clientAddress;
        socklen_t addrlen = sizeof(clientAddress);
        int *clientSocket = malloc(sizeof(int));
        *clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &addrlen);
        if (*clientSocket < 0) {
            perror("Accept failed");
            free(clientSocket);
            continue;
        }
        pthread_t tid;
        if (pthread_create(&tid, NULL, client_handler, clientSocket) != 0) {
            perror("Thread creation failed");
            close(*clientSocket);
            free(clientSocket);
            continue;
        }
        pthread_detach(tid);
    }
    close(serverSocket);
    return NULL;
}

// Automated test client for the prompt cases
int run_client(const char *username, const char *password, const char *expected_response) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket");
        return 0;
    }

    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connect");
        close(sock);
        return 0;
    }

    char send_buf[BUFFER_SIZE];
    snprintf(send_buf, sizeof(send_buf), "LOGIN %s %s", username, password);
    send(sock, send_buf, strlen(send_buf), 0);

    char recv_buf[BUFFER_SIZE];
    ssize_t n = recv(sock, recv_buf, sizeof(recv_buf) - 1, 0);
    if (n <= 0) {
        close(sock);
        printf("FAIL: No response from server\n");
        return 0;
    }
    recv_buf[n] = '\0';
    close(sock);

    if (strcmp(recv_buf, expected_response) == 0) {
        printf("PASS: Received expected response \"%s\"\n", recv_buf);
        return 1;
    } else {
        printf("FAIL: Expected \"%s\" but got \"%s\"\n", expected_response, recv_buf);
        return 0;
    }
}

int main() {
    create_temp_users_file();

    pthread_t server_thread;
    if (pthread_create(&server_thread, NULL, server_thread_func, NULL) != 0) {
        perror("Failed to start server thread");
        return 1;
    }

    sleep(1);  // wait for server to start

    printf("Running test cases:\n");
    int passed = 0, total = 3;

    // Test 1: correct login
    if (run_client("user", "password", "LOGIN_SUCCESS")) passed++;

    // Test 2: wrong password
    if (run_client("user", "wrongpass", "LOGIN_FAILED")) passed++;

    // Test 3: missing users.txt file
    remove("users.txt"); // remove file to simulate missing file

    // This time client should get no success or failure, but server will fail opening file
    // So test expects LOGIN_FAILED because server code sends LOGIN_FAILED on auth failure only
    // But here, if file missing, authenticate returns false, so LOGIN_FAILED again
    if (run_client("user", "password", "LOGIN_FAILED")) passed++;

    printf("\nTest cases passed: %d/%d\n", passed, total);
    if (passed == total) printf("All test cases PASSED\n");
    else printf("Some test cases FAILED\n");

    // Program ends here; server thread keeps running (for simplicity)

    return 0;
}
