#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <stdlib.h>

#define MAX_USERS 100
#define MAX_LEN 50

typedef struct {
    char username[MAX_LEN];
    unsigned char password_hash[SHA256_DIGEST_LENGTH];
} User;

User users[MAX_USERS];
int user_count = 0;

void sha256_hash(const char *str, unsigned char output[SHA256_DIGEST_LENGTH]) {
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str, strlen(str));
    SHA256_Final(output, &sha256);
}

int register_user(const char *username, const char *password) {
    // تحقق من وجود المستخدم مسبقاً
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return 0; // اسم المستخدم موجود
        }
    }
    if (user_count >= MAX_USERS) return 0; // امتلاء

    // تخزين اسم المستخدم والهاش
    strcpy(users[user_count].username, username);
    sha256_hash(password, users[user_count].password_hash);
    user_count++;
    return 1; // تسجيل ناجح
}

int login_user(const char *username, const char *password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    sha256_hash(password, hash);

    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            if (memcmp(users[i].password_hash, hash, SHA256_DIGEST_LENGTH) == 0) {
                return 1; // نجاح تسجيل الدخول
            } else {
                return 0; // كلمة المرور خاطئة
            }
        }
    }
    return 0; // المستخدم غير موجود
}

// طباعة هاش كمجموعة HEX
void print_hash_hex(unsigned char hash[SHA256_DIGEST_LENGTH]) {
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");
}

void run_tests() {
    // اختبارات صارمة مشابهة للبايثون
    printf("Test 1: Register user1/password123\n");
    if (register_user("user1", "password123")) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

    printf("Test 2: Register user1 again\n");
    if (!register_user("user1", "anything")) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

    printf("Test 3: Login user1 with correct password\n");
    if (login_user("user1", "password123")) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

    printf("Test 4: Login user1 with wrong password\n");
    if (!login_user("user1", "wrongpass")) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

    printf("Test 5: Login non-existing user\n");
    if (!login_user("unknown", "nopass")) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }
}

int main() {
    char choice[10];
    char username[MAX_LEN];
    char password[MAX_LEN];

    run_tests();

    while (1) {
        printf("1. Register\n2. Login\n3. Exit\nChoose an option: ");
        if (!fgets(choice, sizeof(choice), stdin)) break;

        if (choice[0] == '1') {
            printf("Enter username: ");
            if (!fgets(username, sizeof(username), stdin)) break;
            username[strcspn(username, "\n")] = 0;

            printf("Enter password: ");
            if (!fgets(password, sizeof(password), stdin)) break;
            password[strcspn(password, "\n")] = 0;

            if (register_user(username, password)) {
                printf("Registration successful\n");
            } else {
                printf("Username already exists\n");
            }
        } else if (choice[0] == '2') {
            printf("Enter username: ");
            if (!fgets(username, sizeof(username), stdin)) break;
            username[strcspn(username, "\n")] = 0;

            printf("Enter password: ");
            if (!fgets(password, sizeof(password), stdin)) break;
            password[strcspn(password, "\n")] = 0;

            if (login_user(username, password)) {
                printf("Login successful\n");
            } else {
                printf("Invalid credentials\n");
            }
        } else if (choice[0] == '3') {
            break;
        } else {
            printf("Invalid choice\n");
        }
    }
    return 0;
}
