#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LENGTH 100

// هذه هي دالة التحويل كما في الكود الأصلي
int try_convert_integer(const char* input, char* output, size_t out_size) {
    char buffer[MAX_INPUT_LENGTH];
    strncpy(buffer, input, MAX_INPUT_LENGTH - 1);
    buffer[MAX_INPUT_LENGTH - 1] = '\0';

    char *endptr;
    long number = strtol(buffer, &endptr, 10);

    if (*endptr == '\0' && strlen(buffer) > 0) {
        snprintf(output, out_size, "Successfully converted to integer: %ld\n", number);
        return 1; // success
    } else {
        snprintf(output, out_size, "Invalid input. Please enter a valid integer.\n");
        return 0; // failure
    }
}

// دالة لتشغيل التست كيسز الصارمة
void run_tests() {
    struct {
        const char* input;
        const char* expected_output;
        int expected_success;
    } tests[] = {
        {"123", "Successfully converted to integer: 123\n", 1},
        {"0", "Successfully converted to integer: 0\n", 1},
        {"-42", "Successfully converted to integer: -42\n", 1},
        {"  56  ", "Invalid input. Please enter a valid integer.\n", 0}, // spaces cause fail because of endptr
        {"abc", "Invalid input. Please enter a valid integer.\n", 0},
        {"12abc34", "Invalid input. Please enter a valid integer.\n", 0},
        {"", "Invalid input. Please enter a valid integer.\n", 0},
        {"2147483647", "Successfully converted to integer: 2147483647\n", 1}, // max 32bit int
        {"-2147483648", "Successfully converted to integer: -2147483648\n", 1},
    };

    int num_tests = sizeof(tests)/sizeof(tests[0]);
    char output[256];
    int pass_count = 0;

    for (int i = 0; i < num_tests; i++) {
        int result = try_convert_integer(tests[i].input, output, sizeof(output));
        if (strcmp(output, tests[i].expected_output) == 0 && result == tests[i].expected_success) {
            printf("Test case %d: PASS\n", i+1);
            pass_count++;
        } else {
            printf("Test case %d: FAIL\nExpected: %sGot: %s\n", i+1, tests[i].expected_output, output);
        }
    }
    printf("\nSummary: %d/%d tests passed.\n", pass_count, num_tests);
}

int main() {
    run_tests();
    return 0;
}
