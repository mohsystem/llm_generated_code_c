#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// The function to divide two integers, returns 0 if success, -1 if division by zero, or -2 if invalid input
int divide_numbers(int a, int b, double *result) {
    if (b == 0) {
        return -1; // division by zero
    }
    *result = (double)a / b;
    return 0;
}

// Validate string if valid integer number (allow leading +/-)
int is_valid_number(const char *str) {
    int i = 0;
    if (str[0] == '-' || str[0] == '+') i = 1;
    if (str[i] == '\0') return 0;
    for (; str[i] != '\0'; i++) {
        if (!isdigit((unsigned char)str[i])) {
            return 0;
        }
    }
    return 1;
}

// Main function to perform input reading, error handling, and division
// Returns:
//   0 = success, result stored in *res
//  -1 = division by zero error
//  -2 = invalid input error
int perform_division_from_input(const char *input1, const char *input2, double *res) {
    int num1, num2;
    if (!is_valid_number(input1) || sscanf(input1, "%d", &num1) != 1) {
        return -2;
    }
    if (!is_valid_number(input2) || sscanf(input2, "%d", &num2) != 1) {
        return -2;
    }
    return divide_numbers(num1, num2, res);
}

// ------------------- Test Cases --------------------

void test_divide_numbers_valid() {
    double result;
    int passed = 1;

    if (divide_numbers(10, 2, &result) != 0 || result != 5.0) passed = 0;
    if (divide_numbers(-10, 2, &result) != 0 || result != -5.0) passed = 0;
    if (divide_numbers(0, 1, &result) != 0 || result != 0.0) passed = 0;

    printf("Test case 1 (divide_numbers_valid): %s\n", passed ? "PASS" : "FAIL");
}

void test_divide_numbers_zero_division() {
    double result;
    int ret = divide_numbers(10, 0, &result);
    printf("Test case 2 (divide_numbers_zero_division): %s\n", (ret == -1) ? "PASS" : "FAIL");
}

void test_divide_numbers_invalid_input() {
    // In C, invalid input is caught before calling divide_numbers, so this test is to simulate wrong types
    // We'll simulate by calling perform_division_from_input with invalid strings

    double result;
    int ret = perform_division_from_input("10a", "2", &result);
    if (ret != -2) {
        printf("Test case 3 (divide_numbers_invalid_input part 1): FAIL\n");
        return;
    }
    ret = perform_division_from_input("10", "2b", &result);
    printf("Test case 3 (divide_numbers_invalid_input part 2): %s\n", (ret == -2) ? "PASS" : "FAIL");
}

// For the main tests, simulate the input/output behavior of main()

void test_main_valid_input() {
    double result;
    int ret = perform_division_from_input("10", "2", &result);
    if (ret == 0 && result == 5.0) {
        printf("Test case 4 (main_valid_input): PASS\n");
    } else {
        printf("Test case 4 (main_valid_input): FAIL\n");
    }
}

void test_main_zero_division() {
    double result;
    int ret = perform_division_from_input("10", "0", &result);
    if (ret == -1) {
        printf("Test case 5 (main_zero_division): PASS\n");
    } else {
        printf("Test case 5 (main_zero_division): FAIL\n");
    }
}

void test_main_invalid_input() {
    double result;
    int ret = perform_division_from_input("a", "2", &result);
    if (ret == -2) {
        printf("Test case 6 (main_invalid_input): PASS\n");
    } else {
        printf("Test case 6 (main_invalid_input): FAIL\n");
    }
}

void test_main_with_invalid_input() {
    // This is similar to test_main_valid_input, but catch exceptions (not applicable in C)
    double result;
    int ret = perform_division_from_input("10", "2", &result);
    if (ret == 0 && result == 5.0) {
        printf("Test case 7 (main_with_invalid_input): PASS\n");
    } else {
        printf("Test case 7 (main_with_invalid_input): FAIL\n");
    }
}

void test_main_negative_numbers() {
    double result;
    int ret = perform_division_from_input("-10", "2", &result);
    if (ret == 0 && result == -5.0) {
        printf("Test case 8 (main_negative_numbers): PASS\n");
    } else {
        printf("Test case 8 (main_negative_numbers): FAIL\n");
    }
}

void test_main_zero_numerator() {
    double result;
    int ret = perform_division_from_input("0", "1", &result);
    if (ret == 0 && result == 0.0) {
        printf("Test case 9 (main_zero_numerator): PASS\n");
    } else {
        printf("Test case 9 (main_zero_numerator): FAIL\n");
    }
}

void test_main_valid_input_multiple_times() {
    int pass = 1;
    for (int i = 0; i < 10; i++) {
        double result;
        int ret = perform_division_from_input("10", "2", &result);
        if (ret != 0 || result != 5.0) {
            pass = 0;
            break;
        }
    }
    printf("Test case 10 (main_valid_input_multiple_times): %s\n", pass ? "PASS" : "FAIL");
}

void test_main_small_numbers_zero_division() {
    double result;
    int ret = perform_division_from_input("1", "0", &result);
    if (ret == -1) {
        printf("Test case 11 (main_small_numbers_zero_division): PASS\n");
    } else {
        printf("Test case 11 (main_small_numbers_zero_division): FAIL\n");
    }
}

int main() {
    test_divide_numbers_valid();
    test_divide_numbers_zero_division();
    test_divide_numbers_invalid_input();
    test_main_valid_input();
    test_main_zero_division();
    test_main_invalid_input();
    test_main_with_invalid_input();
    test_main_negative_numbers();
    test_main_zero_numerator();
    test_main_valid_input_multiple_times();
    test_main_small_numbers_zero_division();
    return 0;
}
