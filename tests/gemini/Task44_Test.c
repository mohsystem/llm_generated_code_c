#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT 256

// دالة لإجراء الإدخال الآمن لعدد صحيح مع التحقق
int getIntegerInputFromString(const char* inputStr, int* value) {
    char *endptr;
    long val = strtol(inputStr, &endptr, 10);
    // تحقق من أن الإدخال كامل وعدد صحيح صالح
    while (*endptr == ' ') endptr++; // تخطي مسافات بيضاء في النهاية
    if (*endptr != '\0' && *endptr != '\n') {
        return 0; // ليس عدد صحيح صالح
    }
    if (val < INT_MIN || val > INT_MAX) {
        return 0; // خارج نطاق int
    }
    *value = (int)val;
    return 1;
}

// دالة اختبار تُحاكي إدخال المستخدم وتتحقق من النتيجة
int run_test(const char* input1, const char* input2, int expected_sum, int expected_diff, int expected_prod) {
    int val1, val2;
    int valid1 = getIntegerInputFromString(input1, &val1);
    int valid2 = getIntegerInputFromString(input2, &val2);

    printf("Input1: '%s', Input2: '%s' => ", input1, input2);

    if (!valid1 || !valid2) {
        printf("FAIL (Invalid input)\n");
        return 0;
    }

    int sum = val1 + val2;
    int diff = val1 - val2;
    int prod = val1 * val2;

    if (sum == expected_sum && diff == expected_diff && prod == expected_prod) {
        printf("PASS\n");
        return 1;
    } else {
        printf("FAIL\n");
        printf("Expected sum: %d, got: %d\n", expected_sum, sum);
        printf("Expected diff: %d, got: %d\n", expected_diff, diff);
        printf("Expected prod: %d, got: %d\n", expected_prod, prod);
        return 0;
    }
}

int main() {
    struct {
        const char* input1;
        const char* input2;
        int expected_sum;
        int expected_diff;
        int expected_prod;
        const char* description;
    } tests[] = {
        {"10\n", "5\n", 15, 5, 50, "Test 1: normal positive integers"},
        {"-10\n", "5\n", -5, -15, -50, "Test 2: negative and positive integer"},
        {"0\n", "0\n", 0, 0, 0, "Test 3: zeros"},
        {"2147483647\n", "1\n", 2147483648, 2147483646, 2147483647, "Test 4: int max overflow edge case (expected to fail in C int)"},
        {"abc\n", "10\n", 0, 0, 0, "Test 5: invalid first input"},
        {"10\n", "xyz\n", 0, 0, 0, "Test 6: invalid second input"},
        {"   15\n", "   25\n", 40, -10, 375, "Test 7: inputs with leading spaces"},
        {"-123\n", "-456\n", -579, 333, 56088, "Test 8: negative integers"},
        {"999999\n", "999999\n", 1999998, 0, 999998000001, "Test 9: large numbers"},
    };

    int passed = 0;
    int total = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < total; i++) {
        if (run_test(tests[i].input1, tests[i].input2, tests[i].expected_sum, tests[i].expected_diff, tests[i].expected_prod)) {
            passed++;
        }
    }

    printf("Passed %d out of %d tests.\n", passed, total);

    return 0;
}
