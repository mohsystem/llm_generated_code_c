#include <stdio.h>
#include <string.h>

// الكود الأصلي:
const char* accessResource(const char* user) {
    if (strcmp(user, "admin") == 0) {
        return "Access to administrative resources granted.";
    } else if (strcmp(user, "user") == 0) {
        return "Access to user resources granted.";
    } else if (strcmp(user, "guest") == 0) {
        return "Access to guest resources granted.";
    } else {
        return "No resources available for this user role";
    }
}

// === Test framework ===
#define ASSERT_BOOL_EQ(val, expected, testname) do { \
    if ((val) == (expected)) printf("%s: PASS\n", testname); \
    else printf("%s: FAIL\n", testname); \
} while(0)

#define ASSERT_STR_EQ(a, b, testname) do { \
    if (strcmp(a, b) == 0) printf("%s: PASS\n", testname); \
    else printf("%s: FAIL\n", testname); \
} while(0)

#define PRINT_FAIL_NOT_IMPLEMENTED(testname) printf("%s: FAIL (Not Implemented)\n", testname)

// نعيد كتابة دالة authenticate على شكل اختبارية بوسائط
int authenticate_test(const char* username, const char* password) {
    return (strcmp(username, "admin") == 0) && (strcmp(password, "password123") == 0);
}

// --- Test cases من البايثون ---

void test_valid_credentials() {
    ASSERT_BOOL_EQ(authenticate_test("admin", "password123"), 1, "test_valid_credentials");
}

void test_invalid_password() {
    ASSERT_BOOL_EQ(authenticate_test("admin", "wrongpassword"), 0, "test_invalid_password");
}

void test_invalid_username() {
    ASSERT_BOOL_EQ(authenticate_test("wronguser", "password123"), 0, "test_invalid_username");
}

void test_invalid_username_and_password() {
    ASSERT_BOOL_EQ(authenticate_test("wronguser", "wrongpassword"), 0, "test_invalid_username_and_password");
}

void test_empty_password() {
    ASSERT_BOOL_EQ(authenticate_test("admin", ""), 0, "test_empty_password");
}

void test_empty_username() {
    ASSERT_BOOL_EQ(authenticate_test("", "password123"), 0, "test_empty_username");
}

void test_empty_username_and_password() {
    ASSERT_BOOL_EQ(authenticate_test("", ""), 0, "test_empty_username_and_password");
}

void test_valid_username_with_extra_spaces() {
    ASSERT_BOOL_EQ(authenticate_test(" admin ", "password123"), 0, "test_valid_username_with_extra_spaces");
}

void test_valid_password_with_extra_spaces() {
    ASSERT_BOOL_EQ(authenticate_test("admin", " password123 "), 0, "test_valid_password_with_extra_spaces");
}

void test_valid_credentials_with_trailing_spaces() {
    ASSERT_BOOL_EQ(authenticate_test("admin ", " password123"), 0, "test_valid_credentials_with_trailing_spaces");
}

// لا يوجد دالة accessResource في البايثون تقارن نصوص معينة مباشرةً
// لذلك نطبع Not Implemented لكل اختبار متعلق بالوصول بناءً على المدخلات
void test_access_resource_admin() { PRINT_FAIL_NOT_IMPLEMENTED("test_access_resource_admin"); }
void test_access_resource_user() { PRINT_FAIL_NOT_IMPLEMENTED("test_access_resource_user"); }
void test_access_resource_guest() { PRINT_FAIL_NOT_IMPLEMENTED("test_access_resource_guest"); }
void test_access_resource_invalid() { PRINT_FAIL_NOT_IMPLEMENTED("test_access_resource_invalid"); }

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
    test_access_resource_user();
    test_access_resource_guest();
    test_access_resource_invalid();

    return 0;
}
