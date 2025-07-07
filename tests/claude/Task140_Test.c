#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

// دالة الحساب
// ترجع 0 لو العملية ناجحة، و-1 لو خطأ division by zero، و-2 لو خطأ invalid operator
int calc(double num1, double num2, char operator, double *result, char *error_msg) {
    switch (operator) {
        case '+':
            *result = num1 + num2;
            return 0;
        case '-':
            *result = num1 - num2;
            return 0;
        case '*':
            *result = num1 * num2;
            return 0;
        case '/':
            if (num2 == 0) {
                strcpy(error_msg, "Error: Division by zero");
                return -1;
            }
            *result = num1 / num2;
            return 0;
        default:
            strcpy(error_msg, "Error: Invalid operator");
            return -2;
    }
}

// مقارنة أعداد عائمة بدقة
int double_equals(double a, double b, double epsilon) {
    return fabs(a - b) < epsilon;
}

void run_tests() {
    double res;
    char error[50];
    int status;
    double epsilon = 1e-9;

    // test addition
    status = calc(3, 5, '+', &res, error);
    assert(status == 0);
    assert(double_equals(res, 8.0, epsilon));

    // test subtraction
    status = calc(10, 4, '-', &res, error);
    assert(status == 0);
    assert(double_equals(res, 6.0, epsilon));

    // test multiplication
    status = calc(7, 6, '*', &res, error);
    assert(status == 0);
    assert(double_equals(res, 42.0, epsilon));

    // test division
    status = calc(9, 3, '/', &res, error);
    assert(status == 0);
    assert(double_equals(res, 3.0, epsilon));

    // test division by zero
    status = calc(5, 0, '/', &res, error);
    assert(status == -1);
    assert(strcmp(error, "Error: Division by zero") == 0);

    // test invalid operator
    status = calc(5, 3, '^', &res, error);
    assert(status == -2);
    assert(strcmp(error, "Error: Invalid operator") == 0);

    // test negative numbers addition
    status = calc(-3, -7, '+', &res, error);
    assert(status == 0);
    assert(double_equals(res, -10.0, epsilon));

    // test negative numbers subtraction
    status = calc(-10, -5, '-', &res, error);
    assert(status == 0);
    assert(double_equals(res, -5.0, epsilon));

    // test negative numbers multiplication
    status = calc(-4, 5, '*', &res, error);
    assert(status == 0);
    assert(double_equals(res, -20.0, epsilon));

    // test negative numbers division
    status = calc(-10, 2, '/', &res, error);
    assert(status == 0);
    assert(double_equals(res, -5.0, epsilon));

    // test float numbers addition
    status = calc(5.5, 2.2, '+', &res, error);
    assert(status == 0);
    assert(double_equals(res, 7.7, epsilon));

    // test float numbers multiplication
    status = calc(5.5, 2.2, '*', &res, error);
    assert(status == 0);
    assert(double_equals(res, 5.5 * 2.2, epsilon));

    printf("✓ All tests passed for calculator.\n");
}

int main() {
    run_tests();
    return 0;
}
