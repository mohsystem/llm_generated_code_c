#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generateRandomString(int length, char* result) {
    const char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    srand(time(0));

    for (int i = 0; i < length; ++i) {
        result[i] = characters[rand() % (sizeof(characters) - 1)];
    }
    result[length] = '\0';
}

int validate_random_string(const char* str, int length) {
    if (strlen(str) != length) return 0;

    const char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    int chars_len = sizeof(characters) - 1;

    for (int i = 0; i < length; i++) {
        int found = 0;
        for (int j = 0; j < chars_len; j++) {
            if (str[i] == characters[j]) {
                found = 1;
                break;
            }
        }
        if (!found) return 0;
    }
    return 1;
}

void test_case_1() {
    int length = 10;
    char result[length + 1];
    generateRandomString(length, result);
    printf("Generated string: %s\n", result);

    if (validate_random_string(result, length)) {
        printf("Test case 1: pass\n");
    } else {
        printf("Test case 1: fail\n");
    }
}

int main() {
    test_case_1();
    return 0;
}
