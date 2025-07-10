#include <stdio.h>
#include <string.h>
#include <ctype.h>

// الدالة الأصلية التي أرسلتها
void encrypt(char text[], char result[]) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (isupper(text[i])) {
            result[i] = (text[i] + 1 - 'A') % 26 + 'A';
        } else if (islower(text[i])) {
            result[i] = (text[i] + 1 - 'a') % 26 + 'a';
        } else {
            result[i] = text[i];
        }
    }
}

// دالة مساعدة لتشغيل الاختبارات
void run_test(const char* test_name, const char* input, const char* expected) {
    char output[1000] = {0};
    encrypt((char*)input, output);

    if (strcmp(output, expected) == 0) {
        printf("%s: PASS\n", test_name);
    } else {
        printf("%s: FAIL (Got: \"%s\", Expected: \"%s\")\n", test_name, output, expected);
    }
}

int main() {
    run_test("test_basic_encryption", "abc", "bcd");
    run_test("test_encryption_with_uppercase", "ABC", "BCD");
    run_test("test_encryption_with_mixed_case", "aBc", "bCd");
    run_test("test_encryption_with_non_alpha_characters", "a b-c", "b c-d"); // المسافات والشرطة تبقى كما هي
    run_test("test_encryption_with_large_shift_value", "xyz", "yza"); // shift = 1 دائمًا هنا
    run_test("test_encryption_with_negative_shift_value", "def", "efg"); // لا يدعم shift سلبي
    run_test("test_encryption_with_shift_value_zero", "hello", "ifmmp"); // لا يدعم shift صفر، فقط shift=1
    run_test("test_encryption_with_shift_value_26", "xyz", "yza"); // لا يدعم shift 26، دائماً 1
    run_test("test_encryption_empty_string", "", "");
    run_test("test_encryption_non_alpha_characters_only", "1234!@#$", "1234!@#$"); // لا يتغير

    return 0;
}
