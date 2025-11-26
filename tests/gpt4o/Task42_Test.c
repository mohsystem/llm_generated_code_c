#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>

#define MAX_USERS 100
#define USERNAME_SIZE 50
#define PASSWORD_HASH_SIZE 65

typedef struct {
    char username[USERNAME_SIZE];
    char password_hash[PASSWORD_HASH_SIZE];
} User;

User users[MAX_USERS];
int user_count = 0;

void hash_password(const char *password, char *output_buffer) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, password, strlen(password));
    SHA256_Final(hash, &sha256);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(output_buffer + (i * 2), "%02x", hash[i]);
    }
    output_buffer[64] = 0;
}

int register_user(const char *username, const char *password) {
    if (user_count >= MAX_USERS)
        return 0; // max users reached
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return 0; // user exists
        }
    }
    strcpy(users[user_count].username, username);
    hash_password(password, users[user_count].password_hash);
    user_count++;
    return 1;
}

int authenticate(const char *username, const char *password) {
    char hashed_password[PASSWORD_HASH_SIZE];
    hash_password(password, hashed_password);
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0 &&
            strcmp(users[i].password_hash, hashed_password) == 0) {
            return 1; // success
        }
    }
    return 0; // fail
}

void run_tests() {
    int pass = 0, fail = 0;

    // Test 1: register new user "bob"
    if(register_user("bob", "securepassword") && user_count > 0) {
        printf("Test 1: PASS\n"); pass++;
    } else {
        printf("Test 1: FAIL\n"); fail++;
    }

    // Test 2: register existing user "alice"
    register_user("alice", "password123");
    if(!register_user("alice", "newpassword")) {
        printf("Test 2: PASS\n"); pass++;
    } else {
        printf("Test 2: FAIL\n"); fail++;
    }

    // Test 3: successful login for charlie
    register_user("charlie", "mypassword");
    if(authenticate("charlie", "mypassword")) {
        printf("Test 3: PASS\n"); pass++;
    } else {
        printf("Test 3: FAIL\n"); fail++;
    }

    // Test 4: login failure wrong password for david
    register_user("david", "password");
    if(!authenticate("david", "wrongpassword")) {
        printf("Test 4: PASS\n"); pass++;
    } else {
        printf("Test 4: FAIL\n"); fail++;
    }

    // Test 5: login failure non-existent user
    if(!authenticate("nonexistent", "password")) {
        printf("Test 5: PASS\n"); pass++;
    } else {
        printf("Test 5: FAIL\n"); fail++;
    }

    // Test 6: register multiple users eve, frank and login success
    register_user("eve", "pass1");
    register_user("frank", "pass2");
    if(authenticate("eve", "pass1") && authenticate("frank", "pass2")) {
        printf("Test 6: PASS\n"); pass++;
    } else {
        printf("Test 6: FAIL\n"); fail++;
    }

    // Test 7: hash consistency (login success)
    register_user("grace", "password");
    if(authenticate("grace", "password")) {
        printf("Test 7: PASS\n"); pass++;
    } else {
        printf("Test 7: FAIL\n"); fail++;
    }

    // Test 8: altered stored password hash (simulate tampering)
    for(int i=0; i < user_count; i++) {
        if(strcmp(users[i].username, "grace") == 0) {
            strcpy(users[i].password_hash, "invalidhash");
        }
    }
    if(!authenticate("grace", "password")) {
        printf("Test 8: PASS\n"); pass++;
    } else {
        printf("Test 8: FAIL\n"); fail++;
    }

    // Test 9: register/login with empty password
    if(register_user("hank", "") && authenticate("hank", "") && !authenticate("hank", "wrongpassword")) {
        printf("Test 9: PASS\n"); pass++;
    } else {
        printf("Test 9: FAIL\n"); fail++;
    }

    // Test 10: register/login with very long password
    char long_pass[1001];
    for(int i = 0; i < 1000; i++) long_pass[i] = 'a';
    long_pass[1000] = '\0';

    if(register_user("ida", long_pass) && authenticate("ida", long_pass) && !authenticate("ida", "shortpassword")) {
        printf("Test 10: PASS\n"); pass++;
    } else {
        printf("Test 10: FAIL\n"); fail++;
    }

    printf("Total Passed: %d, Failed: %d\n", pass, fail);
}

int main() {
    run_tests();
    return 0;
}
