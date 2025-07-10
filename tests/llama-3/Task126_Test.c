#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// الكود الأصلي محفوظ كما هو، بدون تعديل

// === Test framework ===
#define ASSERT_STR_EQ(a, b, testname) do { \
if ((a == NULL && b == NULL) || (a != NULL && b != NULL && strcmp(a, b) == 0)) \
printf("%s: PASS\n", testname); \
else \
printf("%s: FAIL\n", testname); \
} while(0)

#define PRINT_FAIL_NOT_IMPLEMENTED(testname) printf("%s: FAIL (Not Implemented)\n", testname)

// دالة محاكاة لاختيار URL حسب مدخل المستخدم (مثل الكود الأصلي)
const char* get_url_for_input(const char* input) {
    if (strcmp(input, "google") == 0) {
        return "https://www.google.com";
    } else if (strcmp(input, "facebook") == 0) {
        return "https://www.facebook.com";
    } else if (strcmp(input, "youtube") == 0) {
        return "https://www.youtube.com";
    } else {
        return NULL;
    }
}

// --- التست كيسز ---

void test_google() {
    const char* url = get_url_for_input("google");
    ASSERT_STR_EQ(url, "https://www.google.com", "test_google");
}

void test_facebook() {
    const char* url = get_url_for_input("facebook");
    ASSERT_STR_EQ(url, "https://www.facebook.com", "test_facebook");
}

void test_youtube() {
    const char* url = get_url_for_input("youtube");
    ASSERT_STR_EQ(url, "https://www.youtube.com", "test_youtube");
}

void test_invalid_input() {
    const char* url = get_url_for_input("twitter");
    ASSERT_STR_EQ(url, NULL, "test_invalid_input");
}

void test_empty_input() {
    const char* url = get_url_for_input("");
    ASSERT_STR_EQ(url, NULL, "test_empty_input");
}

// لا يمكن اختبار system() لفتح المتصفح (خارج نطاق الاختبار)
void test_system_call_execution() {
    PRINT_FAIL_NOT_IMPLEMENTED("test_system_call_execution");
}

int main() {
    test_google();
    test_facebook();
    test_youtube();
    test_invalid_input();
    test_empty_input();
    test_system_call_execution();
    return 0;
}
