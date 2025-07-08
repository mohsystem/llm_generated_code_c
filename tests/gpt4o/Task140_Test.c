#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

int calculate(double a, double b, char operator, double *result, char *error_msg) {
    switch (operator) {
        case '+':
            *result = a + b;
            return 0;
        case '-':
            *result = a - b;
            return 0;
        case '*':
            *result = a * b;
            return 0;
        case '/':
            if (b == 0) {
                strcpy(error_msg, "Error: Division by zero");
                return -1;
            }
            *result = a / b;
            return 0;
        default:
            strcpy(error_msg, "Invalid operator");
            return -2;
    }
}

int double_equals(double x, double y, double epsilon) {
    return fabs(x - y) < epsilon;
}

void run_tests() {
    double res;
    char err[50];
    int status;
    double eps = 1e-9;

    status = calculate(3, 5, '+', &res, err);
    assert(status == 0 && double_equals(res, 8.0, eps));

    status = calculate(10, 4, '-', &res, err);
    assert(status == 0 && double_equals(res, 6.0, eps));

    status = calculate(7, 6, '*', &res, err);
    assert(status == 0 && double_equals(res, 42.0, eps));

    status = calculate(9, 3, '/', &res, err);
    assert(status == 0 && double_equals(res, 3.0, eps));

    status = calculate(5, 0, '/', &res, err);
    assert(status == -1 && strcmp(err, "Error: Division by zero") == 0);

    status = calculate(5, 3, '^', &res, err);
    assert(status == -2 && strcmp(err, "Invalid operator") == 0);

    status = calculate(-3, -7, '+', &res, err);
    assert(status == 0 && double_equals(res, -10.0, eps));

    status = calculate(-10, -5, '-', &res, err);
    assert(status == 0 && double_equals(res, -5.0, eps));

    status = calculate(-4, 5, '*', &res, err);
    assert(status == 0 && double_equals(res, -20.0, eps));

    status = calculate(-10, 2, '/', &res, err);
    assert(status == 0 && double_equals(res, -5.0, eps));

    status = calculate(5.5, 2.2, '+', &res, err);
    assert(status == 0 && double_equals(res, 7.7, eps));

    status = calculate(5.5, 2.2, '*', &res, err);
    assert(status == 0 && double_equals(res, 5.5 * 2.2, eps));

    printf("✓ All tests passed for calculate function.\n");
}

int main() {
    run_tests();
    return 0;
}
