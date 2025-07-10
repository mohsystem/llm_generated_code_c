#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

char* getUserInput(const char* prompt, const char* test_input) {
    // بدل قراءة من stdin، نستخدم test_input لاختبارات تلقائية
    if (test_input == NULL) {
        // قراءة حقيقية (لن تستخدم هنا في الاختبارات)
        char inputStr[256];
        while (true) {
            printf("%s", prompt);
            if (fgets(inputStr, 256, stdin) != NULL) {
                inputStr[strcspn(inputStr, "\n")] = 0; // remove the newline character
                return strdup(inputStr);
            } else {
                printf("Invalid input. Please try again.\n");
            }
        }
    } else {
        // محاكاة الإدخال مع test_input
        return strdup(test_input);
    }
}

int validateInput(const char* inputStr) {
    for (int i = 0; inputStr[i] != '\0'; i++) {
        if (!isalpha((unsigned char)inputStr[i])) {
            return 0; // غير صالح
        }
    }
    return 1; // صالح
}

void toUpperCase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void toLowerCase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void run_test(const char* input, const char* expected_validity) {
    char* inputStr = getUserInput("", input);

    printf("Input: '%s'\n", inputStr);

    int valid = validateInput(inputStr);
    if (valid) {
        printf("Validation: PASS\n");
    } else {
        printf("Validation: FAIL (invalid characters found)\n");
    }

    if ((valid && strcmp(expected_validity, "valid") == 0) || (!valid && strcmp(expected_validity, "invalid") == 0)) {
        printf("Validation result matches expected: PASS\n");
    } else {
        printf("Validation result matches expected: FAIL\n");
    }

    if (valid) {
        toUpperCase(inputStr);
        printf("Upper case: %s\n", inputStr);

        toLowerCase(inputStr);
        printf("Lower case: %s\n", inputStr);
    }

    printf("----------------------------------------------------\n");

    free(inputStr);
}

int main() {
    struct {
        const char* input;
        const char* expected_validity;
    } tests[] = {
        {"HelloWorld", "valid"},
        {"helloworld", "valid"},
        {"HELLOWORLD", "valid"},
        {"Hello World", "invalid"},  // يحتوي على فراغ => invalid
        {"Hello123", "invalid"},     // يحتوي على أرقام => invalid
        {"!@#$%", "invalid"},        // رموز غير صالحة
        {"", "valid"},               // فارغ - لا يحتوي على أحرف غير صالحة
    };

    int n = sizeof(tests) / sizeof(tests[0]);
    for (int i = 0; i < n; i++) {
        printf("Test %d:\n", i+1);
        run_test(tests[i].input, tests[i].expected_validity);
    }

    return 0;
}
