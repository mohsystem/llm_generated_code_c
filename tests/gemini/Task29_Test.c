#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


// #include "../../source/output/claude/c/Task29_CLAUDE_claude-3-5-sonnet-20240620.c"  // Include the header for the function
// #include "../../source/output/codestral/c/Task29_MISTRAL_codestral-latest.c"  // Include the header for the function
#include "../../source/output/gemini/c/Task29_GEMINI_gemini-1.5-pro-001.c"  // Include the header for the function
// #include "../../source/output/gpt4o/c/Task29_OPENAI_gpt-4o.c"  // Include the header for the function
// #include "../../source/output/llama-3/c/Task29_PERPLEXITY_llama-3-sonar-large-32k-chat.c"  // Include the header for the function

void test_case_1() {
    int numbers[] = {1, 2, 3};
    int target = 4;
    Tuple expected = {0, 2}; // 1 + 3 = 4
    Tuple result = two_sum(numbers, 3, target);
    assert(result.index1 == expected.index1 && result.index2 == expected.index2);
    printf("Test Case 1 passed!\n");
}

void test_case_2() {
    int numbers[] = {3, 2, 4};
    int target = 6;
    Tuple expected = {1, 2}; // 2 + 4 = 6
    Tuple result = two_sum(numbers, 3, target);
    assert(result.index1 == expected.index1 && result.index2 == expected.index2);
    printf("Test Case 2 passed!\n");
}

void test_case_3() {
    int numbers[] = {1, -2, 3, 4};
    int target = 7;
    Tuple expected = {2, 3}; // 3 + 4 = 7
    Tuple result = two_sum(numbers, 4, target);
    assert(result.index1 == expected.index1 && result.index2 == expected.index2);
    printf("Test Case 3 passed!\n");
}

void test_case_4() {
    int numbers[] = {0, 1, 2};
    int target = 3;
    Tuple expected = {1, 2}; // 1 + 2 = 3
    Tuple result = two_sum(numbers, 3, target);
    assert(result.index1 == expected.index1 && result.index2 == expected.index2);
    printf("Test Case 4 passed!\n");
}

void test_case_5() {
    int numbers[] = {2, 2, 3};
    int target = 4;
    Tuple expected = {0, 1}; // 2 + 2 = 4
    Tuple result = two_sum(numbers, 3, target);
    assert(result.index1 == expected.index1 && result.index2 == expected.index2);
    printf("Test Case 5 passed!\n");
}

void test_case_6() {
    int numbers[] = {5, 7, 3, 9};
    int target = 12;
    Tuple expected = {0, 1}; // 5 + 7 = 12
    Tuple result = two_sum(numbers, 4, target);
    assert(result.index1 == expected.index1 && result.index2 == expected.index2);
    printf("Test Case 6 passed!\n");
}


void test_case_7() {
    int numbers[] = {-5, -2, -3, 1};
    int target = -7;
    Tuple expected = {0, 1}; // -5 + (-2) = -7
    Tuple result = two_sum(numbers, 4, target);
    assert(result.index1 == expected.index1 && result.index2 == expected.index2);
    printf("Test Case 7 passed!\n");
}

void test_case_8() {
    int numbers[] = {10, 20, 30, 40};
    int target = 50;
    Tuple expected = {1, 2}; // 20 + 30 = 50
    Tuple expected1 = {0, 3}; // 10 + 40 = 50
    Tuple result = two_sum(numbers, 4, target);
    assert((result.index1 == expected.index1 && result.index2 == expected.index2) || (result.index1 == expected1.index1 && result.index2 == expected1.index2));//
    printf("Test Case 8 passed!\n");
}

void test_case_9() {
    int numbers[] = {1, 2, 5, 9};
    int target = 14;
    Tuple expected = {2, 3}; // 5 + 9 = 14
    Tuple result = two_sum(numbers, 4, target);
    assert(result.index1 == expected.index1 && result.index2 == expected.index2);//
    printf("Test Case 9 passed!\n");
}

void test_case_10() {
    int numbers[] = {7, 14, 21, 28};
    int target = 35;
    Tuple expected = {1, 2}; // 14 + 21 = 35
    Tuple expected1 = {0, 3}; // 14 + 21 = 35
    Tuple result = two_sum(numbers, 4, target);
    assert((result.index1 == expected.index1 && result.index2 == expected.index2) || (result.index1 == expected1.index1 && result.index2 == expected1.index2));//
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