#include <stdio.h>
#include <string.h>

// دالة مقارنة كلمات السر من ملف ومستخدم (كما في كودك)
int comparePasswords(const char* filePath, const char* userPassword) {
    FILE* file = fopen(filePath, "r");
    char storedPassword[100];

    if (file != NULL) {
        if (fgets(storedPassword, sizeof(storedPassword), file) != NULL) {
            storedPassword[strcspn(storedPassword, "\n")] = '\0';
            fclose(file);
            return strcmp(userPassword, storedPassword) == 0;
        } else {
            printf("Error reading the file.\n");
            fclose(file);
            return 0;
        }
    } else {
        printf("Error opening the file.\n");
        return 0;
    }
}

// دالة اختبار صارم لحالات المقارنة
void run_test_case(const char* stored_pass, const char* entered_pass, int expected) {
    // نكتب كلمة السر المخزنة مؤقتاً في ملف password.txt
    FILE* file = fopen("password.txt", "w");
    if (!file) {
        printf("Test case setup fail: cannot write password file.\n");
        return;
    }
    fprintf(file, "%s\n", stored_pass);
    fclose(file);

    int result = comparePasswords("password.txt", entered_pass);

    printf("Test case - stored: '%s', entered: '%s'\n", stored_pass, entered_pass);
    if (result == expected) {
        printf("Result: pass\n");
    } else {
        printf("Result: fail\n");
    }
}

int main() {
    char userPassword[100];

    printf("Enter your password: ");
    if (fgets(userPassword, sizeof(userPassword), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    userPassword[strcspn(userPassword, "\n")] = '\0';

    if (comparePasswords("password.txt", userPassword)) {
        printf("Password is correct.\n");
    } else {
        printf("Password is incorrect.\n");
    }

    // اختبارات صارمة طبقاً للتست كيس البايثون
    printf("\nRunning strict test cases:\n");
    run_test_case("secret123", "secret123", 1);
    run_test_case("secret123", "Secret123", 0);
    run_test_case("password", "password1", 0);
    run_test_case("123456", "123456", 1);
    run_test_case("", "", 1);
    run_test_case("abc", "abcd", 0);

    return 0;
}
