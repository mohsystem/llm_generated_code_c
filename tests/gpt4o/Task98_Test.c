#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 50

int try_strtol_conversion(const char* input, char* output, size_t out_size) {
    char buffer[MAX_INPUT];
    strncpy(buffer, input, MAX_INPUT - 1);
    buffer[MAX_INPUT - 1] = '\0';

    char *endptr;
    long val = strtol(buffer, &endptr, 10);

    // نتحقق:
    // 1. أن هناك جزء تم تحويله (endptr != buffer)
    // 2. أن ما بعد الرقم مباشرة هو '\n' (نهاية السطر)
    if (endptr != buffer && *endptr == '\n') {
        snprintf(output, out_size, "Converted Integer: %ld\n", val);
        return 1;
    } else {
        snprintf(output, out_size, "Error: Not a valid integer\n");
        return 0;
    }
}

void run_tests() {
    struct {
        const char* input;
        const char* expected_output;
        int expected_pass;
    } tests[] = {
        {"123\n", "Converted Integer: 123\n", 1},
        {"0\n", "Converted Integer: 0\n", 1},
        {"-42\n", "Converted Integer: -42\n", 1},
        {"abc\n", "Error: Not a valid integer\n", 0},
        {"12abc\n", "Error: Not a valid integer\n", 0},
        {"\n", "Error: Not a valid integer\n", 0},
        {" 45\n", "Error: Not a valid integer\n", 0},
        {"   \n", "Error: Not a valid integer\n", 0},
    };

    int n = sizeof(tests) / sizeof(tests[0]);
    char output[128];
    int pass_count = 0;

    for (int i = 0; i < n; i++) {
        int res = try_strtol_conversion(tests[i].input, output, sizeof(output));
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
