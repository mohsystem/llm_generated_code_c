#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 12345
#define BUFFER_SIZE 1024

// ملف المستخدمين بسيط
void create_users_file() {
    FILE *f = fopen("users.txt", "w");
    if (!f) {
        perror("Failed to create users.txt");
        exit(1);
    }
    fprintf(f, "user1:pass1\n");
    fprintf(f, "user2:pass2\n");
    fclose(f);
}

// تحقق من صحة بيانات المستخدم من ملف users.txt
int authenticate(const char* username, const char* password) {
    FILE *f = fopen("users.txt", "r");
    if (!f) return -1; // ملف غير موجود

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        char file_user[128], file_pass[128];
        if (sscanf(line, "%[^:]:%s", file_user, file_pass) == 2) {
            if (strcmp(file_user, username) == 0 && strcmp(file_pass, password) == 0) {
                fclose(f);
                return 1; // مصدق
            }
        }
    }
    fclose(f);
    return 0; // غير مصدق
}

void *handle_client(void *arg) {
    int client_sock = *(int*)arg;
    free(arg);

    char buffer[BUFFER_SIZE];
    ssize_t n = recv(client_sock, buffer, BUFFER_SIZE - 1, 0);
    if (n <= 0) {
        close(client_sock);
        return NULL;
    }
    buffer[n] = '\0';

    // الصيغة المتوقعة: LOGIN|username|password
    char *cmd = strtok(buffer, "|");
    char *username = strtok(NULL, "|");
    char *password = strtok(NULL, "|");

    char response[BUFFER_SIZE];

    if (!cmd || !username || !password || strcmp(cmd, "LOGIN") != 0) {
        snprintf(response, sizeof(response), "FAILURE: Invalid request format");
    } else {
        int auth = authenticate(username, password);
        if (auth == 1) {
            snprintf(response, sizeof(response), "SUCCESS: Login successful");
        } else if (auth == 0) {
            snprintf(response, sizeof(response), "FAILURE: Invalid credentials");
        } else {
            snprintf(response, sizeof(response), "FAILURE: Error opening users file");
        }
    }

    send(client_sock, response, strlen(response), 0);
    close(client_sock);
    return NULL;
}

void *server_thread(void *arg) {
    int server_sock, *client_sock_ptr;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    pthread_t tid;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Socket failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    if (listen(server_sock, 5) < 0) {
        perror("Listen failed");
        exit(1);
    }

    while (1) {
        client_sock_ptr = malloc(sizeof(int));
        *client_sock_ptr = accept(server_sock, (struct sockaddr*)&client_addr, &client_len);
        if (*client_sock_ptr < 0) {
            free(client_sock_ptr);
            perror("Accept failed");
            continue;
        }
        pthread_create(&tid, NULL, handle_client, client_sock_ptr);
        pthread_detach(tid);
    }
    return NULL;
}

// دالة تشغيل العميل من الكود السابق
int run_client(const char *username, const char *password, const char *expected_response) {
    int sock;
    struct sockaddr_in server_addr;
    char recv_buffer[BUFFER_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Client socket");
        return 0;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Client connect");
        close(sock);
        return 0;
    }

    char send_buffer[BUFFER_SIZE];
    snprintf(send_buffer, sizeof(send_buffer), "LOGIN|%s|%s", username, password);
    send(sock, send_buffer, strlen(send_buffer), 0);

    ssize_t n = recv(sock, recv_buffer, sizeof(recv_buffer)-1, 0);
    if (n <= 0) {
        printf("FAIL: No response from server\n");
        close(sock);
        return 0;
    }
    recv_buffer[n] = '\0';
    close(sock);

    if (strcmp(recv_buffer, expected_response) == 0) {
        printf("PASS: Received expected response \"%s\"\n", recv_buffer);
        return 1;
    } else {
        printf("FAIL: Expected \"%s\" but got \"%s\"\n", expected_response, recv_buffer);
        return 0;
    }
}

int main() {
    pthread_t server_tid;
    int passed = 0;
    int total = 2;

    create_users_file();

    if (pthread_create(&server_tid, NULL, server_thread, NULL) != 0) {
        perror("Failed to start server thread");
        return 1;
    }

    sleep(1); // انتظر السيرفر ليبدأ

    printf("Running test cases:\n");

    // Test 1: بيانات صحيحة
    if (run_client("user1", "pass1", "SUCCESS: Login successful")) passed++;

    // Test 2: بيانات خاطئة
    if (run_client("user1", "wrongpass", "FAILURE: Invalid credentials")) passed++;

    // Test 3: ملف المستخدمين غير موجود - غير مفعّل في السيرفر
    printf("FAIL: Test case for missing users.txt file - not implemented\n");

    printf("\nTest cases passed: %d/%d\n", passed, total);
    if (passed == total) {
        printf("All test cases PASSED\n");
    } else {
        printf("Some test cases FAILED\n");
    }

    return 0;
}
