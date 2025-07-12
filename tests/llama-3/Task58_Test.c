#include <stdio.h>
#include <string.h>
#include <ctype.h>

int xo(char *s) {
    int xCount = 0, oCount = 0;
    for (int i = 0; i < strlen(s); i++) {
        if (tolower(s[i]) == 'x') xCount++;
        if (tolower(s[i]) == 'o') oCount++;
    }
    return xCount == oCount;
}

int main() {
    struct {
        char* input;
        int expected;
        const char* test_name;
    } tests[] = {
        {"ooxx", 1, "test_all_x_and_o"},
        {"xooxx", 0, "test_more_x_than_o"},
        {"xxoo", 1, "test_more_o_than_x"},
        {"ooxXm", 1, "test_case_insensitive"},
        {"zpzpzpp", 1, "test_no_x_no_o"},
        {"zzoo", 0, "test_no_x_with_o"},
        {"xxxx", 0, "test_no_o_with_x"},
        {"", 1, "test_empty_string"},
        {"o", 0, "test_single_character_o"},
        {"x", 0, "test_single_character_x"}
    };

    int pass = 0, fail = 0;
    int total_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < total_tests; i++) {
        int result = xo(tests[i].input);
        if (result == tests[i].expected) {
            printf("Test %d (%s): PASS\n", i + 1, tests[i].test_name);
            pass++;
        } else {
            printf("Test %d (%s): FAIL\n", i + 1, tests[i].test_name);
            fail++;
        }
    }

    printf("\nTotal Passed: %d\nTotal Failed: %d\n", pass, fail);
    return 0;
}
