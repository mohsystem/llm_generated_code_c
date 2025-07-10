#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// الدالة الأصلية كما هي
void processString(const char* inputString) {
    printf("Processed string: %s", inputString);
}

// نسخة خاصة بالاختبارات تقوم بتجميع الناتج في buffer بدلاً من الطباعة
void processString_test(const char* inputString, char* buffer, size_t max_len) {
    snprintf(buffer, max_len, "Processed string: %s", inputString);
}

// دالة الاختبار
int run_test(const char* input, const char* expected_output, const char* test_name) {
    char output_buffer[1024];
    processString_test(input, output_buffer, sizeof(output_buffer));

    if (strcmp(output_buffer, expected_output) == 0) {
        printf("%s: PASS\n", test_name);
        return 1;
    } else {
        printf("%s: FAIL\nExpected:\n%s\nGot:\n%s\n", test_name, expected_output, output_buffer);
        return 0;
    }
}

int main() {
    // حالات الاختبار مع مراعاة أن input قد تحتوي '\n' ويجب أن تكون موجودة في expected_output
    run_test("Hello, world!\n", "Processed string: Hello, world!\n", "Test 1: Normal input");
    run_test("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n",
             "Processed string: aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n",
             "Test 2: Input exactly 100 chars");
    run_test("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n",
             "Processed string: aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n",
             "Test 3: Input longer than 100 chars");
    run_test("\n", "Processed string: \n", "Test 4: Empty input");
    run_test("@#$%^&*()\n", "Processed string: @#$%^&*()\n", "Test 5: Special characters");
    run_test("1234567890\n", "Processed string: 1234567890\n", "Test 6: Numeric input");
    run_test("Hello123\n", "Processed string: Hello123\n", "Test 7: Mixed alphanumeric input");
    run_test("     \n", "Processed string:      \n", "Test 8: Whitespace input");
    run_test("Hello\nWorld\n", "Processed string: Hello\nWorld\n", "Test 9: Input with newline");
    run_test("  Hello  \n", "Processed string:   Hello  \n", "Test 10: Leading/trailing spaces");

    return 0;
}
