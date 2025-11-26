#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

char* generate_random_string(int length) {
    const char characters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char* result = malloc((length + 1) * sizeof(char));

    srand(time(NULL));

    for (int i = 0; i < length; i++) {
        int random_index = rand() % (sizeof(characters) - 1);
        result[i] = characters[random_index];
    }

    result[length] = '\0';
    return result;
}

// تحقق من أن النص مكون فقط من حروف ASCII كبيرة وصغيرة وطوله صحيح
int validate_random_string(const char* str, int length) {
    if (strlen(str) != length) return 0;
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
    char* random_string = generate_random_string(length);
    printf("Generated string: %s\n", random_string);
    if (validate_random_string(random_string, length)) {
        printf("Test case 1: pass\n");
    } else {
        printf("Test case 1: fail\n");
    }
    free(random_string);
}

int main() {
    test_case_1();
    return 0;
}
