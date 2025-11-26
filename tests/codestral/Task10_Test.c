#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


//#include "../../source/output/claude/c/Task10_CLAUDE_claude-3-5-sonnet-20240620.c"  // Include the header for the function
#include "../../source/output/codestral/c/Task10_MISTRAL_codestral-latest.c"  // Include the header for the function
// #include "../../source/output/gemini/c/Task10_GEMINI_gemini-1.5-pro-001.c"  // Include the header for the function
// #include "../../source/output/gpt4o/c/Task10_OPENAI_gpt-4o.c"  // Include the header for the function
// #include "../../source/output/llama-3/c/Task10_PERPLEXITY_llama-3-sonar-large-32k-chat.c"  // Include the header for the function
// Helper function to run a test case



// Function prototype
// int getSum(int a, int b);

void testPositiveRange() {
    // Test with positive numbers where a < b
    assert(getSum(1, 2) == 3);
}

void testPositiveRangeReversed() {
    // Test with positive numbers where b < a
    assert(getSum(2, 1) == 3);
}

void testSingleNumber() {
    // Test with a single number
    assert(getSum(5, 5) == 5);
}

void testZeroInRange() {
    // Test with a range that includes zero
    assert(getSum(0, 1) == 1);
}

void testNegativeToPositive() {
    // Test with a range from negative to positive numbers
    assert(getSum(-1, 2) == 2);
}

void testNegativeRange() {
    // Test with negative numbers where a < b
    assert(getSum(-3, -1) == -6);
}

void testNegativeRangeReversed() {
    // Test with negative numbers where b < a
    assert(getSum(-1, -3) == -6);
}

void testPositiveAndNegativeRange() {
    // Test with a mix of positive and negative numbers where a < b
    assert(getSum(-2, 1) == -2);
}

void testPositiveAndNegativeRangeReversed() {
    // Test with a mix of positive and negative numbers where b < a
    assert(getSum(1, -2) == -2);
}

void testLargeRange() {
    // Test with a large range from -100 to 100
    assert(getSum(-100, 100) == 0);
}

int main() {
    testPositiveRange();
    testPositiveRangeReversed();
    testSingleNumber();
    testZeroInRange();
    testNegativeToPositive();
    testNegativeRange();
    testNegativeRangeReversed();
    testPositiveAndNegativeRange();
    testPositiveAndNegativeRangeReversed();
    testLargeRange();

    printf("All test cases passed!\n");
    return 0;
}
