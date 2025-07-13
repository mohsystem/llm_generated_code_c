#include <stdio.h>
#include <string.h>

// دالة تحقق صارمة بدون I/O لاستخدامها في الاختبارات
int check_password_strict(const char* stored, const char* entered) {
    if (strlen(stored) != strlen(entered)) return 0;
    for (size_t i = 0; i < strlen(stored); i++) {
        if (stored[i] != entered[i]) return 0;
    }
    return 1;
}

// دالة تنفيذ اختبار وطباعة pass/fail
void run_test_case(const char* stored, const char* entered, int expected) {
    int result = check_password_strict(stored, entered);
    printf("Test case - stored: '%s', entered: '%s'\n", stored, entered);
    if (result == expected) {
        printf("Result: pass\n");
    } else {
        printf("Result: fail\n");
    }
}

int main() {
    FILE *file = fopen("password.txt", "r");
    if (!file) {
        printf("Password file not found.\n");
        return 1;
    }
    char storedPassword[100];
    fgets(storedPassword, 100, file);
    storedPassword[strcspn(storedPassword, "\n")] = 0;
    fclose(file);

    char userPassword[100];
    printf("Enter your password: ");
    fgets(userPassword, 100, stdin);
    userPassword[strcspn(userPassword, "\n")] = 0;

    if (strlen(storedPassword) != strlen(userPassword)) {
        printf("Password does not match\n");
    } else {
        int match = 1;
        for (int i = 0; i < (int)strlen(storedPassword); i++) {
            if (storedPassword[i] != userPassword[i]) {
                match = 0;
                break;
            }
        }
        if (match) {
            printf("Password matches\n");
        } else {
            printf("Password does not match\n");
        }
    }

    // اختبارات صارمة
    printf("\nRunning strict test cases:\n");
    run_test_case("secret123", "secret123", 1);
    run_test_case("secret123", "Secret123", 0);
    run_test_case("password", "password1", 0);
    run_test_case("123456", "123456", 1);
    run_test_case("", "", 1);
    run_test_case("abc", "abcd", 0);

    return 0;
}
