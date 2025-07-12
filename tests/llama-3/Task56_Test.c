#include <stdio.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>
#include <stdlib.h>

char* generateToken(const char* username, const char* password) {
    time_t timestamp = time(NULL);
    char tokenStr[256];
    sprintf(tokenStr, "%s%s%ld", username, password, timestamp);
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)tokenStr, strlen(tokenStr), hash);
    char tokenHash[SHA256_DIGEST_LENGTH * 2 + 1];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(tokenHash + i * 2, "%02x", hash[i]);
    }
    return strdup(tokenHash);
}

// تحقق الطول (64) وصلاحية الأحرف (0-9,a-f)
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

    // Test 1: الطول 64
    if (strlen(token) == SHA256_DIGEST_LENGTH * 2) {
        printf("Test 1: PASS (Correct token length)\n");
        pass++;
    } else {
        printf("Test 1: FAIL (Incorrect token length)\n");
        fail++;
    }

    // Test 2: الحروف هيكس صحيحة
    if (validate_token_format(token)) {
        printf("Test 2: PASS (Valid hex characters)\n");
        pass++;
    } else {
        printf("Test 2: FAIL (Invalid hex characters)\n");
        fail++;
    }

    free(token);

    printf("\nTotal Passed: %d\nTotal Failed: %d\n", pass, fail);
    return 0;
}
