#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>

char* generateToken(const char* username, const char* password) {
    char timestamp[20];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", &tm);
    char data[256];
    sprintf(data, "%s%s%s", username, password, timestamp);
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, data, strlen(data));
    SHA256_Final(hash, &sha256);
    char* token = malloc(SHA256_DIGEST_LENGTH * 2 + 1);
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(token + (i * 2), "%02x", hash[i]);
    }
    return token;
}

// اختبار صلاحية التوكن: طول 64، فقط 0-9 و a-f
int validate_token_format(const char* token) {
    if (strlen(token) != SHA256_DIGEST_LENGTH * 2) return 0;
    for (int i = 0; i < SHA256_DIGEST_LENGTH * 2; i++) {
        char c = token[i];
        if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))) {
            return 0;
        }
    }
    return 1;
}

int main() {
    int pass = 0, fail = 0;

    char* token = generateToken("user", "password");
    printf("Generated token: %s\n", token);

    // Test 1: طول التوكن 64 حرف هيكس
    if (strlen(token) == SHA256_DIGEST_LENGTH * 2) {
        printf("Test 1: PASS (Token length correct)\n");
        pass++;
    } else {
        printf("Test 1: FAIL (Token length incorrect)\n");
        fail++;
    }

    // Test 2: كل الأحرف هي حروف hex صحيحة
    if (validate_token_format(token)) {
        printf("Test 2: PASS (Token hex format valid)\n");
        pass++;
    } else {
        printf("Test 2: FAIL (Token hex format invalid)\n");
        fail++;
    }

    free(token);

    printf("\nTotal Passed: %d\nTotal Failed: %d\n", pass, fail);
    return 0;
}
