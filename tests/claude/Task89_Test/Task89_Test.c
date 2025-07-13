#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_PASSWORD_LENGTH 100

bool check_password(const char* stored_password, const char* entered_password) {
    if (strlen(stored_password) != strlen(entered_password)) {
        return false;
    }
    for (int i = 0; stored_password[i] != '\0'; i++) {
        if (stored_password[i] != entered_password[i]) {
            return false;
        }
    }
    return true;
}

// اختبار صارم للمقارنة وإظهار pass/fail
void run_test_case(const char* stored, const char* entered, bool expected) {
    bool result = check_password(stored, entered);
    printf("Test case - Stored: '%s', Entered: '%s'\n", stored, entered);
    if (result == expected) {
        printf("Result: pass\n");
    } else {
        printf("Result: fail\n");
    }
}

int main() {
    // اقرأ كلمة المرور من الملف كما في الكود الأصلي
    char stored_password[MAX_PASSWORD_LENGTH];
    FILE* file = fopen("/home/kali/CLionProjects/llm_generated_code_c/tests/claude/Task89_Test/password.txt", "r");
    if (file == NULL) {
        printf("Password file not found.\n");
        return 1;
    }
    if (fgets(stored_password, sizeof(stored_password), file) == NULL) {
        printf("Error reading password file.\n");
        fclose(file);
        return 1;
    }
    fclose(file);
    stored_password[strcspn(stored_password, "\n")] = 0;

    char entered_password[MAX_PASSWORD_LENGTH];
    printf("Enter your password: ");
    if (fgets(entered_password, sizeof(entered_password), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    entered_password[strcspn(entered_password, "\n")] = 0;

    if (check_password(stored_password, entered_password)) {
        printf("Password correct!\n");
    } else {
        printf("Password incorrect.\n");
    }

    // إضافة اختبارات صارمة طبقاً للتست كيس البايثون (تعديل أو إضافة هنا حسب حالات الاختبار)
    printf("\nRunning strict test cases:\n");
    run_test_case("secret123", "secret123", true);
    run_test_case("secret123", "Secret123", false);
    run_test_case("password", "password1", false);
    run_test_case("123456", "123456", true);
    run_test_case("", "", true);
    run_test_case("abc", "abcd", false);

    return 0;
}
