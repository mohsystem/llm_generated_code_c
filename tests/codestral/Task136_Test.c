#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_LENGTH 50

typedef struct {
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
} User;

typedef struct {
    User users[MAX_USERS];
    int count;
} Authentication;

void registerUser(Authentication* auth, const char* username, const char* password) {
    // Check if username exists
    for (int i = 0; i < auth->count; i++) {
        if (strcmp(auth->users[i].username, username) == 0) {
            printf("Registration failed: Username already exists.\n");
            return;
        }
    }
    if (auth->count < MAX_USERS) {
        strcpy(auth->users[auth->count].username, username);
        strcpy(auth->users[auth->count].password, password);
        auth->count++;
        printf("User registered successfully.\n");
    } else {
        printf("Maximum number of users reached.\n");
    }
}

void login(Authentication* auth, const char* username, const char* password) {
    for (int i = 0; i < auth->count; i++) {
        if (strcmp(auth->users[i].username, username) == 0 && strcmp(auth->users[i].password, password) == 0) {
            printf("Login successful.\n");
            return;
        }
    }
    printf("Invalid username or password.\n");
}

void test_case_register(Authentication* auth, const char* username, const char* password, int expect_success) {
    printf("Test register '%s': ", username);
    int prev_count = auth->count;
    registerUser(auth, username, password);
    int success = (auth->count > prev_count);
    printf("%s\n", (success == expect_success) ? "PASS" : "FAIL");
}

void test_case_login(Authentication* auth, const char* username, const char* password, int expect_success) {
    printf("Test login '%s' with password '%s': ", username, password);
    int found = 0;
    for (int i = 0; i < auth->count; i++) {
        if (strcmp(auth->users[i].username, username) == 0 && strcmp(auth->users[i].password, password) == 0) {
            found = 1;
            break;
        }
    }
    printf("%s\n", (found == expect_success) ? "PASS" : "FAIL");
}

int main() {
    Authentication auth;
    auth.count = 0;

    // Same exact test cases as the basic example plus some added edge cases

    // Register user1
    test_case_register(&auth, "user1", "password1", 1);
    // Try registering user1 again (should fail)
    test_case_register(&auth, "user1", "newpassword", 0);

    // Login user1 with correct password
    test_case_login(&auth, "user1", "password1", 1);
    // Login user1 with wrong password
    test_case_login(&auth, "user1", "wrongpassword", 0);
    // Login non-existent user
    test_case_login(&auth, "user2", "password1", 0);

    // Register user2
    test_case_register(&auth, "user2", "password2", 1);
    // Login user2 with correct password
    test_case_login(&auth, "user2", "password2", 1);

    return 0;
}
