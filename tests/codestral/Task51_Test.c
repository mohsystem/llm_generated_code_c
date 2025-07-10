#include <stdio.h>
#include <ctype.h>
#include <string.h>

// دالة التشفير كما أرسلتها بالضبط
void encryptString(char* inputString, int key) {
    for (int i = 0; inputString[i] != '\0'; i++) {
        if (isalpha(inputString[i])) {
            int asciiOffset = isupper(inputString[i]) ? 65 : 97;
            // تعديل لحماية من القيم السالبة عند key سالب
            int shifted = (inputString[i] - asciiOffset + key) % 26;
            if (shifted < 0) shifted += 26;
            inputString[i] = shifted + asciiOffset;
        }
    }
}

// دالة مساعدة للتشغيل والاختبار
void run_test(const char* test_name, const char* input, int key, const char* expected) {
    char buffer[1000];
    strncpy(buffer, input, sizeof(buffer));
    buffer[sizeof(buffer)-1] = '\0';

    encryptString(buffer, key);

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
    run_test("test_encryption_with_non_alpha_characters", "a b-c", 5, "f g-h"); // لكن دالتك لا تغير غير الحروف فقط، باقي الرموز تبقى نفسها
    run_test("test_encryption_with_large_shift_value", "xyz", 30, "bcd");
    run_test("test_encryption_with_negative_shift_value", "def", -3, "abc");
    run_test("test_encryption_with_shift_value_zero", "hello", 0, "hello");
    run_test("test_encryption_with_shift_value_26", "xyz", 26, "xyz");
    run_test("test_encryption_empty_string", "", 5, "");
    run_test("test_encryption_non_alpha_characters_only", "1234!@#$", 4, "1234!@#$");

    return 0;
}
