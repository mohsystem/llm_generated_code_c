#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 256

int try_sscanf_conversion(const char* input, char* output, size_t out_size) {
    char buffer[MAX_INPUT];
    strncpy(buffer, input, MAX_INPUT - 1);
    buffer[MAX_INPUT - 1] = '\0';

    int value;
    int matched = sscanf(buffer, "%d", &value);

    // نتحقق إن كل السلسلة تم تحليلها بشكل صحيح:
    // sscanf ترجع عدد العناصر التي تمت قراءتها (1 إذا نجح)
    // لكن قد يكون هناك نص بعد الرقم => لازم نتحقق إن endptr يثبت نهاية السطر

    // عشان نتحقق هذي بشكل دقيق، لازم نستخدم strtol بدل sscanf
    // لكن حسب طلبك نفس كود sscanf الأصلي لذا نفعل فحص بسيط:
    // نبحث إذا ما بعد الرقم مباشرة بعد محاولة sscanf توجد حروف غير مسافات أو نهاية السطر

    // الحل العملي: نبحث بعد الرقم عن أول حرف غير رقمي أو فراغ وبعدها يجب ان يكون نهاية السطر

    // لكن لأن sscanf لا تعطي مؤشر للنهاية، لنحافظ على نفس السلوك
    // لذلك:
    if (matched == 1) {
        // sscanf في هذه الحالة تقرأ الرقم فقط من البداية
        // لكن مثلا "12abc" يتم قراءة 12 فقط => نعتبرها نجاح (كما هو سلوك sscanf)
        snprintf(output, out_size, "Successfully converted to integer: %d\n", value);
        return 1;
    } else {
        snprintf(output, out_size, "Invalid input. Please enter a valid integer.\n");
        return 0;
    }
}

void run_tests() {
    struct {
        const char* input;
        const char* expected_output;
        int expected_pass;
    } tests[] = {
        {"123\n", "Successfully converted to integer: 123\n", 1},
        {"0\n", "Successfully converted to integer: 0\n", 1},
        {"-42\n", "Successfully converted to integer: -42\n", 1},
        {"abc\n", "Invalid input. Please enter a valid integer.\n", 0},
        {"12abc\n", "Successfully converted to integer: 12\n", 1},
        {"\n", "Invalid input. Please enter a valid integer.\n", 0},
        {" 45\n", "Successfully converted to integer: 45\n", 1},
        {"1 2\n", "Successfully converted to integer: 1\n", 1},
        {"123!@\n", "Successfully converted to integer: 123\n", 1},
        {"999999999\n", "Successfully converted to integer: 999999999\n", 1},
        {"   \n", "Invalid input. Please enter a valid integer.\n", 0},
    };

    int n = sizeof(tests) / sizeof(tests[0]);
    char output[512];
    int pass_count = 0;

    for (int i = 0; i < n; i++) {
        int res = try_sscanf_conversion(tests[i].input, output, sizeof(output));
        if (strcmp(output, tests[i].expected_output) == 0 && res == tests[i].expected_pass) {
            printf("Test case %d: PASS\n", i + 1);
            pass_count++;
        } else {
            printf("Test case %d: FAIL\nExpected: %sGot: %s\n", i + 1, tests[i].expected_output, output);
        }
    }
    printf("\nSummary: %d/%d tests passed.\n", pass_count, n);
}

int main() {
    run_tests();
    return 0;
}
