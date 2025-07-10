#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OUTPUT 1024

// دالة تحاكي قراءة إدخال كـ string وتحويله لعدد صحيح مع التحقق
int parse_int(const char* input, int* number) {
    char *endptr;
    long val = strtol(input, &endptr, 10);
    // تحقق من أن التحويل تم بنجاح وأن باقي النص هو فقط newline أو نهاية السلسلة
    if (endptr == input) return 0; // لم يتم تحويل أي رقم
    while (*endptr == ' ') endptr++;
    if (*endptr != '\0' && *endptr != '\n') return 0;
    if (val < INT_MIN || val > INT_MAX) return 0;
    *number = (int)val;
    return 1;
}

// دالة تحاكي تشغيل البرنامج مع إدخال معين وتحفظ الناتج في buffer
int run_test(const char* input, char* output, size_t max_output) {
    int number;
    output[0] = '\0';

    if (!parse_int(input, &number)) {
        snprintf(output, max_output, "Invalid input. Please enter a number.\n");
        return 0;
    }

    int squared = number * number;

    snprintf(output, max_output,
             "You entered: %d\nSquare of the number: %d\n",
             number, squared);

    return 1;
}

int main() {
    struct {
        const char* input;
        const char* expected_output;
        const char* description;
    } tests[] = {
        {"10\n", "You entered: 10\nSquare of the number: 100\n", "Test 1: valid positive number"},
        {"-5\n", "You entered: -5\nSquare of the number: 25\n", "Test 2: valid negative number"},
        {"0\n", "You entered: 0\nSquare of the number: 0\n", "Test 3: zero input"},
        {"123abc\n", "Invalid input. Please enter a number.\n", "Test 4: invalid mixed input"},
        {"abc\n", "Invalid input. Please enter a number.\n", "Test 5: invalid non-numeric input"},
        {"9999999999\n", "Invalid input. Please enter a number.\n", "Test 6: number too large"},
        {" 42 \n", "You entered: 42\nSquare of the number: 1764\n", "Test 7: input with spaces"},
    };

    int passed = 0;
    int total = sizeof(tests) / sizeof(tests[0]);
    char output[MAX_OUTPUT];

    for (int i = 0; i < total; i++) {
        int ret = run_test(tests[i].input, output, sizeof(output));
        printf("%s... ", tests[i].description);
        if ((ret && strcmp(output, tests[i].expected_output) == 0) ||
            (!ret && strcmp(output, tests[i].expected_output) == 0)) {
            printf("PASS\n");
            passed++;
        } else {
            printf("FAIL\nExpected:\n%sGot:\n%s\n", tests[i].expected_output, output);
        }
    }

    printf("Passed %d/%d tests.\n", passed, total);

    return 0;
}
