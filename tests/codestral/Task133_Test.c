#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PASSWORD_LENGTH 12
#define MAX_USERNAME_LENGTH 50

const char CHAR_SET[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+-=[]{}|;:,.<>?";

void generatePassword(char *password, int length) {
    int charSetLength = strlen(CHAR_SET);
    for (int i = 0; i < length; i++) {
        password[i] = CHAR_SET[rand() % charSetLength];
    }
    password[length] = '\0';
}

void resetPassword(const char *username, char *outPassword) {
    generatePassword(outPassword, PASSWORD_LENGTH);
    printf("Password for %s has been reset.\n", username);
    printf("New password: %s\n", outPassword);
    // في نظام حقيقي، سيتم تخزين الهاش هنا
}

void test_resetPassword(const char *testUsername) {
    printf("=== Test Case: username = \"%s\" ===\n", testUsername);
    char generatedPassword[PASSWORD_LENGTH + 1];
    resetPassword(testUsername, generatedPassword);

    int pass = (strlen(generatedPassword) == PASSWORD_LENGTH);
    for (int i = 0; i < PASSWORD_LENGTH && pass; i++) {
        if (strchr(CHAR_SET, generatedPassword[i]) == NULL) {
            pass = 0;
        }
    }

    if (pass) {
        printf("Result: PASS\n");
    } else {
        printf("Result: FAIL\n");
    }
    printf("\n");
}

int main() {
    srand(time(NULL));

    // الحالات مكتوبة هنا داخل main
    const char *testCases[] = {
        "alice",
        "bob123",
        "user_with_special_chars!@#",
        "",
        "a"
    };
    int numCases = sizeof(testCases) / sizeof(testCases[0]);

    for (int i = 0; i < numCases; i++) {
        test_resetPassword(testCases[i]);
    }

    return 0;
}
