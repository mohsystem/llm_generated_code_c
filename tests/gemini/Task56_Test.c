#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char* generate_token(size_t length) {
    const char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    char* token = malloc((length + 1) * sizeof(char));
    if (token == NULL) {
        return NULL;
    }

    srand(time(NULL));
    for (size_t i = 0; i < length; ++i) {
        token[i] = characters[rand() % (sizeof(characters) - 1)];
    }
    token[length] = '\0';

    return token;
}

int validate_token(const char* token, size_t expected_length) {
    const char* valid_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    if (strlen(token) != expected_length) return 0;

    for (size_t i = 0; i < expected_length; i++) {
        if (strchr(valid_chars, token[i]) == NULL) {
            return 0;
        }
    }
    return 1;
}

int main() {
    int pass = 0, fail = 0;
    size_t token_length = 32;

    char* token = generate_token(token_length);
    if (token == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    printf("Generated token: %s\n", token);

    // Test 1: Length check
    if (strlen(token) == token_length) {
        printf("Test 1: PASS (Correct token length)\n");
        pass++;
    } else {
        printf("Test 1: FAIL (Incorrect token length)\n");
        fail++;
    }

    // Test 2: Valid characters check
    if (validate_token(token, token_length)) {
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
