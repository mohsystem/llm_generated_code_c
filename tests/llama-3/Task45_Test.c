#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Function to simulate running division operation and returning output message
// Returns 0 on success, 1 on error
// Output message stored in output_buffer (must be large enough)
int run_division(const char* input_num1, const char* input_num2, char* output_buffer, size_t bufsize) {
    char* endptr;
    double num1 = strtod(input_num1, &endptr);
    if (*endptr != '\0') {
        snprintf(output_buffer, bufsize, "Error: Invalid input!\n");
        return 1;
    }
    double num2 = strtod(input_num2, &endptr);
    if (*endptr != '\0') {
        snprintf(output_buffer, bufsize, "Error: Invalid input!\n");
        return 1;
    }
    if (num2 == 0) {
        snprintf(output_buffer, bufsize, "Error: Division by zero!\n");
        return 1;
    }
    snprintf(output_buffer, bufsize, "Result: %f\n", num1 / num2);
    return 0;
}

// Helper function to compare strings ignoring trailing whitespace differences
int compare_output(const char* got, const char* expected) {
    size_t len_got = strlen(got);
    size_t len_exp = strlen(expected);
    while (len_got > 0 && (got[len_got -1] == '\n' || got[len_got -1] == ' ')) len_got--;
    while (len_exp > 0 && (expected[len_exp -1] == '\n' || expected[len_exp -1] == ' ')) len_exp--;
    if (len_got != len_exp) return 0;
    for (size_t i = 0; i < len_got; i++) {
        if (got[i] != expected[i]) return 0;
    }
    return 1;
}

#define PRINT_RESULT(test_num, passed) printf("Test case %d: %s\n", test_num, (passed) ? "PASS" : "FAIL")

void test_case_1() {
    char output[100];
    int pass = 1;
    if (run_division("10", "2", output, sizeof(output)) != 0 || !compare_output(output, "Result: 5.000000\n")) pass = 0;
    if (run_division("-10", "2", output, sizeof(output)) != 0 || !compare_output(output, "Result: -5.000000\n")) pass = 0;
    if (run_division("0", "1", output, sizeof(output)) != 0 || !compare_output(output, "Result: 0.000000\n")) pass = 0;
    PRINT_RESULT(1, pass);
}

void test_case_2() {
    char output[100];
    int ret = run_division("10", "0", output, sizeof(output));
    PRINT_RESULT(2, (ret != 0) && compare_output(output, "Error: Division by zero!\n"));
}

void test_case_3() {
    char output[100];
    int pass = 1;
    if (run_division("10a", "2", output, sizeof(output)) == 0) pass = 0;
    if (run_division("10", "2b", output, sizeof(output)) == 0) pass = 0;
    PRINT_RESULT(3, pass);
}

void test_case_4() {
    char output[100];
    int ret = run_division("10", "2", output, sizeof(output));
    PRINT_RESULT(4, (ret == 0) && compare_output(output, "Result: 5.000000\n"));
}

void test_case_5() {
    char output[100];
    int ret = run_division("10", "0", output, sizeof(output));
    PRINT_RESULT(5, (ret != 0) && compare_output(output, "Error: Division by zero!\n"));
}

void test_case_6() {
    char output[100];
    int ret = run_division("a", "2", output, sizeof(output));
    PRINT_RESULT(6, (ret != 0) && compare_output(output, "Error: Invalid input!\n"));
}

void test_case_7() {
    char output[100];
    int ret = run_division("10", "2", output, sizeof(output));
    PRINT_RESULT(7, (ret == 0) && compare_output(output, "Result: 5.000000\n"));
}

void test_case_8() {
    char output[100];
    int ret = run_division("-10", "2", output, sizeof(output));
    PRINT_RESULT(8, (ret == 0) && compare_output(output, "Result: -5.000000\n"));
}

void test_case_9() {
    char output[100];
    int ret = run_division("0", "1", output, sizeof(output));
    PRINT_RESULT(9, (ret == 0) && compare_output(output, "Result: 0.000000\n"));
}

void test_case_10() {
    char output[100];
    int pass = 1;
    for (int i = 0; i < 10; i++) {
        int ret = run_division("10", "2", output, sizeof(output));
        if (ret != 0 || !compare_output(output, "Result: 5.000000\n")) {
            pass = 0;
            break;
        }
    }
    PRINT_RESULT(10, pass);
}

void test_case_11() {
    char output[100];
    int ret = run_division("1", "0", output, sizeof(output));
    PRINT_RESULT(11, (ret != 0) && compare_output(output, "Error: Division by zero!\n"));
}

int main() {
    test_case_1();
    test_case_2();
    test_case_3();
    test_case_4();
    test_case_5();
    test_case_6();
    test_case_7();
    test_case_8();
    test_case_9();
    test_case_10();
    test_case_11();
    return 0;
}
