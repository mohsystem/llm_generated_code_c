#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LENGTH 50

// الكود الأصلي (دوال authenticate و access_resource)

bool authenticate() {
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);
    return (strcmp(username, "admin") == 0 && strcmp(password, "password123") == 0);
}

void access_resource(const char* resource) {
    if (strcmp(resource, "1") == 0) {
        printf("Accessing confidential files...\n");
    } else if (strcmp(resource, "2") == 0) {
        printf("Accessing financial records...\n");
    } else if (strcmp(resource, "3") == 0) {
        printf("Accessing employee database...\n");
    } else {
        printf("Invalid resource selection.\n");
    }
}

// --- هنا نضيف التست كيسز فقط ---

// بدل الدالة الأصلية authenticate بنسخة جديدة تدعم اختبار
bool authenticate_test(const char* username, const char* password) {
    return (strcmp(username, "admin") == 0 && strcmp(password, "password123") == 0);
}

// تابع اختبار: مقارنة قيمتين بوليانية
#define ASSERT_BOOL_EQ(val, expected, testname) do { \
    if ((val) == (expected)) printf("%s: PASS\n", testname); \
    else printf("%s: FAIL\n", testname); \
} while(0)

// تابع طباعة FAIL للحالات غير المنفذة
#define PRINT_FAIL_NOT_IMPLEMENTED(testname) printf("%s: FAIL (Not Implemented)\n", testname)

// اختبار جميع الحالات الصارمة من البايثون

void test_valid_credentials() {
    ASSERT_BOOL_EQ(authenticate_test("admin", "password123"), true, "test_valid_credentials");
}

void test_invalid_password() {
    ASSERT_BOOL_EQ(authenticate_test("admin", "wrongpassword"), false, "test_invalid_password");
}

void test_invalid_username() {
    ASSERT_BOOL_EQ(authenticate_test("wronguser", "password123"), false, "test_invalid_username");
}

void test_invalid_username_and_password() {
    ASSERT_BOOL_EQ(authenticate_test("wronguser", "wrongpassword"), false, "test_invalid_username_and_password");
}

void test_empty_password() {
    ASSERT_BOOL_EQ(authenticate_test("admin", ""), false, "test_empty_password");
}

void test_empty_username() {
    ASSERT_BOOL_EQ(authenticate_test("", "password123"), false, "test_empty_username");
}

void test_empty_username_and_password() {
    ASSERT_BOOL_EQ(authenticate_test("", ""), false, "test_empty_username_and_password");
}

void test_valid_username_with_extra_spaces() {
    ASSERT_BOOL_EQ(authenticate_test(" admin ", "password123"), false, "test_valid_username_with_extra_spaces");
}

void test_valid_password_with_extra_spaces() {
    ASSERT_BOOL_EQ(authenticate_test("admin", " password123 "), false, "test_valid_password_with_extra_spaces");
}

void test_valid_credentials_with_trailing_spaces() {
    ASSERT_BOOL_EQ(authenticate_test("admin ", " password123"), false, "test_valid_credentials_with_trailing_spaces");
}

// لا يوجد دعم لاختبار access_resource لأن الكود يطبع فقط (ليس له ناتج قابل للفحص هنا)
// نطبع Not Implemented لهذا الجزء

void test_access_resource_1() { PRINT_FAIL_NOT_IMPLEMENTED("test_access_resource_1"); }
void test_access_resource_2() { PRINT_FAIL_NOT_IMPLEMENTED("test_access_resource_2"); }
void test_access_resource_3() { PRINT_FAIL_NOT_IMPLEMENTED("test_access_resource_3"); }
void test_access_resource_invalid() { PRINT_FAIL_NOT_IMPLEMENTED("test_access_resource_invalid"); }

int main() {
    // فقط تنفيذ التست كيسز الخاصة بالauthenticate من البايثون
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

    // طباعة FAIL لحالات اختبار الوصول للموارد لأنها غير قابلة للاختبار حالياً
    test_access_resource_1();
    test_access_resource_2();
    test_access_resource_3();
    test_access_resource_invalid();

    return 0;
}
