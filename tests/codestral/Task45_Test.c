#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_OUTPUT 256

// دالة مساعدة لتشغيل البرنامج داخليًا مع مدخل نصي
// تحاكي قراءة scanf وتلتقط المخرجات
int run_program_with_input(const char* input, char* output, size_t max_output) {
    FILE* in = tmpfile();
    FILE* out = tmpfile();
    if (!in || !out) {
        snprintf(output, max_output, "Failed to open temp files\n");
        return 1;
    }

    // كتابة الإدخال إلى ملف مؤقت
    fputs(input, in);
    rewind(in);

    // توجيه stdin و stdout إلى الملفات المؤقتة
    FILE* old_stdin = stdin;
    FILE* old_stdout = stdout;
    stdin = in;
    stdout = out;

    // كود البرنامج الأصلي محاكاة هنا:
    double number;
    int ret = 0;
    if (scanf("%lf", &number) == 1) {
        double result = number * 2;
        printf("The result is: %.2lf\n", result);
    } else {
        printf("Invalid input. Please enter a number.\n");
        ret = 1;
    }

    fflush(stdout);
    stdout = old_stdout;
    stdin = old_stdin;

    rewind(out);
    size_t read_bytes = fread(output, 1, max_output - 1, out);
    output[read_bytes] = '\0';

    fclose(in);
    fclose(out);
    return ret;
}

int main() {
    struct {
        const char* input;
        const char* expected_output;
        const char* description;
    } tests[] = {
        {"10\n", "The result is: 20.00\n", "Test 1: Integer input"},
        {"3.14\n", "The result is: 6.28\n", "Test 2: Floating point input"},
        {"-5\n", "The result is: -10.00\n", "Test 3: Negative number"},
        {"0\n", "The result is: 0.00\n", "Test 4: Zero input"},
        {"abc\n", "Invalid input. Please enter a number.\n", "Test 5: Invalid non-numeric input"},
        {"12abc\n", "Invalid input. Please enter a number.\n", "Test 6: Partially invalid input"},
        {"\n", "Invalid input. Please enter a number.\n", "Test 7: Empty input"},
        {" 7 \n", "The result is: 14.00\n", "Test 8: Input with spaces"},
    };

    char output[MAX_OUTPUT];
    int n = sizeof(tests) / sizeof(tests[0]);
    for (int i = 0; i < n; i++) {
        int ret = run_program_with_input(tests[i].input, output, sizeof(output));
        printf("%s... ", tests[i].description);
        if (strcmp(output, tests[i].expected_output) == 0) {
            printf("PASS\n");
        } else {
            printf("FAIL\nExpected:\n%sGot:\n%s\n", tests[i].expected_output, output);
        }
    }
    return 0;
}
