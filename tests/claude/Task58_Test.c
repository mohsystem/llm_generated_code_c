#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

bool XO(const char* str)
{
    int x_count = 0, o_count = 0;
    for (int i = 0; str[i]; i++) {
        char c = tolower(str[i]);
        if (c == 'x') x_count++;
        else if (c == 'o') o_count++;
    }
    return x_count == o_count;
}

int main() {
    struct {
        const char* input;
        bool expected;
        const char* test_name;
    } tests[] = {
        {"ooxx", true, "test_all_x_and_o"},
        {"xooxx", false, "test_more_x_than_o"},
        {"xxoo", true, "test_more_o_than_x"},
        {"ooxXm", true, "test_case_insensitive"},
        {"zpzpzpp", true, "test_no_x_no_o"},
        {"zzoo", false, "test_no_x_with_o"},
        {"xxxx", false, "test_no_o_with_x"},
        {"", true, "test_empty_string"},
        {"o", false, "test_single_character_o"},
        {"x", false, "test_single_character_x"},
    };

    int pass = 0, fail = 0;
    int total_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < total_tests; i++) {
        bool result = XO(tests[i].input);
        if (result == tests[i].expected) {
            printf("Test %d (%s): PASS\n", i+1, tests[i].test_name);
            pass++;
        } else {
            printf("Test %d (%s): FAIL\n", i+1, tests[i].test_name);
            fail++;
        }
    }

    printf("\nTotal Passed: %d\nTotal Failed: %d\n", pass, fail);
    return 0;
}
