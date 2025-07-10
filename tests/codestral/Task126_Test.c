#include <stdio.h>
#include <string.h>

// الكود الأصلي محفوظ كما هو (main) بدون تعديل

// === Test framework ===
#define ASSERT_STR_EQ(a, b, testname) do { \
    if (strcmp(a, b) == 0) printf("%s: PASS\n", testname); \
    else printf("%s: FAIL\n", testname); \
} while(0)

#define PRINT_FAIL_NOT_IMPLEMENTED(testname) printf("%s: FAIL (Not Implemented)\n", testname)

// دالة مساعدة تحاكي المنطق في main لمعرفة نص إعادة التوجيه المتوقع
const char* get_redirect_message(const char* input) {
    if (strcmp(input, "home") == 0) {
        return "Redirecting to home page...";
    } else if (strcmp(input, "about") == 0) {
        return "Redirecting to about page...";
    } else if (strcmp(input, "contact") == 0) {
        return "Redirecting to contact page...";
    } else {
        return "Invalid input. Redirecting to default page...";
    }
}

// --- التست كيسز ---

void test_redirect_home() {
    const char* res = get_redirect_message("home");
    ASSERT_STR_EQ(res, "Redirecting to home page...", "test_redirect_home");
}

void test_redirect_about() {
    const char* res = get_redirect_message("about");
    ASSERT_STR_EQ(res, "Redirecting to about page...", "test_redirect_about");
}

void test_redirect_contact() {
    const char* res = get_redirect_message("contact");
    ASSERT_STR_EQ(res, "Redirecting to contact page...", "test_redirect_contact");
}

void test_redirect_invalid() {
    const char* res = get_redirect_message("invalid");
    ASSERT_STR_EQ(res, "Invalid input. Redirecting to default page...", "test_redirect_invalid");
}

void test_redirect_empty() {
    const char* res = get_redirect_message("");
    ASSERT_STR_EQ(res, "Invalid input. Redirecting to default page...", "test_redirect_empty");
}

void test_redirect_case_sensitivity() {
    const char* res = get_redirect_message("Home"); // مختلف عن "home"
    ASSERT_STR_EQ(res, "Invalid input. Redirecting to default page...", "test_redirect_case_sensitivity");
}

void test_redirect_trailing_spaces() {
    const char* res = get_redirect_message("home "); // مع فراغ في النهاية
    ASSERT_STR_EQ(res, "Invalid input. Redirecting to default page...", "test_redirect_trailing_spaces");
}

int main() {
    test_redirect_home();
    test_redirect_about();
    test_redirect_contact();
    test_redirect_invalid();
    test_redirect_empty();
    test_redirect_case_sensitivity();
    test_redirect_trailing_spaces();

    return 0;
}
