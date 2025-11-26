#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


// #include "../../source/output/claude/c/Task4_CLAUDE_claude-3-5-sonnet-20240620.c"  // Include the header for the function
#include "../../source/output/codestral/c/Task4_MISTRAL_codestral-latest.c"  // Include the header for the function
// #include "../../source/output/gemini/c/Task4_GEMINI_gemini-1.5-pro-001.c"  // Include the header for the function
// #include "../../source/output/gpt4o/c/Task4_OPENAI_gpt-4o.c"  // Include the header for the function
// #include "../../source/output/llama-3/c/Task4_PERPLEXITY_llama-3-sonar-large-32k-chat.c"  // Include the header for the function
// Helper function to run a test case


void run_test_case(char *str, int expected, int *passed_tests) {
    int result = isIsogram(str);
    assert(result == expected);
    if (result == expected) {
        (*passed_tests)++;
    }
}

int main() {
    int total_tests = 10;
    int passed_tests = 0;

    // Test Case 1: Basic isogram test
    char str1[] = "Dermatoglyphics";
    run_test_case(str1, 1, &passed_tests);

    // Test Case 2: Repeating letter
    char str2[] = "aba";
    run_test_case(str2, 0, &passed_tests);

    // Test Case 3: Case-insensitive repeating letter
    char str3[] = "moOse";
    run_test_case(str3, 0, &passed_tests);

    // Test Case 4: All lowercase isogram
    char str4[] = "isogram";
    run_test_case(str4, 1, &passed_tests);

    // Test Case 5: Mixed case isogram
    char str5[] = "Machine";
    run_test_case(str5, 1, &passed_tests);

    // Test Case 6: Edge case - Empty string
    char str6[] = "";
    run_test_case(str6, 1, &passed_tests);

    // Test Case 7: Consecutive repeating letter
    char str7[] = "letter";
    run_test_case(str7, 0, &passed_tests);

    // Test Case 8: Multiple repeating letters
    char str8[] = "Bookkeeper";
    run_test_case(str8, 0, &passed_tests);

    // Test Case 9: No repeating letters, mixed case
    char str9[] = "Alphabet";
    run_test_case(str9, 0, &passed_tests);

    // Test Case 10: Edge case - String with hyphen
    char str10[] = "a-c";
    run_test_case(str10, 1, &passed_tests);

    // Calculate success percentage
    double success_percentage = ((double)passed_tests / total_tests) * 100.0;
    printf("Passed %d out of %d tests.\n", passed_tests, total_tests);
    printf("Success rate: %.2f%%\n", success_percentage);

    return 0;
}