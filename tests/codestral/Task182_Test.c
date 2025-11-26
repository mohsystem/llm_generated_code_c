#include <stdio.h>
#include <string.h>
#include <ctype.h>

// دالة تنظيف رقم الهاتف: تحذف كل شيء غير الأرقام، وإذا بدأ بـ '1' و الطول > 10، تحذف الـ '1'
void clean_phone_number(const char* input, char* output) {
    int j = 0;
    // ابدأ بجمع الأرقام فقط
    for (int i = 0; input[i] != '\0'; i++) {
        if (isdigit((unsigned char)input[i])) {
            output[j++] = input[i];
        }
    }
    output[j] = '\0';

    // إذا بدأ الرقم بـ '1' وكان الطول أكبر من 10، احذف أول رقم
    if (output[0] == '1' && strlen(output) > 10) {
        memmove(output, output + 1, strlen(output));
    }
}

int main() {
    struct {
        const char* input;
        const char* expected;
    } tests[] = {
        {"+1 (613)-995-0253", "6139950253"},
        {"613-995-0253", "6139950253"},
        {"1 613 995 0253", "6139950253"},
        {"613.995.0253", "6139950253"},
        {"6139950253", "6139950253"},
        {"  +1  613  995  0253  ", "6139950253"},
        {"(613) 995-0253", "6139950253"},
        {"1-613-995-0253", "6139950253"},
        {"1 613.995 0253", "6139950253"},
        {"613 995 0253", "6139950253"},
    };

    char output[20];
    int passed = 1;
    int n = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < n; i++) {
        clean_phone_number(tests[i].input, output);
        if (strcmp(output, tests[i].expected) != 0) {
            printf("Test %d Failed: Input='%s' Expected='%s' Got='%s'\n", i+1, tests[i].input, tests[i].expected, output);
            passed = 0;
        }
    }

    if (passed) {
        printf("All tests passed!\n");
    }

    return 0;
}
