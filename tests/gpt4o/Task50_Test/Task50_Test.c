#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"
#define BUFSIZE 1024

int send_file(const char *filepath) {
    int sock;
    struct sockaddr_in serv_addr;
    FILE *fp;
    char buffer[BUFSIZE];
    size_t bytes_read;

    // افتح الملف للقراءة الثنائية
    fp = fopen(filepath, "rb");
    if (fp == NULL) {
        perror("Cannot open file");
        return 0;
    }

    // انشئ سوكيت
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        fclose(fp);
        return 0;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address");
        close(sock);
        fclose(fp);
        return 0;
    }

    // اتصل بالسيرفر
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        fclose(fp);
        return 0;
    }

    // اقرأ أول 1024 بايت (أو أقل لو الملف أصغر)
    bytes_read = fread(buffer, 1, BUFSIZE, fp);
    fclose(fp);

    if (bytes_read <= 0) {
        fprintf(stderr, "File is empty or error reading\n");
        close(sock);
        return 0;
    }

    // أرسل البيانات دفعة واحدة
    if (send(sock, buffer, bytes_read, 0) != bytes_read) {
        perror("Failed to send file data");
        close(sock);
        return 0;
    }

    // استقبل رد السيرفر
    int valread = read(sock, buffer, BUFSIZE - 1);
    if (valread <= 0) {
        perror("Failed to receive response");
        close(sock);
        return 0;
    }
    buffer[valread] = '\0';
    printf("Server response: %s\n", buffer);

    close(sock);

    // تحقق من رد السيرفر
    if (strstr(buffer, "successfully") != NULL)
        return 1;  // pass
    else
        return 0;  // fail
}

int main() {
    const char *testfile = "/home/kali/CLionProjects/llm_generated_code_c/tests/gpt4o/Task50_Test/testFile.txt";  // مسار ملف الاختبار

    printf("Test case: send file %s\n", testfile);

    int result = send_file(testfile);

    if (result)
        printf("Test case result: PASS\n");
    else
        printf("Test case result: FAIL\n");

    return 0;
}
