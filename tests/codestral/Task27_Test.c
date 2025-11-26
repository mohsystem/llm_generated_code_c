#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


// #include "../../source/output/claude/c/Task27_CLAUDE_claude-3-5-sonnet-20240620.c"  // Include the header for the function
#include "../../source/output/codestral/c/Task27_MISTRAL_codestral-latest.c"  // Include the header for the function
// #include "../../source/output/gemini/c/Task27_GEMINI_gemini-1.5-pro-001.c"  // Include the header for the function
// #include "../../source/output/gpt4o/c/Task27_OPENAI_gpt-4o.c"  // Include the header for the function
// #include "../../source/output/llama-3/c/Task27_PERPLEXITY_llama-3-sonar-large-32k-chat.c"  // Include the header for the function

void test_case_1() {
    int input[] = {2, 4, 0, 100, 4, 11, 2602, 36};
    int expected = 11; // The only odd number
    int result = find_outlier(input, sizeof(input) / sizeof(input[0]));
    assert(result == expected);
    printf("Test Case 1 passed!\n");
}

void test_case_2() {
    int input[] = {160, 3, 1719, 19, 11, 13, -21};
    int expected = 160; // The only even number
    int result = find_outlier(input, sizeof(input) / sizeof(input[0]));
    assert(result == expected);
    printf("Test Case 2 passed!\n");
}

void test_case_3() {
    int input[] = {1, 3, 5, 7, 8}; // Only 8 is even
    int expected = 8;
    int result = find_outlier(input, sizeof(input) / sizeof(input[0]));
    assert(result == expected);
    printf("Test Case 3 passed!\n");
}

void test_case_4() {
    int input[] = {2, 4, 6, 8, 10, 13}; // Only 13 is odd
    int expected = 13;
    int result = find_outlier(input, sizeof(input) / sizeof(input[0]));
    assert(result == expected);
    printf("Test Case 4 passed!\n");
}

void test_case_5() {
    int input[] = {10, 12, 14, 16, 18, 21, 22}; // Only 21 is odd
    int expected = 21;
    int result = find_outlier(input, sizeof(input) / sizeof(input[0]));
    assert(result == expected);
    printf("Test Case 5 passed!\n");
}

void test_case_6() {
    int input[] = {1, 2, 3}; // Only 2 is even
    int expected = 2;
    int result = find_outlier(input, sizeof(input) / sizeof(input[0]));
    assert(result == expected);
    printf("Test Case 6 passed!\n");
}

void test_case_7() {
    int input[] = {3, 5, 7, 11, 2}; // Only 2 is even among odds
    int expected = 2;
    int result = find_outlier(input, sizeof(input) / sizeof(input[0]));
    assert(result == expected);
    printf("Test Case 7 passed!\n");
}

void test_case_8() {
    int input[] = {2, 2, 2, 2, 1}; // Only 1 is odd
    int expected = 1;
    int result = find_outlier(input, sizeof(input) / sizeof(input[0]));
    assert(result == expected);
    printf("Test Case 8 passed!\n");
}

void test_case_9() {
    int input[] = {11, 13, 15, 17, 19, 20}; // Only 20 is even
    int expected = 20;
    int result = find_outlier(input, sizeof(input) / sizeof(input[0]));
    assert(result == expected);
    printf("Test Case 9 passed!\n");
}

void test_case_10() {
    int input[] = {4, 8, 12, 16, 20, 21, 24}; // Only 21 is odd
    int expected = 21;
    int result = find_outlier(input, sizeof(input) / sizeof(input[0]));
    assert(result == expected);
    printf("Test Case 10 passed!\n");
}

int main() {
    test_case_1();
    test_case_2();
    test_case_3();
    test_case_4();
    test_case_5();
    test_case_6();
    test_case_7();
    test_case_8();
    test_case_9();
    test_case_10();

    printf("All test cases passed!\n");
    return 0;
}