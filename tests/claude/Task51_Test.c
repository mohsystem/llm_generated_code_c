#include <stdio.h>
#include <string.h>
#include <ctype.h>

// الدالة الأصلية من كودك (بدون تعديل)
void encrypt(char *text, int shift) {
    int i;
    for (i = 0; text[i] != '\0'; ++i) {
        if (isalpha(text[i])) {
            char asciiOffset = isupper(text[i]) ? 65 : 97;
            text[i] = ((text[i] - asciiOffset + shift) % 26) + asciiOffset;
            // للتعامل مع shift سالب
            if (text[i] < asciiOffset) {
                text[i] += 26;
            }
        }
    }
}

// دالة مساعدة للمقارنة وطباعة النتائج
void run_test(const char* test_name, const char* input, int shift, const char* expected) {
    char buffer[1000];
    strncpy(buffer, input, sizeof(buffer));
    buffer[sizeof(buffer)-1] = '\0';

    encrypt(buffer, shift);

    if (strcmp(buffer, expected) == 0) {
        printf("%s: PASS\n", test_name);
    } else {
        printf("%s: FAIL (Got: \"%s\", Expected: \"%s\")\n", test_name, buffer, expected);
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
    run_test("test_encryption_empty_string", "", 5, "");
    run_test("test_encryption_non_alpha_characters_only", "1234!@#$", 4, "1234!@#$");

    return 0;
}
