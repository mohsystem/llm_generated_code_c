#include <stdio.h>
#include <string.h>

int authenticate(const char* username, const char* password) {
    const char* storedUsername = "user";
    const char* storedPassword = "pass";
    return strcmp(username, storedUsername) == 0 && strcmp(password, storedPassword) == 0;
}

void run_tests() {
    printf("Test 1: Correct username and password\n");
    if (authenticate("user", "pass")) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

    printf("Test 2: Correct username, wrong password\n");
    if (!authenticate("user", "wrongpass")) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

    printf("Test 3: Wrong username, correct password\n");
    if (!authenticate("wronguser", "pass")) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

    printf("Test 4: Wrong username and password\n");
    if (!authenticate("wronguser", "wrongpass")) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

    printf("Test 5: Empty username and password\n");
    if (!authenticate("", "")) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }
}

int main() {
    run_tests();

    // التفاعل مع المستخدم كما في الكود الأصلي
    char username[50], password[50];

    printf("Enter username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0; // إزالة نهاية السطر

    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;

    if (authenticate(username, password)) {
        printf("Access granted\n");
    } else {
        printf("Access denied\n");
    }

    return 0;
}
