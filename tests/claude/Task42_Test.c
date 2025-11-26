#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include <openssl/rand.h>

#define MAX_USERS 100
#define SALT_LENGTH 32
#define HASH_LENGTH (SALT_LENGTH + SHA256_DIGEST_LENGTH)

struct User {
    char username[50];
    unsigned char password_hash[HASH_LENGTH];
};

struct User users[MAX_USERS];
int user_count = 0;

void hash_password(const char* password, const unsigned char* salt, unsigned char* result) {
    memcpy(result, salt, SALT_LENGTH);
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, salt, SALT_LENGTH);
    SHA256_Update(&sha256, password, strlen(password));
    SHA256_Final(result + SALT_LENGTH, &sha256);
}

int verify_password(const unsigned char* stored_hash, const char* provided_password) {
    unsigned char computed_hash[HASH_LENGTH];
    hash_password(provided_password, stored_hash, computed_hash);
    return memcmp(stored_hash, computed_hash, HASH_LENGTH) == 0;
}

int register_user(const char* username, const char* password) {
    if (user_count >= MAX_USERS) {
        return 0;
    }
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return 0;
        }
    }
    strcpy(users[user_count].username, username);
    unsigned char salt[SALT_LENGTH];
    RAND_bytes(salt, SALT_LENGTH);
    hash_password(password, salt, users[user_count].password_hash);
    user_count++;
    return 1;
}

int login(const char* username, const char* password) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return verify_password(users[i].password_hash, password);
        }
    }
    return 0;
}

// دالة طباعة hex (لمساعدتك في طباعة الـ hashes لو احتجت)
void print_hex(const unsigned char* data, size_t len) {
    for(size_t i=0; i<len; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}

// دالة الاختبارات
void run_tests() {
    int pass_count = 0, fail_count = 0;

    // Test 1: register new user
    if(register_user("bob", "securepassword") && strcmp(users[user_count-1].username, "bob") == 0) {
        printf("Test 1: PASS\n"); pass_count++;
    } else {
        printf("Test 1: FAIL\n"); fail_count++;
    }

    // Test 2: register existing user
    register_user("alice", "password123");
    if(!register_user("alice", "newpassword")) {
        printf("Test 2: PASS\n"); pass_count++;
    } else {
        printf("Test 2: FAIL\n"); fail_count++;
    }

    // Test 3: login successful
    register_user("charlie", "mypassword");
    if(login("charlie", "mypassword")) {
        printf("Test 3: PASS\n"); pass_count++;
    } else {
        printf("Test 3: FAIL\n"); fail_count++;
    }

    // Test 4: login failure incorrect password
    register_user("david", "password");
    if(!login("david", "wrongpassword")) {
        printf("Test 4: PASS\n"); pass_count++;
    } else {
        printf("Test 4: FAIL\n"); fail_count++;
    }

    // Test 5: login failure non-existent user
    if(!login("nonexistent", "password")) {
        printf("Test 5: PASS\n"); pass_count++;
    } else {
        printf("Test 5: FAIL\n"); fail_count++;
    }

    // Test 6: multiple users register & login
    register_user("eve", "pass1");
    register_user("frank", "pass2");
    if(login("eve", "pass1") && login("frank", "pass2")) {
        printf("Test 6: PASS\n"); pass_count++;
    } else {
        printf("Test 6: FAIL\n"); fail_count++;
    }

    // Test 7: hash consistency - different salts + verify
    unsigned char hash1[HASH_LENGTH], hash2[HASH_LENGTH];
    unsigned char salt1[SALT_LENGTH], salt2[SALT_LENGTH];
    RAND_bytes(salt1, SALT_LENGTH);
    RAND_bytes(salt2, SALT_LENGTH);
    hash_password("password", salt1, hash1);
    hash_password("password", salt2, hash2);

    if(memcmp(hash1, hash2, HASH_LENGTH) != 0 && verify_password(hash1, "password")) {
        printf("Test 7: PASS\n"); pass_count++;
    } else {
        printf("Test 7: FAIL\n"); fail_count++;
    }

    // Test 8: login with altered stored password
    register_user("grace", "password");
    unsigned char altered_hash[HASH_LENGTH];
    unsigned char salt_alt[SALT_LENGTH];
    RAND_bytes(salt_alt, SALT_LENGTH);
    hash_password("differentpassword", salt_alt, altered_hash);
    // استبدال hash المستخدم grace
    for(int i=0; i<user_count; i++) {
        if(strcmp(users[i].username, "grace") == 0) {
            memcpy(users[i].password_hash, altered_hash, HASH_LENGTH);
            break;
        }
    }
    if(!login("grace", "password")) {
        printf("Test 8: PASS\n"); pass_count++;
    } else {
        printf("Test 8: FAIL\n"); fail_count++;
    }

    // Test 9: register and login with empty password
    if(register_user("hank", "") && login("hank", "") && !login("hank", "wrongpassword")) {
        printf("Test 9: PASS\n"); pass_count++;
    } else {
        printf("Test 9: FAIL\n"); fail_count++;
    }

    // Test 10: register and login with very long password
    char long_password[1001];
    for(int i=0; i<1000; i++) long_password[i] = 'a';
    long_password[1000] = '\0';

    if(register_user("ida", long_password) && login("ida", long_password) && !login("ida", "shortpassword")) {
        printf("Test 10: PASS\n"); pass_count++;
    } else {
        printf("Test 10: FAIL\n"); fail_count++;
    }

    printf("Total Passed: %d, Failed: %d\n", pass_count, fail_count);
}

int main() {
    run_tests();
    return 0;
}
