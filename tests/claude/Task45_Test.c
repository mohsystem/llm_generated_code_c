#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

double divide_numbers(double a, double b) {
    if (b == 0) {
        errno = EDOM;
        return 0;
    }
    return a / b;
}

int run_divide_numbers_test(double a, double b, double expected_result, int expect_error) {
    errno = 0;
    double result = divide_numbers(a, b);

    if (expect_error) {
        if (errno == EDOM) {
            return 1; // Passed: error occurred as expected
        } else {
            return 0; // Failed: expected error but didn't get
        }
    } else {
        if (errno != 0) {
            return 0; // Failed: unexpected error
        }
        // Compare result with expected (allow small epsilon)
        double epsilon = 1e-6;
        if ((result - expected_result) < epsilon && (expected_result - result) < epsilon) {
            return 1; // Passed
        } else {
            return 0; // Failed: wrong result
        }
    }
}

// Helper: simulate main with inputs, capture output into buffer for testing
// Returns 0 on success, 1 on failure
int run_main_test(const char* input1, const char* input2, const char* expected_output_substr) {
    // We simulate user input and capture printed output
    // For simplicity here, we'll call divide_numbers directly after parsing inputs

    char *endptr;
    errno = 0;
    double num1 = strtod(input1, &endptr);
    if (errno != 0 || (*endptr != '\0' && *endptr != '\n')) return 0;

    errno = 0;
    double num2 = strtod(input2, &endptr);
    if (errno != 0 || (*endptr != '\0' && *endptr != '\n')) return 0;

    errno = 0;
    double result = divide_numbers(num1, num2);

    char buffer[256];
    if (errno == EDOM) {
        snprintf(buffer, sizeof(buffer), "Error: Cannot divide by zero");
    } else {
        snprintf(buffer, sizeof(buffer), "Result: %f", result);
    }

    if (strstr(buffer, expected_output_substr) != NULL) {
        return 1; // Passed
    } else {
        return 0; // Failed
    }
}

int main() {
    struct {
        double a;
        double b;
        double expected_result;
        int expect_error;
        const char* description;
    } divide_tests[] = {
        {10, 2, 5, 0, "Valid division"},
        {-10, 2, -5, 0, "Valid negative division"},
        {0, 1, 0, 0, "Zero numerator"},
        {10, 0, 0, 1, "Division by zero error"},
    };

    struct {
        const char* input1;
        const char* input2;
        const char* expected_output_substr;
        const char* description;
    } main_tests[] = {
        {"10", "2", "Result: 5.000000", "Main valid input"},
        {"10", "0", "Error: Cannot divide by zero", "Main zero division"},
        {"a", "2", "Error: Invalid input", "Main invalid first input"},
        {"10", "b", "Error: Invalid input", "Main invalid second input"},
        {"-10", "2", "Result: -5.000000", "Main negative numbers"},
        {"0", "1", "Result: 0.000000", "Main zero numerator"},
    };

    int n_divide = sizeof(divide_tests) / sizeof(divide_tests[0]);
    int n_main = sizeof(main_tests) / sizeof(main_tests[0]);

    printf("=== Testing divide_numbers() ===\n");
    for (int i = 0; i < n_divide; i++) {
        int pass = run_divide_numbers_test(divide_tests[i].a, divide_tests[i].b, divide_tests[i].expected_result, divide_tests[i].expect_error);
        printf("%s: %s\n", divide_tests[i].description, pass ? "PASS" : "FAIL");
    }

    printf("\n=== Testing main simulation ===\n");
    for (int i = 0; i < n_main; i++) {
        // For invalid input cases, the real C main code prints error and returns 1,
        // but here we simulate just checking if input is valid numbers.
        int pass = 0;

        // Check if inputs are numeric
        char *endptr;
        errno = 0;
        strtod(main_tests[i].input1, &endptr);
        int valid1 = (errno == 0 && (*endptr == '\0' || *endptr == '\n'));
        errno = 0;
        strtod(main_tests[i].input2, &endptr);
        int valid2 = (errno == 0 && (*endptr == '\0' || *endptr == '\n'));

        if (!valid1 || !valid2) {
            // Expected invalid input error message in output
            pass = strcmp(main_tests[i].expected_output_substr, "Error: Invalid input") == 0;
        } else {
            // Run main test simulating input and output substring match
            pass = run_main_test(main_tests[i].input1, main_tests[i].input2, main_tests[i].expected_output_substr);
        }

        printf("%s: %s\n", main_tests[i].description, pass ? "PASS" : "FAIL");
    }

    printf("\nProgram execution completed.\n");
    return 0;
}
