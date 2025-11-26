#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// دالة تحقق كلمة السر كما في كودك
bool checkPassword(const char *inputPassword) {
    FILE *file = fopen("password.txt", "r");
    if (file == NULL) return false;

    char storedPassword[100];
    fgets(storedPassword, 100, file);
    fclose(file);

    size_t lenStored = strlen(storedPassword);
    size_t lenInput = strlen(inputPassword);

    if (storedPassword[lenStored - 1] == '\n')
        storedPassword[lenStored - 1] = '\0';

    if (lenStored != lenInput + 1) return false;

    for (size_t i = 0; i < lenInput; ++i) {
        if (storedPassword[i] != inputPassword[i]) {
            return false;
        }
    }
    return true;
}

// دالة اختبار صارم بدون I/O
bool checkPasswordStrict(const char* storedPassword, const char* inputPassword) {
    size_t lenStored = strlen(storedPassword);
    size_t lenInput = strlen(inputPassword);

    if (lenStored != lenInput) return false;

    for (size_t i = 0; i < lenInput; ++i) {
        if (storedPassword[i] != inputPassword[i]) {
            return false;
        }
    }
    return true;
}

// دالة تنفيذ test case صارمة
void run_test_case(const char* stored, const char* entered, bool expected) {
    bool result = checkPasswordStrict(stored, entered);
    printf("Test case - stored: '%s', entered: '%s'\n", stored, entered);
    if (result == expected) {
        printf("Result: pass\n");
    } else {
        printf("Result: fail\n");
    }
}

int main() {
    // كتابة كلمة السر في الملف (كما في كودك)
    const char password[] = "correctpassword";
    FILE *file = fopen("password.txt", "w");
    fputs(password, file);
    fclose(file);

    char userInput[100];
    printf("Enter your password: ");
    scanf("%99s", userInput);

    if (checkPassword(userInput)) {
        printf("Password is correct\n");
    } else {
        printf("Password is incorrect\n");
    }

    // اختبارات صارمة
    printf("\nRunning strict test cases:\n");
    run_test_case("correctpassword", "correctpassword", true);
    run_test_case("correctpassword", "Correctpassword", false);
    run_test_case("password", "password1", false);
    run_test_case("123456", "123456", true);
    run_test_case("", "", true);
    run_test_case("abc", "abcd", false);

    return 0;
}
