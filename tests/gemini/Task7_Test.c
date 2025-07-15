#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 12345
#define MAX_CLIENTS 10

// Structure to store client information
typedef struct {
    int sockfd;
    char username[50];
} Client;

// Global variables
Client clients[MAX_CLIENTS];
int num_clients = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to handle client connections
void *handle_client(void *arg) {
    Client *client = (Client *)arg;
    char buffer[1024];
    int bytes_read;

    // Authentication
    while (1) {
        // Receive username
        send(client->sockfd, "Enter username: ", 16, 0);
        bytes_read = recv(client->sockfd, buffer, sizeof(buffer), 0);
        if (bytes_read <= 0) {
            close(client->sockfd);
            pthread_exit(NULL);
        }
        buffer[bytes_read] = '\0';
        strncpy(client->username, buffer, sizeof(client->username) - 1);

        // Receive password
        send(client->sockfd, "Enter password: ", 16, 0);
        bytes_read = recv(client->sockfd, buffer, sizeof(buffer), 0);
        if (bytes_read <= 0) {
            close(client->sockfd);
            pthread_exit(NULL);
        }
        buffer[bytes_read] = '\0';

        // Authenticate user (replace with actual authentication logic)
        if (strcmp(client->username, "user") == 0 && strcmp(buffer, "password") == 0) {
            send(client->sockfd, "Login successful!\n", 18, 0);
            break;
        } else {
            send(client->sockfd, "Invalid username or password!\n", 30, 0);
        }
    }

    // Send join message to all clients
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < num_clients; i++) {
        if (clients[i].sockfd != client->sockfd) {
            sprintf(buffer, "%s has joined the chat.\n", client->username);
            send(clients[i].sockfd, buffer, strlen(buffer), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    // Chat loop
    while ((bytes_read = recv(client->sockfd, buffer, sizeof(buffer), 0)) > 0) {
        buffer[bytes_read] = '\0';

        // Send message to all clients
        pthread_mutex_lock(&clients_mutex);
        for (int i = 0; i < num_clients; i++) {
            if (clients[i].sockfd != client->sockfd) {
                sprintf(buffer, "%s: %s\n", client->username, buffer);
                send(clients[i].sockfd, buffer, strlen(buffer), 0);
            }
        }
        pthread_mutex_unlock(&clients_mutex);
    }

    // Send leave message to all clients
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < num_clients; i++) {
        if (clients[i].sockfd != client->sockfd) {
            sprintf(buffer, "%s has left the chat.\n", client->username);
            send(clients[i].sockfd, buffer, strlen(buffer), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    // Remove client from list
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < num_clients; i++) {
        if (clients[i].sockfd == client->sockfd) {
            for (int j = i; j < num_clients - 1; j++) {
                clients[j] = clients[j + 1];
            }
            num_clients--;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    close(client->sockfd);
    free(client);
    pthread_exit(NULL);
}

// Client test function
int run_client(const char *username, const char *password, const char *expected_response) {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[1024];
    ssize_t n;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket");
        return 0;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connect");
        close(sock);
        return 0;
    }

    // Receive "Enter username: "
    n = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (n <= 0) { close(sock); return 0; }
    buffer[n] = '\0';

    // Send username + newline
    send(sock, username, strlen(username), 0);
    send(sock, "\n", 1, 0);

    // Receive "Enter password: "
    n = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (n <= 0) { close(sock); return 0; }
    buffer[n] = '\0';

    // Send password + newline
    send(sock, password, strlen(password), 0);
    send(sock, "\n", 1, 0);

    // Receive response
    n = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (n <= 0) { close(sock); return 0; }
    buffer[n] = '\0';

    close(sock);

    // Remove trailing newlines
    for (int i = 0; buffer[i]; i++) {
        if (buffer[i] == '\n' || buffer[i] == '\r') buffer[i] = 0;
    }

    if (strcmp(buffer, expected_response) == 0) {
        printf("PASS: Received expected response \"%s\"\n", buffer);
        return 1;
    } else {
        printf("FAIL: Expected \"%s\" but got \"%s\"\n", expected_response, buffer);
        return 0;
    }
}

// Server main loop running in thread
void *server_main(void *arg) {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_size;
    pthread_t thread_id;

    (void)arg; // unused

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Failed to create socket");
        exit(1);
    }

    // Set server address
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Failed to bind socket");
        close(server_socket);
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1) {
        perror("Failed to listen on socket");
        close(server_socket);
        exit(1);
    }

    // Accept and handle client connections
    while (1) {
        client_address_size = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_size);
        if (client_socket == -1) {
            perror("Failed to accept client connection");
            continue;
        }

        // Create new client and add to list
        pthread_mutex_lock(&clients_mutex);
        if (num_clients < MAX_CLIENTS) {
            Client *client = (Client *)malloc(sizeof(Client));
            client->sockfd = client_socket;
            clients[num_clients++] = *client;

            // Create thread to handle client
            if (pthread_create(&thread_id, NULL, handle_client, (void *)client) != 0) {
                perror("Failed to create client thread");
                close(client_socket);
                free(client);
                num_clients--;
            } else {
                pthread_detach(thread_id);
            }
        } else {
            send(client_socket, "Server full!\n", 13, 0);
            close(client_socket);
        }
        pthread_mutex_unlock(&clients_mutex);
    }

    close(server_socket);
    return NULL;
}

int main() {
    pthread_t server_thread;
    int passed = 0;
    int total = 2;

    if (pthread_create(&server_thread, NULL, server_main, NULL) != 0) {
        perror("Failed to start server thread");
        return 1;
    }

    sleep(1); // wait for server to start

    printf("Running test cases:\n");

    if (run_client("user", "password", "Login successful!")) passed++;
    if (run_client("user", "wrongpass", "Invalid username or password!")) passed++;

    printf("FAIL: Test case for missing user file - not implemented\n");

    printf("\nTest cases passed: %d/%d\n", passed, total);
    if (passed == total) {
        printf("All test cases PASSED\n");
    } else {
        printf("Some test cases FAILED\n");
    }

    // Note: No clean server shutdown here; program ends after tests.

    return 0;
}
