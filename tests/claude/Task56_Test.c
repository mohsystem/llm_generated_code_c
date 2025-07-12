#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TOKEN_LENGTH 32

char* generate_token() {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    char* token = malloc(TOKEN_LENGTH + 1);

    srand(time(NULL));

    for (int i = 0; i < TOKEN_LENGTH; i++) {
        int index = rand() % (sizeof(charset) - 1);
        token[i] = charset[index];
    }

    token[TOKEN_LENGTH] = '\0';
    return token;
}

// دالة للتحقق من أن التوكن يتكون فقط من الأحرف والأرقام المسموح بها
int validate_token(const char* token) {
    const char* valid_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int len = strlen(token);
    if (len != TOKEN_LENGTH) return 0;

    for (int i = 0; i < len; i++) {
        if (strchr(valid_chars, token[i]) == NULL) {
            return 0;
        }
    }
    return 1;
}

int main() {
    int pass = 0, fail = 0;

    char* token = generate_token();
    printf("Generated authentication token: %s\n", token);

    // Test 1: length check
    if (strlen(token) == TOKEN_LENGTH) {
        printf("Test 1: PASS (Correct token length)\n");
        pass++;
    } else {
        printf("Test 1: FAIL (Incorrect token length)\n");
        fail++;
    }

    // Test 2: characters validity
    if (validate_token(token)) {
        printf("Test 2: PASS (Valid characters in token)\n");
        pass++;
    } else {
        printf("Test 2: FAIL (Invalid characters in token)\n");
        fail++;
    }

    free(token);

    printf("\nTotal Passed: %d\nTotal Failed: %d\n", pass, fail);

    return 0;
}
