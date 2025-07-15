// Full C program: server + test cases strictly matching the Python prompt cases
// The server accepts TCP connections, expects client to send "username:password" string,
// checks users.txt for matching line, responds with "Login successful\n" or "Login failed\n".

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 12345
#define BUFFER_SIZE 1024

// Create a temporary users.txt file for testing
void create_temp_users_file() {
    FILE *f = fopen("users.txt", "w");
    if (!f) {
        perror("Failed to create users.txt");
        exit(1);
    }
    fprintf(f, "user:password\n");  // one valid user line exactly
    fclose(f);
}

// Server thread handler function
void *handle_client(void *socket_desc) {
    int sock = *(int *)socket_desc;
    free(socket_desc);
    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE];
    ssize_t read_size;

    while ((read_size = recv(sock, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[read_size] = '\0';

        // Check format username:password
        char *delimiter = strchr(buffer, ':');
        if (!delimiter) {
            strcpy(message, "Invalid format. Use username:password\n");
            send(sock, message, strlen(message), 0);
            continue;
        }

        *delimiter = '\0';
        char *username = buffer;
        char *password = delimiter + 1;

        FILE *users_file = fopen("users.txt", "r");
        if (!users_file) {
            strcpy(message, "Unable to open users.txt\n");
            send(sock, message, strlen(message), 0);
            break;
        }

        char users_line[BUFFER_SIZE];
        int auth = 0;
        while (fgets(users_line, BUFFER_SIZE, users_file)) {
            users_line[strcspn(users_line, "\r\n")] = 0; // strip newline
            if (strcmp(users_line, buffer) == 0) {
                auth = 1;
                break;
            }
        }
        fclose(users_file);

        if (auth) {
            strcpy(message, "Login successful\n");
        } else {
            strcpy(message, "Login failed\n");
        }

        send(sock, message, strlen(message), 0);
    }

    close(sock);
    return NULL;
}

// Test client function: connect, send "username:password", receive response, check it
int run_client(const char *username, const char *password, const char *expected_response) {
    int sock;
    struct sockaddr_in server_addr;
    char send_buffer[BUFFER_SIZE];
    char recv_buffer[BUFFER_SIZE];
    ssize_t n;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket");
        return 0;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connect");
        close(sock);
        return 0;
    }

    snprintf(send_buffer, sizeof(send_buffer), "%s:%s", username, password);
    send(sock, send_buffer, strlen(send_buffer), 0);

    n = recv(sock, recv_buffer, sizeof(recv_buffer) - 1, 0);
    if (n <= 0) {
        close(sock);
        printf("FAIL: No response from server\n");
        return 0;
    }
    recv_buffer[n] = '\0';

    close(sock);

    // strip trailing \r\n
    for (int i = 0; recv_buffer[i]; i++) {
        if (recv_buffer[i] == '\r' || recv_buffer[i] == '\n') recv_buffer[i] = 0;
    }

    if (strcmp(recv_buffer, expected_response) == 0) {
        printf("PASS: Received expected response \"%s\"\n", recv_buffer);
        return 1;
    } else {
        printf("FAIL: Expected \"%s\" but got \"%s\"\n", expected_response, recv_buffer);
        return 0;
    }
}

int main() {
    pthread_t server_thread;
    int server_socket, new_socket, *new_sock;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    create_temp_users_file();

    // Start server thread
    void *server_main(void *);
    pthread_create(&server_thread, NULL, server_main, NULL);
    sleep(1); // wait for server to start

    printf("Running test cases:\n");
    int passed = 0, total = 3;

    // Test 1: correct username and password
    if (run_client("user", "password", "Login successful")) passed++;

    // Test 2: incorrect password
    if (run_client("user", "wrongpass", "Login failed")) passed++;

    // Test 3: missing users.txt
    remove("users.txt"); // remove file to simulate missing file

    // Connect and send login, expect "Unable to open users.txt"
    socklen_t len = sizeof(address);
    socklen_t unused_len = 0;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock >= 0) {
        address.sin_family = AF_INET;
        address.sin_port = htons(PORT);
        inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);
        if (connect(sock, (struct sockaddr *)&address, sizeof(address)) == 0) {
            char buf[BUFFER_SIZE];
            snprintf(buf, sizeof(buf), "user:password");
            send(sock, buf, strlen(buf), 0);

            ssize_t n = recv(sock, buf, sizeof(buf) - 1, 0);
            if (n > 0) {
                buf[n] = '\0';
                // strip newlines
                for (int i = 0; buf[i]; i++) {
                    if (buf[i] == '\r' || buf[i] == '\n') buf[i] = 0;
                }
                if (strcmp(buf, "Unable to open users.txt") == 0) {
                    printf("PASS: Received expected response \"%s\"\n", buf);
                    passed++;
                } else {
                    printf("FAIL: Expected \"Unable to open users.txt\" but got \"%s\"\n", buf);
                }
            } else {
                printf("FAIL: No response for missing users.txt test\n");
            }
            close(sock);
        } else {
            printf("FAIL: Could not connect for missing users.txt test\n");
            close(sock);
        }
    } else {
        printf("FAIL: Could not create socket for missing users.txt test\n");
    }

    printf("\nTest cases passed: %d/%d\n", passed, total);
    if (passed == total) {
        printf("All test cases PASSED\n");
    } else {
        printf("Some test cases FAILED\n");
    }

    return 0;
}

// Server main function for thread
void *server_main(void *arg) {
    (void)arg;
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    while ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen))) {
        int *pclient = malloc(sizeof(int));
        *pclient = new_socket;
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, pclient) != 0) {
            perror("Thread creation failed");
            free(pclient);
        } else {
            pthread_detach(thread_id);
        }
    }
    if (new_socket < 0) {
        perror("Accept failed");
    }
    close(server_fd);
    return NULL;
}
