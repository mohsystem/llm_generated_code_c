#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000
#define SERVER_IP "127.0.0.1"  // أو عنوان السيرفر الحقيقي

int send_command(const char *command, char *response, size_t response_size) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address");
        close(sock);
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        return -1;
    }

    send(sock, command, strlen(command), 0);
    int valread = read(sock, buffer, sizeof(buffer)-1);
    if (valread > 0) {
        buffer[valread] = '\0';
        strncpy(response, buffer, response_size-1);
        response[response_size-1] = '\0';
    } else {
        response[0] = '\0';
    }

    close(sock);
    return 0;
}

int main() {
    char response[1024];

    // Test case 1: Create object "obj" with value 0
    if (send_command("CREATE obj 0", response, sizeof(response)) == 0 && strstr(response, "Object created")) {
        printf("Test case 1 (Create with initial 0): PASS\n");
    } else {
        printf("Test case 1 (Create with initial 0): FAIL\n");
    }

    // Test case 2: Set value 5 for "obj"
    if (send_command("SET obj 5", response, sizeof(response)) == 0 && strstr(response, "Value updated")) {
        printf("Test case 2 (Set value 5): PASS\n");
    } else {
        printf("Test case 2 (Set value 5): FAIL\n");
    }

    // Test case 3: Increment value (not supported)
    printf("Test case 3 (Increment): FAIL not implemented\n");

    // Test case 4: Get value for "obj" and check if 5
    if (send_command("GET obj", response, sizeof(response)) == 0) {
        if (strstr(response, "Value: 5") != NULL) {
            printf("Test case 4 (Get value): PASS\n");
        } else {
            printf("Test case 4 (Get value): FAIL\n");
        }
    } else {
        printf("Test case 4 (Get value): FAIL\n");
    }

    return 0;
}
