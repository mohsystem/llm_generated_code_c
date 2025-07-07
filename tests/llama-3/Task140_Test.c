#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

int calc(double num1, double num2, char operator_, double *result, char *error_msg) {
    switch (operator_) {
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
                strcpy(error_msg, "Error Division by zero is not allowed.");
                return -1;
            }
            *result = num1 / num2;
            return 0;
        default:
            strcpy(error_msg, "Invalid operator. Please enter either +, -, * or /.");
            return -2;
    }
}

int double_equals(double a, double b, double epsilon) {
    return fabs(a - b) < epsilon;
}

void run_tests() {
    double res;
    char err[100];
    int status;
    double eps = 1e-9;

    status = calc(3, 5, '+', &res, err);
    assert(status == 0 && double_equals(res, 8.0, eps));

    status = calc(10, 4, '-', &res, err);
    assert(status == 0 && double_equals(res, 6.0, eps));

    status = calc(7, 6, '*', &res, err);
    assert(status == 0 && double_equals(res, 42.0, eps));

    status = calc(9, 3, '/', &res, err);
    assert(status == 0 && double_equals(res, 3.0, eps));

    status = calc(5, 0, '/', &res, err);
    assert(status == -1 && strcmp(err, "Error Division by zero is not allowed.") == 0);

    status = calc(5, 3, '^', &res, err);
    assert(status == -2 && strcmp(err, "Invalid operator. Please enter either +, -, * or /.") == 0);

    status = calc(-3, -7, '+', &res, err);
    assert(status == 0 && double_equals(res, -10.0, eps));

    status = calc(-10, -5, '-', &res, err);
    assert(status == 0 && double_equals(res, -5.0, eps));

    status = calc(-4, 5, '*', &res, err);
    assert(status == 0 && double_equals(res, -20.0, eps));

    status = calc(-10, 2, '/', &res, err);
    assert(status == 0 && double_equals(res, -5.0, eps));

    status = calc(5.5, 2.2, '+', &res, err);
    assert(status == 0 && double_equals(res, 7.7, eps));

    status = calc(5.5, 2.2, '*', &res, err);
    assert(status == 0 && double_equals(res, 5.5 * 2.2, eps));

    printf("✓ All tests passed for calculator.\n");
}

int main() {
    run_tests();
    return 0;
}
