#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


#include "../../source/output/claude/c/Task29_CLAUDE_claude-3-5-sonnet-20240620.c"  // Include the header for the function
// #include "../../source/output/codestral/c/Task29_MISTRAL_codestral-latest.c"  // Include the header for the function
// #include "../../source/output/gemini/c/Task29_GEMINI_gemini-1.5-pro-001.c"  // Include the header for the function
// #include "../../source/output/gpt4o/c/Task29_OPENAI_gpt-4o.c"  // Include the header for the function
// #include "../../source/output/llama-3/c/Task29_PERPLEXITY_llama-3-sonar-large-32k-chat.c"  // Include the header for the function



void test_case_1() {
    int numbers[] = {1, 2, 3};
    int target = 4;
    int expected[] = {0, 2}; // 1 + 3 = 4
    int returnSize;
    int* result = twoSum(numbers, 3, target, &returnSize);
    assert(result[0] == expected[0] && result[1] == expected[1]);
    free(result);
    printf("Test Case 1 passed!\n");
}

void test_case_2() {
    int numbers[] = {3, 2, 4};
    int target = 6;
    int expected[] = {1, 2}; // 2 + 4 = 6
    int returnSize;
    int* result = twoSum(numbers, 3, target, &returnSize);
    assert(result[0] == expected[0] && result[1] == expected[1]);
    free(result);
    printf("Test Case 2 passed!\n");
}

void test_case_3() {
    int numbers[] = {1, -2, 3, 4};
    int target = 7;
    int expected[] = {2, 3}; // 3 + 4 = 7
    int returnSize;
    int* result = twoSum(numbers, 4, target, &returnSize);
    assert(result[0] == expected[0] && result[1] == expected[1]);
    free(result);
    printf("Test Case 3 passed!\n");
}

void test_case_4() {
    int numbers[] = {0, 1, 2};
    int target = 3;
    int expected[] = {1, 2}; // 1 + 2 = 3
    int returnSize;
    int* result = twoSum(numbers, 3, target, &returnSize);
    assert(result[0] == expected[0] && result[1] == expected[1]);
    free(result);
    printf("Test Case 4 passed!\n");
}

void test_case_5() {
    int numbers[] = {2, 2, 3};
    int target = 4;
    int expected[] = {0, 1}; // 2 + 2 = 4
    int returnSize;
    int* result = twoSum(numbers, 3, target, &returnSize);
    assert(result[0] == expected[0] && result[1] == expected[1]);
    free(result);
    printf("Test Case 5 passed!\n");
}

void test_case_6() {
    int numbers[] = {5, 7, 3, 9};
    int target = 12;
    int expected[] = {0, 1}; // 5 + 7 = 12
    int returnSize;
    int* result = twoSum(numbers, 4, target, &returnSize);
    assert(result[0] == expected[0] && result[1] == expected[1]);
    free(result);
    printf("Test Case 6 passed!\n");
}

void test_case_7() {
    int numbers[] = {-5, -2, -3, 1};
    int target = -7;
    int expected[] = {0, 1}; // -5 + (-2) = -7
    int returnSize;
    int* result = twoSum(numbers, 4, target, &returnSize);
    assert(result[0] == expected[0] && result[1] == expected[1]);
    free(result);
    printf("Test Case 7 passed!\n");
}

void test_case_8() {
    int numbers[] = {10, 20, 30, 40};
    int target = 50;
    int expected[] = {1, 2}; // 20 + 30 = 50
    int expected1[] = {0, 3}; // 10 + 40 = 50
    int returnSize;
    int* result = twoSum(numbers, 4, target, &returnSize);
    assert((result[0] == expected[0] && result[1] == expected[1] )|| (result[0] == expected1[0] && result[1] == expected1[1] ));
    free(result);
    printf("Test Case 8 passed!\n");
}

void test_case_9() {
    int numbers[] = {1, 2, 5, 9};
    int target = 14;
    int expected[] = {2, 3}; // 5 + 9 = 14
    int returnSize;
    int* result = twoSum(numbers, 4, target, &returnSize);
    assert(result[0] == expected[0] && result[1] == expected[1]);
    free(result);
    printf("Test Case 9 passed!\n");
}

void test_case_10() {
    int numbers[] = {7, 14, 21, 28};
    int target = 35;
    int expected[] = {1, 2}; // 14 + 21 = 35
    int expected1[] = {0, 3}; // 14 + 21 = 35
    int returnSize;
    int* result = twoSum(numbers, 4, target, &returnSize);
    assert((result[0] == expected[0] && result[1] == expected[1]) || (result[0] == expected1[0] && result[1] == expected1[1]));
    free(result);
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