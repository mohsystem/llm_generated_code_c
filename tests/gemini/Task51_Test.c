#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// تابع التشفير كما هو مع دعم الحروف والأرقام
char* encrypt(char* text, int key) {
    char* result = strdup(text);
    for (int i = 0; result[i] != '\0'; i++) {
        if (isalpha(result[i])) {
            char start = islower(result[i]) ? 'a' : 'A';
            int shifted = (result[i] - start + key) % 26;
            if (shifted < 0) shifted += 26;
            result[i] = (char)(shifted + start);
        } else if (isdigit(result[i])) {
            int shifted = ((result[i] - '0') + key) % 10;
            if (shifted < 0) shifted += 10;
            result[i] = (char)(shifted + '0');
        }
    }
    return result;
}

// دالة مساعدة لتشغيل الاختبارات
void run_test(const char* test_name, const char* input, int key, const char* expected) {
    char* encrypted = encrypt((char*)input, key);
    if (strcmp(encrypted, expected) == 0) {
        printf("%s: PASS\n", test_name);
    } else {
        printf("%s: FAIL (Got: \"%s\", Expected: \"%s\")\n", test_name, encrypted, expected);
    }
    free(encrypted);
}

int main() {
    run_test("test_basic_encryption", "abc", 3, "def");
    run_test("test_encryption_with_uppercase", "ABC", 3, "DEF");
    run_test("test_encryption_with_mixed_case", "aBc", 2, "cDe");
    run_test("test_encryption_with_non_alpha_characters", "a b-c", 5, "f g-h"); // رموز لا تتغير، نفس الحالة
    run_test("test_encryption_with_large_shift_value", "xyz", 30, "bcd");
    run_test("test_encryption_with_negative_shift_value", "def", -3, "abc");
    run_test("test_encryption_with_shift_value_zero", "hello", 0, "hello");
    run_test("test_encryption_with_shift_value_26", "xyz", 26, "xyz");
    run_test("test_encryption_empty_string", "", 3, "");
    run_test("test_encryption_non_alpha_characters_only", "1234!@#$", 4, "5678!@#$");
    // أضفت حالة لأرقام أيضاً بناء على دالة encrypt

    return 0;
}
