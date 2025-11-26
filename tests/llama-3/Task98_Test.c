#include <stdio.h>
#include <string.h>

#define MAX_INPUT 100

// Simulate reading input and processing with scanf logic
int try_scanf_conversion(const char* input, char* output, size_t out_size) {
    FILE* stream = fmemopen((void*)input, strlen(input), "r");
    if (!stream) {
        snprintf(output, out_size, "Error opening stream\n");
        return 0;
    }

    int num;
    int result = fscanf(stream, "%d", &num);
    if (result != 1) {
        snprintf(output, out_size, "That's not a valid number!\n");
        // Clear input buffer simulation is unnecessary here
        fclose(stream);
        return 0;
    } else {
        snprintf(output, out_size, "You entered: %d\n", num);
        fclose(stream);
        return 1;
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
        {"abc\n", "That's not a valid number!\n", 0},
        {"12abc\n", "You entered: 12\n", 1}, // scanf reads 12 and stops at 'a'
        {"\n", "That's not a valid number!\n", 0},
        {" 45\n", "You entered: 45\n", 1},  // scanf ignores leading spaces
        {"   \n", "That's not a valid number!\n", 0},
    };

    int n = sizeof(tests) / sizeof(tests[0]);
    char output[128];
    int pass_count = 0;

    for (int i = 0; i < n; i++) {
        int res = try_scanf_conversion(tests[i].input, output, sizeof(output));
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
