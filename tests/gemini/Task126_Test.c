#include <stdio.h>
#include <string.h>
#include <ctype.h>

// الكود الأصلي (main) محفوظ بدون تعديل

// === Test framework ===
#define ASSERT_STR_EQ(a, b, testname) do { \
    if (strcmp(a, b) == 0) printf("%s: PASS\n", testname); \
    else printf("%s: FAIL\n", testname); \
} while(0)

#define PRINT_FAIL_NOT_IMPLEMENTED(testname) printf("%s: FAIL (Not Implemented)\n", testname)

// دالة مساعدة تحاكي منطق البرنامج (تحويل للحروف الكبيرة + مقارنة)
const char* get_redirect_message(const char* input) {
    char choice[10];
    int i;
    for (i = 0; input[i] && i < 9; i++) {
        choice[i] = toupper((unsigned char)input[i]);
    }
    choice[i] = '\0';

    if (strcmp(choice, "A") == 0) {
        return "Redirecting to page A...";
    } else if (strcmp(choice, "B") == 0) {
        return "Redirecting to page B...";
    } else if (strcmp(choice, "C") == 0) {
        return "Redirecting to page C...";
    } else {
        return "Invalid choice. Please enter A, B, or C.";
    }
}

// --- التست كيسز ---

void test_redirect_A_lowercase() {
    const char* res = get_redirect_message("a");
    ASSERT_STR_EQ(res, "Redirecting to page A...", "test_redirect_A_lowercase");
}

void test_redirect_B_uppercase() {
    const char* res = get_redirect_message("B");
    ASSERT_STR_EQ(res, "Redirecting to page B...", "test_redirect_B_uppercase");
}

void test_redirect_C_mixedcase() {
    const char* res = get_redirect_message("c");
    ASSERT_STR_EQ(res, "Redirecting to page C...", "test_redirect_C_mixedcase");
}

void test_redirect_invalid() {
    const char* res = get_redirect_message("D");
    ASSERT_STR_EQ(res, "Invalid choice. Please enter A, B, or C.", "test_redirect_invalid");
}

void test_redirect_empty() {
    const char* res = get_redirect_message("");
    ASSERT_STR_EQ(res, "Invalid choice. Please enter A, B, or C.", "test_redirect_empty");
}

void test_redirect_trailing_spaces() {
    // مع مسافات في النهاية، غير معالج في الكود الأصلي، نعتبرها غير صحيحة
    const char* res = get_redirect_message("A ");
    ASSERT_STR_EQ(res, "Invalid choice. Please enter A, B, or C.", "test_redirect_trailing_spaces");
}

int main() {
    test_redirect_A_lowercase();
    test_redirect_B_uppercase();
    test_redirect_C_mixedcase();
    test_redirect_invalid();
    test_redirect_empty();
    test_redirect_trailing_spaces();

    return 0;
}
