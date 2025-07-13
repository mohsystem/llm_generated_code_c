#include <stdio.h>
#include <string.h>

#define MAX_PASSWORD_LEN 100

int checkPassword(const char* storedPassword) {
    char enteredPassword[MAX_PASSWORD_LEN];
    printf("Enter password: ");
    fgets(enteredPassword, MAX_PASSWORD_LEN, stdin);
    enteredPassword[strcspn(enteredPassword, "\n")] = 0; // إزالة نهاية السطر

    if (strlen(enteredPassword) != strlen(storedPassword)) {
        return 0;
    }
    for (size_t i = 0; i < strlen(storedPassword); ++i) {
        if (storedPassword[i] != enteredPassword[i]) {
            return 0;
        }
    }
    return 1;
}

// دالة اختبار صارم بدون تداخل I/O
int checkPasswordStrict(const char* storedPassword, const char* enteredPassword) {
    if (strlen(enteredPassword) != strlen(storedPassword)) {
        return 0;
    }
    for (size_t i = 0; i < strlen(storedPassword); ++i) {
        if (storedPassword[i] != enteredPassword[i]) {
            return 0;
        }
    }
    return 1;
}

// تنفيذ حالات اختبار صارمة وطباعة pass/fail
void run_test_case(const char* stored, const char* entered, int expected) {
    int result = checkPasswordStrict(stored, entered);
    printf("Test case - stored: '%s', entered: '%s'\n", stored, entered);
    if (result == expected) {
        printf("Result: pass\n");
    } else {
        printf("Result: fail\n");
    }
}

int main() {
    // قراءة كلمة السر من الملف كما في الكود الأصلي
    FILE* file = fopen("password.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Password file not found. Please create 'password.txt'.\n");
        return 1;
    }

    char storedPassword[MAX_PASSWORD_LEN];
    fgets(storedPassword, MAX_PASSWORD_LEN, file);
    storedPassword[strcspn(storedPassword, "\n")] = 0;
    fclose(file);

    if (checkPassword(storedPassword)) {
        printf("Passwords match.\n");
    } else {
        printf("Passwords do not match.\n");
    }

    printf("\nRunning strict test cases:\n");
    run_test_case("secret123", "secret123", 1);
    run_test_case("secret123", "Secret123", 0);
    run_test_case("password", "password1", 0);
    run_test_case("123456", "123456", 1);
    run_test_case("", "", 1);
    run_test_case("abc", "abcd", 0);

    return 0;
}
