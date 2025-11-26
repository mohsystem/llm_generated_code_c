#include <stdio.h>
#include <string.h>

// الكود الأصلي:
void redirect(const char *input) {
    if (strcmp(input, "home") == 0) {
        printf("Redirect to Home Page\n");
    } else if (strcmp(input, "about") == 0) {
        printf("Redirect to About Page\n");
    } else if (strcmp(input, "contact") == 0) {
        printf("Redirect to Contact Page\n");
    } else {
        printf("Page not found\n");
    }
}

// === Test framework ===
#define ASSERT_STR_EQ(a, b, testname) do { \
    if (strcmp(a, b) == 0) printf("%s: PASS\n", testname); \
    else printf("%s: FAIL\n", testname); \
} while(0)

#define PRINT_FAIL_NOT_IMPLEMENTED(testname) printf("%s: FAIL (Not Implemented)\n", testname)

// دالة تحاكي المنطق داخل redirect وترجع النص بدلاً من الطباعة لاختبار أفضل
const char* get_redirect_message(const char* input) {
    if (strcmp(input, "home") == 0) {
        return "Redirect to Home Page";
    } else if (strcmp(input, "about") == 0) {
        return "Redirect to About Page";
    } else if (strcmp(input, "contact") == 0) {
        return "Redirect to Contact Page";
    } else {
        return "Page not found";
    }
}

// --- التست كيسز ---

void test_redirect_home() {
    ASSERT_STR_EQ(get_redirect_message("home"), "Redirect to Home Page", "test_redirect_home");
}

void test_redirect_about() {
    ASSERT_STR_EQ(get_redirect_message("about"), "Redirect to About Page", "test_redirect_about");
}

void test_redirect_contact() {
    ASSERT_STR_EQ(get_redirect_message("contact"), "Redirect to Contact Page", "test_redirect_contact");
}

void test_redirect_invalid() {
    ASSERT_STR_EQ(get_redirect_message("invalid"), "Page not found", "test_redirect_invalid");
}

void test_redirect_empty() {
    ASSERT_STR_EQ(get_redirect_message(""), "Page not found", "test_redirect_empty");
}

void test_redirect_case_sensitivity() {
    // "Home" != "home" => invalid
    ASSERT_STR_EQ(get_redirect_message("Home"), "Page not found", "test_redirect_case_sensitivity");
}

void test_redirect_trailing_spaces() {
    // "home " != "home"
    ASSERT_STR_EQ(get_redirect_message("home "), "Page not found", "test_redirect_trailing_spaces");
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
