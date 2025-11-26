#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


//#include "../../source/output/claude/c/Task20_CLAUDE_claude-3-5-sonnet-20240620.c"  // Include the header for the function
 #include "../../source/output/codestral/c/Task20_MISTRAL_codestral-latest.c"  // Include the header for the function
// #include "../../source/output/gemini/c/Task20_GEMINI_gemini-1.5-pro-001.c"  // Include the header for the function
// #include "../../source/output/gpt4o/c/Task20_OPENAI_gpt-4o.c"  // Include the header for the function
// #include "../../source/output/llama-3/c/Task20_PERPLEXITY_llama-3-sonar-large-32k-chat.c"  // Include the header for the function
// Helper function to run a test case
void testBasicExample1() {
    assert(dig_pow(89, 1) == 1);
    printf("Test Basic Example 1 passed!\n");
}

void testBasicExample2() {
    assert(dig_pow(92, 1) == -1);
    printf("Test Basic Example 2 passed!\n");
}

void testBasicExample3() {
    assert(dig_pow(695, 2) == 2);
    printf("Test Basic Example 3 passed!\n");
}

void testNoSolution() {
    assert(dig_pow(123, 2) == -1);
    printf("Test No Solution passed!\n");
}

void testSingleDigit() {
    assert(dig_pow(5, 1) == 1);
    printf("Test Single Digit passed!\n");
}

void testConsecutivePowers() {
    assert(dig_pow(135, 1) == 1);
    printf("Test Consecutive Powers passed!\n");
}

void testLargerValue() {
    assert(dig_pow(1000, 2) == -1);
    printf("Test Larger Value passed!\n");
}

void testAnotherExample() {
    assert(dig_pow(135, 2) == -1);
    printf("Test Another Example passed!\n");
}

void testAllDigitsSame() {
    assert(dig_pow(111, 1) == -1);
    printf("Test All Digits Same passed!\n");
}

void testLeadingZeros() {
    // Handling leading zeros, assuming n = 100 should return -1
    assert(dig_pow(100, 1) == -1);
    printf("Test Leading Zeros passed!\n");
}

int main() {
    testBasicExample1();
    testBasicExample2();
    testBasicExample3();
    testNoSolution();
    testSingleDigit();
    testConsecutivePowers();
    testLargerValue();
    testAnotherExample();
    testAllDigitsSame();
    testLeadingZeros();

    printf("All tests passed!\n");
    return 0;
}