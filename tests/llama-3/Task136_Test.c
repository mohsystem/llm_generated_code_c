#include <stdio.h>
#include <string.h>

int authenticate(const char users[][2][20], int numUsers, const char* username, const char* password) {
    for (int i = 0; i < numUsers; i++) {
        if (strcmp(users[i][0], username) == 0 && strcmp(users[i][1], password) == 0) {
            return 1; // مصادقة ناجحة
        }
    }
    return 0; // فشل
}

void run_tests() {
    char users[][2][20] = {{"admin", "password"}, {"user", "password"}};
    int numUsers = sizeof(users) / sizeof(users[0]);

    printf("Test 1: Valid admin/password\n");
    if (authenticate(users, numUsers, "admin", "password")) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

    printf("Test 2: Valid user/password\n");
    if (authenticate(users, numUsers, "user", "password")) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

    printf("Test 3: Invalid username\n");
    if (!authenticate(users, numUsers, "invalid", "password")) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

    printf("Test 4: Valid username, wrong password\n");
    if (!authenticate(users, numUsers, "admin", "wrongpass")) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

    printf("Test 5: Empty username and password\n");
    if (!authenticate(users, numUsers, "", "")) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }
}

int main() {
    char users[][2][20] = {{"admin", "password"}, {"user", "password"}};
    int numUsers = sizeof(users) / sizeof(users[0]);

    run_tests();

    char username[20], password[20];
    printf("Enter username: ");
    scanf("%19s", username);
    printf("Enter password: ");
    scanf("%19s", password);

    if (authenticate(users, numUsers, username, password)) {
        printf("Authentication successful!\n");
    } else {
        printf("Invalid username or password\n");
    }
    return 0;
}
