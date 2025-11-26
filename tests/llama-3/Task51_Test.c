#include <stdio.h>
#include <string.h>
#include <ctype.h>

// دالة التشفير كما أرسلتها (تعيد مؤشر لمصفوفة ثابتة)
char* encrypt(const char* text, int shift) {
    static char result[256];
    int i = 0;

    for (; *text; text++) {
        if (isalpha(*text)) {
            char base = isupper(*text) ? 'A' : 'a';
            int shifted = (*text - base + shift) % 26;
            if (shifted < 0) shifted += 26;
            result[i++] = (char)(shifted + base);
        } else {
            result[i++] = *text;
        }
    }
    result[i] = '\0';
    return result;
}

// دالة مساعدة لتشغيل الاختبارات
void run_test(const char* test_name, const char* input, int shift, const char* expected) {
    char* encrypted = encrypt(input, shift);
    if (strcmp(encrypted, expected) == 0) {
        printf("%s: PASS\n", test_name);
    } else {
        printf("%s: FAIL (Got: \"%s\", Expected: \"%s\")\n", test_name, encrypted, expected);
    }
}

int main() {
    run_test("test_basic_encryption", "abc", 3, "def");
    run_test("test_encryption_with_uppercase", "ABC", 3, "DEF");
    run_test("test_encryption_with_mixed_case", "aBc", 2, "cDe");
    run_test("test_encryption_with_non_alpha_characters", "a b-c", 5, "f g-h");
    run_test("test_encryption_with_large_shift_value", "xyz", 30, "bcd");
    run_test("test_encryption_with_negative_shift_value", "def", -3, "abc");
    run_test("test_encryption_with_shift_value_zero", "hello", 0, "hello");
    run_test("test_encryption_with_shift_value_26", "xyz", 26, "xyz");
    run_test("test_encryption_empty_string", "",1, "");
    run_test("test_encryption_non_alpha_characters_only", "1234!@#$",2, "1234!@#$");

    return 0;
}
