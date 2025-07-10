#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OUTPUT 2048

// دالة لمعالجة الإدخال (مطابقة للكود المقدم)
void processInput(const char* input) {
    printf("%s\n", input);
}

// دالة اختبار تستخدم نسخ buffer لاختبار processInput
int run_test(const char* test_input, const char* expected_output, const char* test_name) {
    // نعيد توجيه stdout مؤقتا إلى ملف مؤقت داخل الذاكرة باستخدام freopen
    // لكن لتبسيط الكود، سنستخدم خادع صغير: نستعمل ملف مؤقت

    char filename[] = "temp_test_output_XXXXXX";
    int fd = mkstemp(filename);
    if (fd == -1) {
        printf("%s: FAIL (cannot create temp file)\n", test_name);
        return 0;
    }
    FILE* tmpfile = fdopen(fd, "w+");
    if (!tmpfile) {
        printf("%s: FAIL (cannot open temp file)\n", test_name);
        return 0;
    }

    // حفظ مؤشر stdout الحالي
    fflush(stdout);
    FILE* orig_stdout = stdout;

    // استبدال stdout بالملف المؤقت
    stdout = tmpfile;

    // استدعاء الدالة المراد اختبارها
    processInput(test_input);

    fflush(stdout);
    rewind(tmpfile);

    // قراءة المخرجات
    char output[MAX_OUTPUT] = {0};
    fread(output, 1, MAX_OUTPUT - 1, tmpfile);

    // استرجاع stdout
    stdout = orig_stdout;
    fclose(tmpfile);

    // حذف الملف المؤقت
    remove(filename);

    // مقارنة الناتج مع المتوقع
    if (strcmp(output, expected_output) == 0) {
        printf("%s: PASS\n", test_name);
        return 1;
    } else {
        printf("%s: FAIL\nExpected:\n%s\nGot:\n%s\n", test_name, expected_output, output);
        return 0;
    }
}

int main() {
    run_test("Hello, world!", "Hello, world!\n", "Test 1: Normal input");
    run_test("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
             "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n",
             "Test 2: Input exactly 100 chars");
    run_test("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
             "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n",
             "Test 3: Input longer than 100 chars");
    run_test("", "\n", "Test 4: Empty input");
    run_test("@#$%^&*()", "@#$%^&*()\n", "Test 5: Special characters");
    run_test("1234567890", "1234567890\n", "Test 6: Numeric input");
    run_test("Hello123", "Hello123\n", "Test 7: Mixed alphanumeric input");
    run_test("     ", "     \n", "Test 8: Whitespace input");
    run_test("Hello\nWorld", "Hello\nWorld\n", "Test 9: Input with newline");
    run_test("  Hello  ", "  Hello  \n", "Test 10: Leading/trailing spaces");

    return 0;
}
