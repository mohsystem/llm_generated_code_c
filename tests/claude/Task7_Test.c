#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

typedef struct {
    char username[50];
    char password[50];
} Credentials;

typedef struct {
    char status[10];
    char message[100];
} Response;

void *handle_client(void *arg);
Response authenticate(Credentials creds);

// نسخة مبسطة من سيرفر يعمل في thread منفصل
void *server_thread_func(void *arg) {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    pthread_t thread_id;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("socket failed");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(1);
    }
    if (listen(server_sock, 5) < 0) {
        perror("listen failed");
        exit(1);
    }

    while (1) {
        addr_size = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
        if (client_sock < 0) {
            perror("accept failed");
            continue;
        }
        pthread_create(&thread_id, NULL, handle_client, (void*)&client_sock);
        pthread_detach(thread_id);
    }
    return NULL;
}

int run_client(const char *username, const char *password, const char *expected_message) {
    int sock;
    struct sockaddr_in addr;
    Credentials creds;
    Response response;
    int ret;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("client socket");
        return -1;
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5000);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    ret = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    if (ret < 0) {
        perror("client connect");
        close(sock);
        return -1;
    }

    strncpy(creds.username, username, sizeof(creds.username)-1);
    creds.username[sizeof(creds.username)-1] = 0;
    strncpy(creds.password, password, sizeof(creds.password)-1);
    creds.password[sizeof(creds.password)-1] = 0;

    send(sock, &creds, sizeof(creds), 0);
    ret = recv(sock, &response, sizeof(response), 0);
    close(sock);

    if (ret <= 0) {
        printf("FAIL: No response from server\n");
        return 0;
    }

    if (strcmp(response.message, expected_message) == 0) {
        printf("PASS: Received expected response \"%s\"\n", response.message);
        return 1;
    } else {
        printf("FAIL: Expected \"%s\" but got \"%s\"\n", expected_message, response.message);
        return 0;
    }
}

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

// الدالة التي تحقق المصادقة
Response authenticate(Credentials creds) {
    Response response;
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) {
        strcpy(response.status, "failure");
        strcpy(response.message, "Error opening users file");
        return response;
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        char username[50], password[50];
        sscanf(line, "%[^:]:%s", username, password);
        if (strcmp(username, creds.username) == 0 && strcmp(password, creds.password) == 0) {
            strcpy(response.status, "success");
            strcpy(response.message, "Login successful");
            fclose(file);
            return response;
        }
    }
    fclose(file);

    strcpy(response.status, "failure");
    strcpy(response.message, "Invalid credentials");
    return response;
}

void *handle_client(void *arg) {
    int client_sock = *(int*)arg;
    Credentials creds;
    Response response;

    recv(client_sock, &creds, sizeof(creds), 0);
    response = authenticate(creds);
    send(client_sock, &response, sizeof(response), 0);

    close(client_sock);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t server_thread;
    int passed_tests = 0;
    int total_tests = 2;

    // إعداد ملف المستخدمين
    create_users_file();

    // تشغيل السيرفر في thread منفصل
    if (pthread_create(&server_thread, NULL, server_thread_func, NULL) != 0) {
        perror("Failed to start server thread");
        return 1;
    }
    // انتظر قليلاً حتى يشتغل السيرفر
    sleep(1);

    printf("Running test cases:\n");

    // حالة 1: بيانات صحيحة
    if (run_client("user1", "pass1", "Login successful")) passed_tests++;

    // حالة 2: بيانات خاطئة
    if (run_client("user1", "wrongpass", "Invalid credentials")) passed_tests++;

    // حالة 3: ملف المستخدمين غير موجود → لم يتم تنفيذها لأن ملف المستخدمين موجود
    printf("FAIL: Test case for missing users.txt file - not implemented\n");

    // نتائج عامة
    printf("\nTest cases passed: %d/%d\n", passed_tests, total_tests);
    if (passed_tests == total_tests)
        printf("All test cases PASSED\n");
    else
        printf("Some test cases FAILED\n");

    // أوقف السيرفر
    // (لتبسيط لا يوجد آلية لإيقافه هنا، سيتوقف عند إيقاف البرنامج)

    return 0;
}
