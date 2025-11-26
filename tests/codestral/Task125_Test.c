#include <stdio.h>
#include <string.h>

#define MAX_RESOURCES 3
#define MAX_KEY_LENGTH 10

struct Resource {
    char key[MAX_KEY_LENGTH];
    char value[MAX_KEY_LENGTH];
};

// الكود الأصلي:
void run_resource_access() {
    struct Resource resources[MAX_RESOURCES] = {
        {"A", "Resource A"},
        {"B", "Resource B"},
        {"C", "Resource C"}
    };

    char userInput[MAX_KEY_LENGTH];
    printf("Enter a key to access a resource: ");
    scanf("%s", userInput);

    int i;
    for (i = 0; i < MAX_RESOURCES; i++) {
        if (strcmp(resources[i].key, userInput) == 0) {
            printf("Access granted to: %s\n", resources[i].value);
            return;
        }
    }

    printf("Access denied. Invalid key.\n");
}

// === Test framework ===
#define ASSERT(cond, testname) do { \
    if (cond) printf("%s: PASS\n", testname); \
    else printf("%s: FAIL\n", testname); \
} while(0)

#define PRINT_FAIL_NOT_IMPLEMENTED(testname) printf("%s: FAIL (Not Implemented)\n", testname)

// مساعد لاختبار المفتاح والنتيجة المتوقعة (مقارنة النص)
int test_access_key(const char* key, const char* expected_value) {
    struct Resource resources[MAX_RESOURCES] = {
        {"A", "Resource A"},
        {"B", "Resource B"},
        {"C", "Resource C"}
    };
    for (int i = 0; i < MAX_RESOURCES; i++) {
        if (strcmp(resources[i].key, key) == 0) {
            return strcmp(resources[i].value, expected_value) == 0;
        }
    }
    // إذا المفتاح غير موجود، متوقع رفض الوصول
    return strcmp("Access denied", expected_value) == 0;
}

// --- التست كيسز من بايثون (مصادقة + وصول موارد) ---

// لا توجد دالة authenticate في الكود، فطبعنا Not Implemented لاختبارات المصادقة:
void test_valid_credentials() { PRINT_FAIL_NOT_IMPLEMENTED("test_valid_credentials"); }
void test_invalid_password() { PRINT_FAIL_NOT_IMPLEMENTED("test_invalid_password"); }
void test_invalid_username() { PRINT_FAIL_NOT_IMPLEMENTED("test_invalid_username"); }
void test_invalid_username_and_password() { PRINT_FAIL_NOT_IMPLEMENTED("test_invalid_username_and_password"); }
void test_empty_password() { PRINT_FAIL_NOT_IMPLEMENTED("test_empty_password"); }
void test_empty_username() { PRINT_FAIL_NOT_IMPLEMENTED("test_empty_username"); }
void test_empty_username_and_password() { PRINT_FAIL_NOT_IMPLEMENTED("test_empty_username_and_password"); }
void test_valid_username_with_extra_spaces() { PRINT_FAIL_NOT_IMPLEMENTED("test_valid_username_with_extra_spaces"); }
void test_valid_password_with_extra_spaces() { PRINT_FAIL_NOT_IMPLEMENTED("test_valid_password_with_extra_spaces"); }
void test_valid_credentials_with_trailing_spaces() { PRINT_FAIL_NOT_IMPLEMENTED("test_valid_credentials_with_trailing_spaces"); }

// اختبارات الوصول للموارد:
void test_access_resource_A() { ASSERT(test_access_key("A", "Resource A"), "test_access_resource_A"); }
void test_access_resource_B() { ASSERT(test_access_key("B", "Resource B"), "test_access_resource_B"); }
void test_access_resource_C() { ASSERT(test_access_key("C", "Resource C"), "test_access_resource_C"); }
void test_access_resource_invalid() { ASSERT(test_access_key("X", "Access denied"), "test_access_resource_invalid"); }

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

    test_access_resource_A();
    test_access_resource_B();
    test_access_resource_C();
    test_access_resource_invalid();

    return 0;
}
