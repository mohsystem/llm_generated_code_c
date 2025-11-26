#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


//#include "../../source/output/claude/c/Task21_CLAUDE_claude-3-5-sonnet-20240620.c"  // Include the header for the function
// #include "../../source/output/codestral/c/Task21_MISTRAL_codestral-latest.c"  // Include the header for the function
// #include "../../source/output/gemini/c/Task21_GEMINI_gemini-1.5-pro-001.c"  // Include the header for the function
// #include "../../source/output/gpt4o/c/Task21_OPENAI_gpt-4o.c"  // Include the header for the function
 #include "../../source/output/llama-3/c/Task21_PERPLEXITY_llama-3-sonar-large-32k-chat.c"  // Include the header for the function
// Helper function to run a test case





void testCase1() {
    int input[] = {1, 2, 3, 4, 5};
    int expected[] = {2, 3, 4, 5};
    int newSize;

    int* result = removeSmallest(input, 5, &newSize);

    assert(newSize == 4);
    for (int i = 0; i < newSize; i++) {
        assert(result[i] == expected[i]);
    }
    printf("Test Case 1 passed!\n");

    free(result);
}

void testCase2() {
    int input[] = {5, 3, 2, 1, 4};
    int expected[] = {5, 3, 2, 4};
    int newSize;

    int* result = removeSmallest(input, 5, &newSize);

    assert(newSize == 4);
    for (int i = 0; i < newSize; i++) {
        assert(result[i] == expected[i]);
    }
    printf("Test Case 2 passed!\n");

    free(result);
}

void testCase3() {
    int input[] = {2, 2, 1, 2, 1};
    int expected[] = {2, 2, 2, 1};
    int newSize;

    int* result = removeSmallest(input, 5, &newSize);

    assert(newSize == 4);
    for (int i = 0; i < newSize; i++) {
        assert(result[i] == expected[i]);
    }
    printf("Test Case 3 passed!\n");

    free(result);
}

void testCase4() {
    int input[] = {1};
    int expected[] = {};
    int newSize;

    int* result = removeSmallest(input, 1, &newSize);

    assert(newSize == 0);
    assert(result == NULL || newSize == 0); // Ensure no memory is allocated if size is 0
    printf("Test Case 4 passed!\n");

    free(result);
}

void testCase5() {
    int input[] = {1, 1, 1, 1};
    int expected[] = {1, 1, 1};
    int newSize;

    int* result = removeSmallest(input, 4, &newSize);

    assert(newSize == 3);
    for (int i = 0; i < newSize; i++) {
        assert(result[i] == expected[i]);
    }
    printf("Test Case 5 passed!\n");

    free(result);
}

void testCase6() {
    int input[] = {};
    int expected[] = {};
    int newSize;

    int* result = removeSmallest(input, 0, &newSize);

    assert(newSize == 0);
    assert(result == NULL || newSize == 0); // Ensure no memory is allocated if size is 0
    printf("Test Case 6 passed!\n");

    free(result);
}

void testCase7() {
    int input[] = {2, 3, 2, 3, 1};
    int expected[] = {2, 3, 2, 3};
    int newSize;

    int* result = removeSmallest(input, 5, &newSize);

    assert(newSize == 4);
    for (int i = 0; i < newSize; i++) {
        assert(result[i] == expected[i]);
    }
    printf("Test Case 7 passed!\n");

    free(result);
}

void testCase8() {
    int input[] = {7, 7, 7, 7, 7};
    int expected[] = {7, 7, 7, 7};
    int newSize;

    int* result = removeSmallest(input, 5, &newSize);

    assert(newSize == 4);
    for (int i = 0; i < newSize; i++) {
        assert(result[i] == expected[i]);
    }
    printf("Test Case 8 passed!\n");

    free(result);
}

void testCase9() {
    int input[] = {5, 6, 7, 8, 9};
    int expected[] = {6, 7, 8, 9};
    int newSize;

    int* result = removeSmallest(input, 5, &newSize);

    assert(newSize == 4);
    for (int i = 0; i < newSize; i++) {
        assert(result[i] == expected[i]);
    }
    printf("Test Case 9 passed!\n");

    free(result);
}

void testCase10() {
    int input[] = {10, 10, 10};
    int expected[] = {10, 10};
    int newSize;

    int* result = removeSmallest(input, 3, &newSize);

    assert(newSize == 2);
    for (int i = 0; i < newSize; i++) {
        assert(result[i] == expected[i]);
    }
    printf("Test Case 10 passed!\n");

    free(result);
}

int main() {
    testCase1();
    testCase2();
    testCase3();
    testCase4();
    testCase5();
    testCase6();
    testCase7();
    testCase8();
    testCase9();
    testCase10();
    return 0;
}