// Strictly the provided C code with test cases result printing as per Python prompt
// This C server listens on port 8080 and sends a fixed JSON response with CORS headers
// Compile with: gcc -o cors_server cors_server.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char *response = "HTTP/1.1 200 OK\r\n"
                     "Access-Control-Allow-Origin: *\r\n"
                     "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n"
                     "Access-Control-Allow-Headers: Content-Type\r\n"
                     "Content-Type: application/json\r\n\r\n"
                     "{\"message\": \"This is CORS-enabled data\"}";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    // For test case demonstration: we will accept only one connection and then print PASS or FAIL
    // because in real use it runs infinitely.

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    int valread = read(new_socket, buffer, BUFFER_SIZE);
    if (valread > 0) {
        printf("Received request:\n%s\n", buffer);
    } else {
        printf("Failed to read request\n");
        close(new_socket);
        close(server_fd);
        printf("Test case result: FAIL\n");
        return 1;
    }

    ssize_t sent = write(new_socket, response, strlen(response));
    if (sent == strlen(response)) {
        printf("Sent CORS-enabled JSON response successfully.\n");
        printf("Test case: PASS\n");
    } else {
        printf("Failed to send full response\n");
        printf("Test case: FAIL\n");
    }

    close(new_socket);
    close(server_fd);

    return 0;
}
