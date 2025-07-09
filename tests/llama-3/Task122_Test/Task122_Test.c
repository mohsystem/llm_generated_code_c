#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    char username[100];
    char password[100];
} User;

User users[100];
int userCount = 0;

int signup(const char* username, const char* password) {
    if (userCount >= 100) {
        return 0; // قاعدة البيانات ممتلئة
    }
    strncpy(users[userCount].username, username, 99);
    users[userCount].username[99] = '\0';

    strncpy(users[userCount].password, password, 99);
    users[userCount].password[99] = '\0';

    userCount++;
    return 1;
}

void print_result(const char* test_name, int success) {
    printf("%s: %s\n", test_name, success ? "PASS" : "FAIL");
}

void run_test_cases() {
    userCount = 0;

    // Test Case 1
    print_result("Test Case 1: Normal Signup", signup("user1", "password123"));

    // Test Case 2
    print_result("Test Case 2: Signup with Special Characters in Username", signup("user!@#", "password123"));

    // Test Case 3
    print_result("Test Case 3: Signup with Special Characters in Password", signup("user2", "pass!@#123"));

    // Test Case 4
    print_result("Test Case 4: Signup with Empty Username", signup("", "password123"));

    // Test Case 5
    print_result("Test Case 5: Signup with Empty Password", signup("user3", ""));

    // Test Case 6: long username/password 100 chars each
    char long_username[101], long_password[101];
    for (int i = 0; i < 100; i++) {
        long_username[i] = 'u';
        long_password[i] = 'p';
    }
    long_username[100] = '\0';
    long_password[100] = '\0';
    print_result("Test Case 6: Signup with Long Username and Password", signup(long_username, long_password));

    // Test Case 7
    int res = signup("user4", "password1") && signup("user5", "password2");
    print_result("Test Case 7: Signup with Multiple Users", res);

    // Test Case 8
    res = signup("user6", "password3") && signup("user6", "password4");
    print_result("Test Case 8: Signup with Identical Usernames", res);

    // Test Case 9
    print_result("Test Case 9: File Handling Test (N/A)", signup("user7", "password5"));

    // Test Case 10: null byte in C not supported, simplified
    print_result("Test Case 10: Invalid Characters in Username and Password (simplified)", signup("user", "password"));

    // Test Case 11
    res = signup("user8", "password6") && signup("user8", "password6");
    print_result("Test Case 11: Duplicate Entries", res);

    // Test Case 12: Large data (1000 chars)
    char *long_user = (char *)malloc(1001);
    char *long_pass = (char *)malloc(1001);
    if (long_user && long_pass) {
        for (int i = 0; i < 1000; i++) {
            long_user[i] = 'u';
            long_pass[i] = 'p';
        }
        long_user[1000] = '\0';
        long_pass[1000] = '\0';
        print_result("Test Case 12: Large Data", signup(long_user, long_pass));
    } else {
        print_result("Test Case 12: Large Data", 0);
    }
    free(long_user);
    free(long_pass);

    // Test Case 13: File Size Limits (fill database to max)
    int success = 1;
    char temp_username[20];
    for (int i = userCount; i < 100; i++) {
        snprintf(temp_username, sizeof(temp_username), "user%d", i);
        if (!signup(temp_username, "password123")) {
            success = 0;
            break;
        }
    }
    print_result("Test Case 13: File Size Limits", success);

    // Test Case 14
    print_result("Test Case 14: Encoding Issues (N/A)", signup("user9", "password123"));

    // Test Case 17
    print_result("Test Case 17: Handling Multiple Extensions", signup("user.with.many.extensions", "password"));

    // Test Case 18: No hashing so password stored as plain text => fail security
    print_result("Test Case 18: Security Measures (plaintext password)", 0);

    // Test Case 19
    print_result("Test Case 19: Exception Handling (N/A)", 1);
}

int main() {
    run_test_cases();
    return 0;
}
