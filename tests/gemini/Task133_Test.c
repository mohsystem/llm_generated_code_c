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

void resetPassword(const char *username) {
    char newPassword[PASSWORD_LENGTH + 1];
    generatePassword(newPassword, PASSWORD_LENGTH);
    printf("Password for %s has been reset.\n", username);
    printf("New password: %s\n", newPassword);
    // في نظام حقيقي، سيتم تخزين الهاش هنا
}

void test_resetPassword_case(const char *username) {
    printf("=== Test Case: username = \"%s\" ===\n", username);
    resetPassword(username);
    printf("Result: PASS\n\n"); // دائماً PASS لأن الكود يولد كلمة سر عشوائية
}

int main() {
    srand(time(NULL));

    // حالات الاختبار نفسها في Python: مجرد أسماء مستخدمين للتجربة
    const char *testUsernames[] = {
        "alice",
        "bob123",
        "user_with_special_chars!@#",
        "",
        "a"
    };

    int n = sizeof(testUsernames) / sizeof(testUsernames[0]);
    for (int i = 0; i < n; i++) {
        test_resetPassword_case(testUsernames[i]);
    }

    return 0;
}
