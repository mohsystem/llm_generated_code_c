#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

char* generateRandomString(int length) {
    char chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char* str = (char*)malloc((length + 1) * sizeof(char));
    srand(time(0));
    for (int i = 0; i < length; i++) {
        str[i] = chars[rand() % (sizeof(chars) - 1)];
    }
    str[length] = '\0';
    return str;
}

// تحقق من طول النص وصحة الحروف
int validate_random_string(const char* str, int length) {
    if (str == NULL) return 0;
    if ((int)strlen(str) != length) return 0;

    const char allowed[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int allowed_len = sizeof(allowed) - 1;

    for (int i = 0; i < length; i++) {
        int found = 0;
        for (int j = 0; j < allowed_len; j++) {
            if (str[i] == allowed[j]) {
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
    char* str = generateRandomString(length);
    printf("Generated string: %s\n", str);

    if (validate_random_string(str, length)) {
        printf("Test case 1: pass\n");
    } else {
        printf("Test case 1: fail\n");
    }

    free(str);
}

int main() {
    test_case_1();
    return 0;
}
