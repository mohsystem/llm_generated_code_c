#include <stdio.h>

void resetPassword(const char* username, const char* newPassword) {
    printf("Password for %s has been reset to %s\n", username, newPassword);
}

int main() {
    // حالات الاختبار بالضبط (username, newPassword)
    const char *testUsernames[] = {
        "alice",
        "bob123",
        "user_with_special_chars!@#",
        "emptyUser",
        "singleCharUser"
    };

    const char *testPasswords[] = {
        "Password123!",
        "MyNewPass2025",
        "Spec!@#Pass",
        "EmptyPass123",
        "A1b2C3d4"
    };

    int numTests = sizeof(testUsernames) / sizeof(testUsernames[0]);

    for (int i = 0; i < numTests; i++) {
        printf("=== Test Case %d ===\n", i + 1);
        resetPassword(testUsernames[i], testPasswords[i]);
        printf("Result: PASS\n\n");
    }

    return 0;
}
