#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


// #include "../../source/output/claude/c/Task4_CLAUDE_claude-3-5-sonnet-20240620.c"  // Include the header for the function
// #include "../../source/output/codestral/c/Task4_MISTRAL_codestral-latest.c"  // Include the header for the function
// #include "../../source/output/gemini/c/Task4_GEMINI_gemini-1.5-pro-001.c"  // Include the header for the function
#include "../../source/output/gpt4o/c/Task4_OPENAI_gpt-4o.c"  // Include the header for the function
// #include "../../source/output/llama-3/c/Task4_PERPLEXITY_llama-3-sonar-large-32k-chat.c"  // Include the header for the function
// Helper function to run a test case


void run_test_case(const char *input, bool expected, const char *test_name) {
    bool result = isIsogram(input);
    assert(result == expected && "Test case failed");
    printf("%s passed: %s\n", test_name, result ? "true" : "false");
}

int main() {
    // Test Case 1: Basic isogram test
    run_test_case("Dermatoglyphics", true, "Test Case 1");

    // Test Case 2: Repeating letter
    run_test_case("aba", false, "Test Case 2");

    // Test Case 3: Case-insensitive repeating letter
    run_test_case("moOse", false, "Test Case 3");

    // Test Case 4: All lowercase isogram
    run_test_case("isogram", true, "Test Case 4");

    // Test Case 5: Mixed case isogram
    run_test_case("Machine", true, "Test Case 5");

    // Test Case 6: Edge case - Empty string
    run_test_case("", true, "Test Case 6");

    // Test Case 7: Consecutive repeating letter
    run_test_case("letter", false, "Test Case 7");

    // Test Case 8: Multiple repeating letters
    run_test_case("Bookkeeper", false, "Test Case 8");

    // Test Case 9: No repeating letters, mixed case
    run_test_case("Alphabet", false, "Test Case 9");

    // Test Case 10: Edge case - String with hyphen
    run_test_case("a-c", true, "Test Case 10");

    printf("All test cases passed!\n");
    return 0;
}