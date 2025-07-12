#include <stdio.h>
#include <ctype.h>
#include <string.h>

void accum(const char *s, char *result) {
    int pos = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        if (i > 0) result[pos++] = '-';
        result[pos++] = toupper(s[i]);
        for (int j = 0; j < i; j++) {
            result[pos++] = tolower(s[i]);
        }
    }
    result[pos] = '\0';
}

int main() {
    struct {
        const char* input;
        const char* expected;
        const char* test_name;
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

    char result[256];
    int pass = 0, fail = 0;
    int total_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < total_tests; i++) {
        accum(tests[i].input, result);
        if (strcmp(result, tests[i].expected) == 0) {
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
