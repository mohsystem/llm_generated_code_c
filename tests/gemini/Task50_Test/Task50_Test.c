#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>

#define PORT 65432
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

int upload_file(const char *filepath) {
    int sock;
    struct sockaddr_in server_addr;
    FILE *fp;
    char buffer[BUFFER_SIZE];
    char filename[256];
    struct stat st;

    // افتح الملف
    fp = fopen(filepath, "rb");
    if (!fp) {
        perror("Error opening file");
        return 0;
    }

    // احصل اسم الملف فقط من المسار
    const char *slash = strrchr(filepath, '/');
    if (slash)
        strncpy(filename, slash + 1, sizeof(filename) - 1);
    else
        strncpy(filename, filepath, sizeof(filename) - 1);
    filename[sizeof(filename)-1] = '\0';

    // احصل حجم الملف
    if (stat(filepath, &st) < 0) {
        perror("Error getting file size");
        fclose(fp);
        return 0;
    }
    long filesize = st.st_size;

    // انشئ socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation error");
        fclose(fp);
        return 0;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        close(sock);
        fclose(fp);
        return 0;
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        fclose(fp);
        return 0;
    }

    // أرسل اسم الملف
    if (send(sock, filename, sizeof(filename), 0) <= 0) {
        perror("Failed to send filename");
        close(sock);
        fclose(fp);
        return 0;
    }

    // أرسل حجم الملف (كـ string)
    snprintf(buffer, sizeof(buffer), "%ld", filesize);
    if (send(sock, buffer, sizeof(buffer), 0) <= 0) {
        perror("Failed to send filesize");
        close(sock);
        fclose(fp);
        return 0;
    }

    // أرسل بيانات الملف
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, fp)) > 0) {
        if (send(sock, buffer, bytes_read, 0) <= 0) {
            perror("Failed to send file data");
            close(sock);
            fclose(fp);
            return 0;
        }
    }

    // استقبل رد السيرفر
    int bytes_received = recv(sock, buffer, sizeof(buffer)-1, 0);
    if (bytes_received <= 0) {
        perror("Failed to receive server response");
        close(sock);
        fclose(fp);
        return 0;
    }
    buffer[bytes_received] = '\0';
    printf("Server response: %s\n", buffer);

    close(sock);
    fclose(fp);

    // تحقق من رد السيرفر
    if (strstr(buffer, "successfully") != NULL) {
        return 1; // PASS
    } else {
        return 0; // FAIL
    }
}

int main() {
    // مثال Test case: رفع testFile.txt
    const char *test_file_path = "/home/kali/CLionProjects/llm_generated_code_c/tests/gemini/Task50_Test/testFile.txt";

    printf("Test case 1: Upload file %s\n", test_file_path);
    int result = upload_file(test_file_path);

    if (result)
        printf("Test case 1 result: PASS\n");
    else
        printf("Test case 1 result: FAIL\n");

    return 0;
}
