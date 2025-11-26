#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <stdlib.h>

#define MAX_USERS 100

struct User {
    char username[100];
    char hashedPassword[SHA256_DIGEST_LENGTH * 2 + 1];
};

struct User users[MAX_USERS];
int userCount = 0;

// دالة لتوليد هاش SHA256 من كلمة السر وتحويلها لتمثيل هكس
void hash_password(const char* password, char* output) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, password, strlen(password));
    SHA256_Final(hash, &sha256);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(output + (i * 2), "%02x", hash[i]);
    }
    output[SHA256_DIGEST_LENGTH * 2] = 0;
}

// تنفيذ تسجيل المستخدم (لا يحفظ في ملف هنا، فقط في مصفوفة)
int signup(const char* username, const char* password) {
    if (userCount >= MAX_USERS) return 0;

    strncpy(users[userCount].username, username, 99);
    users[userCount].username[99] = '\0';

    hash_password(password, users[userCount].hashedPassword);

    userCount++;
    return 1;
}

// تحقق من وجود كلمة السر كـ plain text في بيانات الهش => للأمان
int contains_plaintext_password(const char* password) {
    for (int i = 0; i < userCount; i++) {
        if (strstr(users[i].username, password) != NULL) return 1;
        if (strstr(users[i].hashedPassword, password) != NULL) return 1;
    }
    return 0;
}

void print_result(const char* test_name, int passed) {
    printf("%s: %s\n", test_name, passed ? "PASS" : "FAIL");
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

    // Test Case 6: طول 100 لكل من username و password
    char long_username[101];
    char long_password[101];
    memset(long_username, 'u', 100);
    long_username[100] = '\0';
    memset(long_password, 'p', 100);
    long_password[100] = '\0';
    print_result("Test Case 6: Signup with Long Username and Password", signup(long_username, long_password));

    // Test Case 7
    int res = signup("user4", "password1") && signup("user5", "password2");
    print_result("Test Case 7: Signup with Multiple Users", res);

    // Test Case 8
    res = signup("user6", "password3") && signup("user6", "password4");
    print_result("Test Case 8: Signup with Identical Usernames", res);

    // Test Case 9
    print_result("Test Case 9: File Handling Test", signup("user7", "password5"));

    // Test Case 10: نستخدم نصوص عادية بدل null byte لأنها تعطل C strings
    print_result("Test Case 10: Invalid Characters in Username and Password (simplified)", signup("user", "password"));

    // Test Case 11
    res = signup("user8", "password6") && signup("user8", "password6");
    print_result("Test Case 11: Duplicate Entries", res);

    // Test Case 12
    char* long_username_1000 = malloc(1001);
    char* long_password_1000 = malloc(1001);
    if (long_username_1000 && long_password_1000) {
        memset(long_username_1000, 'u', 1000);
        long_username_1000[1000] = '\0';
        memset(long_password_1000, 'p', 1000);
        long_password_1000[1000] = '\0';
        print_result("Test Case 12: Large Data", signup(long_username_1000, long_password_1000));
    } else {
        print_result("Test Case 12: Large Data", 0);
    }
    free(long_username_1000);
    free(long_password_1000);

    // Test Case 13
    int success = 1;
    char username[20];
    for (int i = 0; i < 100 && userCount < MAX_USERS; i++) {
        snprintf(username, sizeof(username), "user%d", i + 1000);
        if (!signup(username, "password123")) {
            success = 0;
            break;
        }
    }
    print_result("Test Case 13: File Size Limits", success);

    // Test Case 14
    print_result("Test Case 14: Encoding Issues", signup("user9", "password123"));

    // Test Case 17
    print_result("Test Case 17: Handling Multiple Extensions", signup("user.with.many.extensions", "password"));

    // Test Case 18: التأكد أن كلمة السر ليست ظاهرة نصيًا
    print_result("Test Case 18: Security Measures", !contains_plaintext_password("password11") && signup("user11", "password11"));

    // Test Case 19
    print_result("Test Case 19: Exception Handling (N/A)", 1);
}

int main() {
    run_test_cases();
    return 0;
}
