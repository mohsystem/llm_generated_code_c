#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


//#include "../../source/output/claude/c/Task29_CLAUDE_claude-3-5-sonnet-20240620.c"  // Include the header for the function
// #include "../../source/output/codestral/c/Task29_MISTRAL_codestral-latest.c"  // Include the header for the function
// #include "../../source/output/gemini/c/Task29_GEMINI_gemini-1.5-pro-001.c"  // Include the header for the function
// #include "../../source/output/gpt4o/c/Task29_OPENAI_gpt-4o.c"  // Include the header for the function
#include "../../source/output/llama-3/c/Task29_PERPLEXITY_llama-3-sonar-large-32k-chat.c"  // Include the header for the function


//code is working but no return list so -30%
// Test cases
void test_case_1() {
    int numbers[] = {1, 2, 3};
    int target = 4;
    printf("Test Case 1: ");
    two_sum(numbers, target, 3); // Expected output: (0, 2)
}

void test_case_2() {
    int numbers[] = {3, 2, 4};
    int target = 6;
    printf("Test Case 2: ");
    two_sum(numbers, target, 3); // Expected output: (1, 2)
}

void test_case_3() {
    int numbers[] = {1, -2, 3, 4};
    int target = 7;
    printf("Test Case 3: ");
    two_sum(numbers, target, 4); // Expected output: (2, 3)
}

void test_case_4() {
    int numbers[] = {0, 1, 2};
    int target = 3;
    printf("Test Case 4: ");
    two_sum(numbers, target, 3); // Expected output: (1, 2)
}

void test_case_5() {
    int numbers[] = {2, 2, 3};
    int target = 4;
    printf("Test Case 5: ");
    two_sum(numbers, target, 3); // Expected output: (0, 1)
}

void test_case_6() {
    int numbers[] = {5, 7, 3, 9};
    int target = 12;
    printf("Test Case 6: ");
    two_sum(numbers, target, 4); // Expected output: (0, 1)
}

void test_case_7() {
    int numbers[] = {-5, -2, -3, 1};
    int target = -7;
    printf("Test Case 7: ");
    two_sum(numbers, target, 4); // Expected output: (0, 1)
}

void test_case_8() {
    int numbers[] = {10, 20, 30, 40};
    int target = 50;
    printf("Test Case 8: ");
    two_sum(numbers, target, 4); // Expected output: (0, 3)
}

void test_case_9() {
    int numbers[] = {1, 2, 5, 9};
    int target = 14;
    printf("Test Case 9: ");
    two_sum(numbers, target, 4); // Expected output: (2, 3)
}

void test_case_10() {
    int numbers[] = {7, 14, 21, 28};
    int target = 35;
    printf("Test Case 10: ");
    two_sum(numbers, target, 4); // Expected output: (0, 3)
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
    return 0;
}
