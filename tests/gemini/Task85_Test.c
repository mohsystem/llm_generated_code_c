#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char *generateRandomString(int length) {
    char *str = malloc(sizeof(char) * (length + 1));
    if (str == NULL) {
        return NULL;
    }

    srand(time(NULL));
    for (int i = 0; i < length; i++) {
        str[i] = (rand() % 26) + ((rand() % 2) ? 'a' : 'A');
    }
    str[length] = '\0';

    return str;
}

// دالة تحقق طول النص وصحة الحروف
int validate_random_string(const char *str, int length) {
    if (str == NULL) return 0;
    if ((int)strlen(str) != length) return 0;
    for (int i = 0; i < length; i++) {
        char c = str[i];
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) {
            return 0;
        }
    }
    return 1;
}

void test_case_1() {
    int length = 10;
    char *random_str = generateRandomString(length);
    if (!random_str) {
        printf("Test case 1: fail (malloc returned NULL)\n");
        return;
    }
    printf("Generated string: %s\n", random_str);

    if (validate_random_string(random_str, length)) {
        printf("Test case 1: pass\n");
    } else {
        printf("Test case 1: fail\n");
    }
    free(random_str);
}

int main() {
    test_case_1();
    return 0;
}
