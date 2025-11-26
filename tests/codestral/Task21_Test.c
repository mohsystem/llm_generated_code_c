#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
//#include "../../source/output/claude/c/Task21_CLAUDE_claude-3-5-sonnet-20240620.c"  // Include the header for the function
 #include "../../source/output/codestral/c/Task21_MISTRAL_codestral-latest.c"  // Include the header for the function
//#include "../../source/output/gemini/c/Task21_GEMINI_gemini-1.5-pro-001.c"  // Include the header for the function
// #include "../../source/output/gpt4o/c/Task21_OPENAI_gpt-4o.c"  // Include the header for the function
// #include "../../source/output/llama-3/c/Task21_PERPLEXITY_llama-3-sonar-large-32k-chat.c"  // Include the header for the function
// Helper function to run a test case

void testCase1() {
    int input[] = {1, 2, 3, 4, 5};
    int expected[] = {2, 3, 4, 5};
    size_t new_size;

    // Call remove_smallest and store the result in a pointer
    int* result = remove_smallest(input, 5, &new_size);

    // Assert that the new size is correct
    assert(new_size == 4);

    // Assert that each element of the result matches the expected output
    for (size_t i = 0; i < new_size; i++) {
        assert(result[i] == expected[i]);
    }

    printf("Test Case 1 passed!\n");

    // Free the allocated memory
    free(result);
}void testCase2() {
    int input[] = {5, 3, 2, 1, 4};
    int expected[] = {5, 3, 2, 4};
    size_t new_size;

    int* result = remove_smallest(input, 5, &new_size);

    assert(new_size == 4);

    for (size_t i = 0; i < new_size; i++) {
        assert(result[i] == expected[i]);
    }

    printf("Test Case 2 passed!\n");
    free(result);
}

void testCase3() {
    int input[] = {2, 2, 1, 2, 1};
    int expected[] = {2, 2, 2, 1};
    size_t new_size;

    int* result = remove_smallest(input, 5, &new_size);

    assert(new_size == 4);

    for (size_t i = 0; i < new_size; i++) {
        assert(result[i] == expected[i]);
    }

    printf("Test Case 3 passed!\n");
    free(result);
}

void testCase4() {
    int input[] = {1};
    int expected[] = {};
    size_t new_size;

    int* result = remove_smallest(input, 1, &new_size);

    assert(new_size == 0);

    printf("Test Case 4 passed!\n");
    free(result);
}

void testCase5() {
    int input[] = {1, 1, 1, 1};
    int expected[] = {1, 1, 1};
    size_t new_size;

    int* result = remove_smallest(input, 4, &new_size);

    assert(new_size == 3);

    for (size_t i = 0; i < new_size; i++) {
        assert(result[i] == expected[i]);
    }

    printf("Test Case 5 passed!\n");
    free(result);
}

void testCase6() {
    int input[] = {};
    int expected[] = {};
    size_t new_size;

    int* result = remove_smallest(input, 0, &new_size);

    assert(new_size == 0);

    printf("Test Case 6 passed!\n");
    free(result);
}

void testCase7() {
    int input[] = {2, 3, 2, 3, 1};
    int expected[] = {2, 3, 2, 3};
    size_t new_size;

    int* result = remove_smallest(input, 5, &new_size);

    assert(new_size == 4);

    for (size_t i = 0; i < new_size; i++) {
        assert(result[i] == expected[i]);
    }

    printf("Test Case 7 passed!\n");
    free(result);
}

void testCase8() {
    int input[] = {7, 7, 7, 7, 7};
    int expected[] = {7, 7, 7, 7};
    size_t new_size;

    int* result = remove_smallest(input, 5, &new_size);

    assert(new_size == 4);

    for (size_t i = 0; i < new_size; i++) {
        assert(result[i] == expected[i]);
    }

    printf("Test Case 8 passed!\n");
    free(result);
}

void testCase9() {
    int input[] = {5, 6, 7, 8, 9};
    int expected[] = {6, 7, 8, 9};
    size_t new_size;

    int* result = remove_smallest(input, 5, &new_size);

    assert(new_size == 4);

    for (size_t i = 0; i < new_size; i++) {
        assert(result[i] == expected[i]);
    }

    printf("Test Case 9 passed!\n");
    free(result);
}

void testCase10() {
    int input[] = {10, 10, 10};
    int expected[] = {10, 10};
    size_t new_size;

    int* result = remove_smallest(input, 3, &new_size);

    assert(new_size == 2);

    for (size_t i = 0; i < new_size; i++) {
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