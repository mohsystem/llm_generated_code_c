#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 256

int try_convert_and_check(const char* input, char* output, size_t out_size) {
    char buffer[MAX_INPUT];
    strncpy(buffer, input, MAX_INPUT - 1);
    buffer[MAX_INPUT - 1] = '\0';

    char *endptr;
    long val = strtol(buffer, &endptr, 10);

    // يجب أن يشير endptr إلى نهاية السطر \n لإدخال صحيح
    if (endptr == buffer || *endptr != '\n') {
        snprintf(output, out_size, "Invalid input. Please enter an integer.\n");
        return 0; // fail
    } else {
        snprintf(output, out_size, "You entered: %ld\n", val);
        return 1; // success
    }
}

void run_tests() {
    struct {
        const char* input;
        const char* expected_output;
        int expected_pass;
    } tests[] = {
        {"123\n", "You entered: 123\n", 1},
        {"0\n", "You entered: 0\n", 1},
        {"-42\n", "You entered: -42\n", 1},
        {"abc\n", "Invalid input. Please enter an integer.\n", 0},
        {"12abc\n", "Invalid input. Please enter an integer.\n", 0},
        {" 123\n", "Invalid input. Please enter an integer.\n", 0},
        {"\n", "Invalid input. Please enter an integer.\n", 0},
        {"999999999999999999999\n", "You entered: 999999999999999999999\n", 1}, // long number, test boundary
    };

    int n = sizeof(tests) / sizeof(tests[0]);
    char output[512];
    int pass_count = 0;

    for (int i = 0; i < n; i++) {
        int res = try_convert_and_check(tests[i].input, output, sizeof(output));
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

