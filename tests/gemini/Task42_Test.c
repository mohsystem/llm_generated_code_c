#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/sha.h>

#define MAX_USERNAME_LEN 100
#define MAX_PASSWORD_LEN 100
#define HASH_LEN (SHA256_DIGEST_LENGTH * 2 + 1)
#define MAX_USERS 100

struct User {
    char username[MAX_USERNAME_LEN];
    char password_hash[HASH_LEN];
};

struct User users[MAX_USERS];
int user_count = 0;

// هش كلمة المرور
void hash_password(const char *password, char *hashed_password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, password, strlen(password));
    SHA256_Final(hash, &sha256);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hashed_password + (i * 2), "%02x", hash[i]);
    }
    hashed_password[HASH_LEN - 1] = '\0';
}

// تسجيل مستخدم جديد
int register_user(const char* username, const char* password) {
    if (user_count >= MAX_USERS) return 0;

    // تحقق من عدم وجود المستخدم مسبقًا
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0)
            return 0; // مستخدم موجود
    }

    strcpy(users[user_count].username, username);
    hash_password(password, users[user_count].password_hash);
    user_count++;
    return 1;
}

// التحقق من تسجيل الدخول
int login(const char* username, const char* password) {
    char hash[HASH_LEN];
    hash_password(password, hash);
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0 &&
            strcmp(users[i].password_hash, hash) == 0) {
            return 1; // نجاح
        }
    }
    return 0; // فشل
}

// دالة للاختبارات بنفس حالات البايثون
void run_tests() {
    int pass = 0, fail = 0;

    // Test 1: تسجيل مستخدم جديد
    if (register_user("bob", "securepassword") && user_count > 0) { printf("Test 1: PASS\n"); pass++; } else { printf("Test 1: FAIL\n"); fail++; }

    // Test 2: تسجيل مستخدم موجود مسبقاً
    register_user("alice", "password123");
    if (!register_user("alice", "newpassword")) { printf("Test 2: PASS\n"); pass++; } else { printf("Test 2: FAIL\n"); fail++; }

    // Test 3: تسجيل دخول ناجح
    register_user("charlie", "mypassword");
    if (login("charlie", "mypassword")) { printf("Test 3: PASS\n"); pass++; } else { printf("Test 3: FAIL\n"); fail++; }

    // Test 4: تسجيل دخول بكلمة سر خاطئة
    register_user("david", "password");
    if (!login("david", "wrongpassword")) { printf("Test 4: PASS\n"); pass++; } else { printf("Test 4: FAIL\n"); fail++; }

    // Test 5: تسجيل دخول لمستخدم غير موجود
    if (!login("nonexistent", "password")) { printf("Test 5: PASS\n"); pass++; } else { printf("Test 5: FAIL\n"); fail++; }

    // Test 6: تسجيل مستخدمين متعددين وتسجيل دخول ناجح
    register_user("eve", "pass1");
    register_user("frank", "pass2");
    if (login("eve", "pass1") && login("frank", "pass2")) { printf("Test 6: PASS\n"); pass++; } else { printf("Test 6: FAIL\n"); fail++; }

    // Test 7: تحقق من ثبات الهاش (هنا فقط تحقق أن تسجيل الدخول يعمل)
    register_user("grace", "password");
    if (login("grace", "password")) { printf("Test 7: PASS\n"); pass++; } else { printf("Test 7: FAIL\n"); fail++; }

    // Test 8: تسجيل دخول مع تغيير هاش كلمة السر
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, "grace") == 0) {
            strcpy(users[i].password_hash, "invalidhash");
        }
    }
    if (!login("grace", "password")) { printf("Test 8: PASS\n"); pass++; } else { printf("Test 8: FAIL\n"); fail++; }

    // Test 9: تسجيل مستخدم وكلمة سر فارغة وتسجيل الدخول
    if (register_user("hank", "") && login("hank", "") && !login("hank", "wrongpassword")) { printf("Test 9: PASS\n"); pass++; } else { printf("Test 9: FAIL\n"); fail++; }

    // Test 10: تسجيل مستخدم وكلمة سر طويلة وتسجيل الدخول
    char long_pass[1001];
    for (int i = 0; i < 1000; i++) long_pass[i] = 'a';
    long_pass[1000] = '\0';
    if (register_user("ida", long_pass) && login("ida", long_pass) && !login("ida", "shortpassword")) { printf("Test 10: PASS\n"); pass++; } else { printf("Test 10: FAIL\n"); fail++; }

    printf("Total Passed: %d, Failed: %d\n", pass, fail);
}

int main() {
    run_tests();
    return 0;
}
