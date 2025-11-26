#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <openssl/sha.h>

#define MAX_USERS 100
#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50

struct User {
    char username[MAX_USERNAME_LENGTH];
    unsigned char password_hash[SHA256_DIGEST_LENGTH];
};

struct User users[MAX_USERS];
int user_count = 0;

void sha256(const char *string, unsigned char hash[SHA256_DIGEST_LENGTH]) {
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, string, strlen(string));
    SHA256_Final(hash, &sha256);
}

bool register_user(const char *username, const char *password) {
    if (user_count >= MAX_USERS) {
        return false;
    }
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return false;
        }
    }
    strcpy(users[user_count].username, username);
    sha256(password, users[user_count].password_hash);
    user_count++;
    return true;
}

bool login(const char *username, const char *password) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            unsigned char input_hash[SHA256_DIGEST_LENGTH];
            sha256(password, input_hash);
            return memcmp(users[i].password_hash, input_hash, SHA256_DIGEST_LENGTH) == 0;
        }
    }
    return false;
}

void run_test_cases() {
    printf("Running test cases...\n");

    // Test case 1: Register new user "alice" with password "password123"
    bool result = register_user("alice", "password123");
    printf("Register alice: %s\n", result ? "PASS" : "FAIL");

    // Test case 2: Register existing user "alice" again (should fail)
    result = register_user("alice", "newpass");
    printf("Register duplicate alice: %s\n", !result ? "PASS" : "FAIL");

    // Test case 3: Login with correct credentials for alice
    result = login("alice", "password123");
    printf("Login alice with correct password: %s\n", result ? "PASS" : "FAIL");

    // Test case 4: Login with incorrect password for alice
    result = login("alice", "wrongpassword");
    printf("Login alice with wrong password: %s\n", !result ? "PASS" : "FAIL");

    // Test case 5: Login with non-existent user
    result = login("bob", "password123");
    printf("Login non-existent user bob: %s\n", !result ? "PASS" : "FAIL");

    // Test case 6: Register new user "bob" with password "mysecurepass"
    result = register_user("bob", "mysecurepass");
    printf("Register bob: %s\n", result ? "PASS" : "FAIL");

    // Test case 7: Login bob with correct password
    result = login("bob", "mysecurepass");
    printf("Login bob with correct password: %s\n", result ? "PASS" : "FAIL");
}

int main() {
    run_test_cases();
    return 0;
}
