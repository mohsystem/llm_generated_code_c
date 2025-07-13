#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void generateRandomString(int length, char* result) {
    const char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    int lettersLength = strlen(letters);

    srand(time(0));

    for (int i = 0; i < length; i++) {
        int index = rand() % lettersLength;
        result[i] = letters[index];
    }

    result[length] = '\0';
}

// تحقق من صحة النص: طول صحيح وحروف فقط من الحروف المسموح بها
int validate_random_string(const char* str, int length) {
    if (strlen(str) != length) return 0;

    const char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    int lettersLength = strlen(letters);

    for (int i = 0; i < length; i++) {
        int found = 0;
        for (int j = 0; j < lettersLength; j++) {
            if (str[i] == letters[j]) {
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
