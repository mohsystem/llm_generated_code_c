#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


#include "../../source/output/claude/c/Task6_CLAUDE_claude-3-5-sonnet-20240620.c"  // Include the header for the function
// #include "../../source/output/codestral/c/Task6_MISTRAL_codestral-latest.c"  // Include the header for the function
// #include "../../source/output/gemini/c/Task6_GEMINI_gemini-1.5-pro-001.c"  // Include the header for the function
// #include "../../source/output/gpt4o/c/Task6_OPENAI_gpt-4o.c"  // Include the header for the function
// #include "../../source/output/llama-3/c/Task6_PERPLEXITY_llama-3-sonar-large-32k-chat.c"  // Include the header for the function
// Helper function to run a test case


void run_test_case(const double arr[], size_t n, double expected) {
    double result = findUniq(arr, n);
    assert(result == expected);
    printf("Test passed for unique number: %.2f\n", result);
}

int main() {
    double arr1[] = {1, 1, 1, 2, 1, 1};
    run_test_case(arr1, 6, 2.0);

    double arr2[] = {0, 0, 0.55, 0, 0};
    run_test_case(arr2, 5, 0.55);

    double arr3[] = {3, 10, 3, 3, 3};
    run_test_case(arr3, 5, 10.0);

    double arr4[] = {7, 7, 7, 7, 3};
    run_test_case(arr4, 5, 3.0);

    double arr5[] = {0, 0, 0, 0, -1};
    run_test_case(arr5, 5, -1.0);

    double arr6[] = {9, 1, 9, 9, 9};
    run_test_case(arr6, 5, 1.0);

    double arr7[] = {6, 5, 6, 6, 6};
    run_test_case(arr7, 5, 5.0);

    double arr8[] = {100000, 100000, 100000, 999999};
    run_test_case(arr8, 4, 999999.0);

    double arr9[] = {-10, -10, -10, -1};
    run_test_case(arr9, 4, -1.0);

    double arr10[] = {99.9, 99.9, 0.1, 99.9};
    run_test_case(arr10, 4, 0.1);

    printf("All test cases passed successfully.\n");
    return 0;
}
