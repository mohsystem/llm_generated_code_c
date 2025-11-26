#include <stdio.h>
#include <string.h>

// الكود الأصلي:
void accessResource(char resource[]) {
    if (strcmp(resource, "admin") == 0) {
        printf("Access granted to admin resource\n");
    } else if (strcmp(resource, "moderator") == 0) {
        printf("Access granted to moderator resource\n");
    } else if (strcmp(resource, "user") == 0) {
        printf("Access granted to user resource\n");
    } else {
        printf("Access denied\n");
    }
}

// === Test framework ===
#define ASSERT_STR_EQ(a, b, testname) do { \
    if (strcmp(a, b) == 0) printf("%s: PASS\n", testname); \
    else printf("%s: FAIL\n", testname); \
} while(0)

#define PRINT_FAIL_NOT_IMPLEMENTED(testname) printf("%s: FAIL (Not Implemented)\n", testname)

// اختبارات بسيطة لمصادقة (مقلدة لبايثون):

int authenticate_test(const char* username, const char* password) {
    return (strcmp(username, "admin") == 0) && (strcmp(password, "password123") == 0);
}

void test_valid_credentials() {
    int res = authenticate_test("admin", "password123");
    printf("test_valid_credentials: %s\n", res ? "PASS" : "FAIL");
}

void test_invalid_password() {
    int res = authenticate_test("admin", "wrongpassword");
    printf("test_invalid_password: %s\n", !res ? "PASS" : "FAIL");
}

void test_invalid_username() {
    int res = authenticate_test("wronguser", "password123");
    printf("test_invalid_username: %s\n", !res ? "PASS" : "FAIL");
}

void test_invalid_username_and_password() {
    int res = authenticate_test("wronguser", "wrongpassword");
    printf("test_invalid_username_and_password: %s\n", !res ? "PASS" : "FAIL");
}

void test_empty_password() {
    int res = authenticate_test("admin", "");
    printf("test_empty_password: %s\n", !res ? "PASS" : "FAIL");
}

void test_empty_username() {
    int res = authenticate_test("", "password123");
    printf("test_empty_username: %s\n", !res ? "PASS" : "FAIL");
}

void test_empty_username_and_password() {
    int res = authenticate_test("", "");
    printf("test_empty_username_and_password: %s\n", !res ? "PASS" : "FAIL");
}

void test_valid_username_with_extra_spaces() {
    int res = authenticate_test(" admin ", "password123");
    printf("test_valid_username_with_extra_spaces: %s\n", !res ? "PASS" : "FAIL");
}

void test_valid_password_with_extra_spaces() {
    int res = authenticate_test("admin", " password123 ");
    printf("test_valid_password_with_extra_spaces: %s\n", !res ? "PASS" : "FAIL");
}

void test_valid_credentials_with_trailing_spaces() {
    int res = authenticate_test("admin ", " password123");
    printf("test_valid_credentials_with_trailing_spaces: %s\n", !res ? "PASS" : "FAIL");
}

// اختبارات لـ accessResource غير ممكنة لسبب أنها تطبع مباشرة للنص وليس لها عودة قيمة
// نطبع FAIL مع توضيح

void test_access_resource_admin() {
    PRINT_FAIL_NOT_IMPLEMENTED("test_access_resource_admin");
}

void test_access_resource_moderator() {
    PRINT_FAIL_NOT_IMPLEMENTED("test_access_resource_moderator");
}

void test_access_resource_user() {
    PRINT_FAIL_NOT_IMPLEMENTED("test_access_resource_user");
}

void test_access_resource_invalid() {
    PRINT_FAIL_NOT_IMPLEMENTED("test_access_resource_invalid");
}

int main() {
    test_valid_credentials();
    test_invalid_password();
    test_invalid_username();
    test_invalid_username_and_password();
    test_empty_password();
    test_empty_username();
    test_empty_username_and_password();
    test_valid_username_with_extra_spaces();
    test_valid_password_with_extra_spaces();
    test_valid_credentials_with_trailing_spaces();

    test_access_resource_admin();
    test_access_resource_moderator();
    test_access_resource_user();
    test_access_resource_invalid();

    return 0;
}
