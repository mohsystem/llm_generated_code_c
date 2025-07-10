#include <stdio.h>
#include <string.h>

void test_case(const char *username, const char *oldPassword, const char *newPassword, const char *confirmPassword) {
    printf("=== Test Case ===\n");
    printf("Username: %s\n", username);
    printf("Old Password: %s\n", oldPassword);
    printf("New Password: %s\n", newPassword);
    printf("Confirm Password: %s\n", confirmPassword);

    if (strcmp(newPassword, confirmPassword) == 0) {
        printf("Result: PASS (Password reset successfully)\n");
    } else {
        printf("Result: FAIL (New password and confirm password do not match)\n");
    }
    printf("\n");
}

int main() {
    // نفس الحالات بالضبط (username, oldPassword, newPassword, confirmPassword)
    struct {
        const char *username;
        const char *oldPassword;
        const char *newPassword;
        const char *confirmPassword;
    } testCases[] = {
        {"alice", "oldPass1", "newPass123", "newPass123"},         // تطابق => PASS
        {"bob", "oldPass2", "password", "password1"},             // لا تطابق => FAIL
        {"charlie", "oldPass3", "MyNewPass!", "MyNewPass!"},      // تطابق => PASS
        {"dave", "oldPass4", "12345678", "1234567"},              // لا تطابق => FAIL
        {"eve", "oldPass5", "Password!", "Password!"}             // تطابق => PASS
    };

    int n = sizeof(testCases) / sizeof(testCases[0]);
    for (int i = 0; i < n; i++) {
        test_case(testCases[i].username, testCases[i].oldPassword, testCases[i].newPassword, testCases[i].confirmPassword);
    }

    return 0;
}
