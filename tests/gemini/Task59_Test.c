#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* accum(char* s) {
    int len = strlen(s);
    char* result = malloc(len * (len + 1) / 2 + len + 1);
    if (result == NULL) {
        return NULL;
    }
    int index = 0;
    for (int i = 0; i < len; i++) {
        if (i > 0) {
            result[index++] = '-';
        }
        result[index++] = toupper(s[i]);
        for (int j = 0; j < i; j++) {
            result[index++] = tolower(s[i]);
        }
    }
    result[index] = '\0';
    return result;
}

int main() {
    struct {
        char* input;
        char* expected;
        char* test_name;
    } tests[] = {
        {"abcd", "A-Bb-Ccc-Dddd", "test_all_unique_characters"},
        {"RqaEzty", "R-Qq-Aaa-Eeee-Zzzzz-Tttttt-Yyyyyyy", "test_mixed_case_characters"},
        {"cwAt", "C-Ww-Aaa-Tttt", "test_mixed_case_characters_with_repeating"},
        {"a", "A", "test_single_character_string"},
        {"XYZ", "X-Yy-Zzz", "test_all_uppercase_characters"},
        {"xyz", "X-Yy-Zzz", "test_all_lowercase_characters"},
        {"aAaA", "A-Aa-Aaa-Aaaa", "test_mixed_case_with_repeating_characters"},
        {"", "", "test_empty_string"},
        {"aaabbb", "A-Aa-Aaa-Bbbb-Bbbbb-Bbbbbb", "test_string_with_repeated_characters"},
        {"AaBbCc", "A-Aa-Bbb-Bbbb-Ccccc-Cccccc", "test_case_sensitivity_check"}
    };

    int pass = 0, fail = 0;
    int total_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < total_tests; i++) {
        char* result = accum(tests[i].input);
        if (result && strcmp(result, tests[i].expected) == 0) {
            printf("Test %d (%s): PASS\n", i + 1, tests[i].test_name);
            pass++;
        } else {
            printf("Test %d (%s): FAIL\n", i + 1, tests[i].test_name);
            fail++;
        }
        free(result);
    }

    printf("\nTotal Passed: %d\nTotal Failed: %d\n", pass, fail);
    return 0;
}
