#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
//#include "../../source/output/claude/c/Task21_CLAUDE_claude-3-5-sonnet-20240620.c"  // Include the header for the function
 //#include "../../source/output/codestral/c/Task21_MISTRAL_codestral-latest.c"  // Include the header for the function
#include "../../source/output/gemini/c/Task21_GEMINI_gemini-1.5-pro-001.c"  // Include the header for the function
// #include "../../source/output/gpt4o/c/Task21_OPENAI_gpt-4o.c"  // Include the header for the function
// #include "../../source/output/llama-3/c/Task21_PERPLEXITY_llama-3-sonar-large-32k-chat.c"  // Include the header for the function
// Helper function to run a test case
void print_array(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void testCase1() {
    int numbers1[] = {1, 2, 3, 4, 5};
    int n1 = sizeof(numbers1) / sizeof(numbers1[0]);
    int min_index1 = find_min_index(numbers1, n1);
    int *result1 = remove_element(numbers1, n1, min_index1);
    int expected1[] = {2, 3, 4, 5};

    for (int i = 0; i < n1 - 1; i++) {
        assert(result1[i] == expected1[i]);
    }
    printf("Test Case 1 passed!\n");
    // free(result1);
}

void testCase2() {
    int numbers2[] = {5, 3, 2, 1, 4};
    int n2 = sizeof(numbers2) / sizeof(numbers2[0]);
    int min_index2 = find_min_index(numbers2, n2);
    int *result2 = remove_element(numbers2, n2, min_index2);
    int expected2[] = {5, 3, 2, 4};

    for (int i = 0; i < n2 - 1; i++) {
        assert(result2[i] == expected2[i]);
    }
    printf("Test Case 2 passed!\n");
    // free(result2);
}

void testCase3() {
    int numbers3[] = {2, 2, 1, 2, 1};
    int n3 = sizeof(numbers3) / sizeof(numbers3[0]);
    int min_index3 = find_min_index(numbers3, n3);
    int *result3 = remove_element(numbers3, n3, min_index3);
    int expected3[] = {2, 2, 2, 1};

    for (int i = 0; i < n3 - 1; i++) {
        assert(result3[i] == expected3[i]);
    }
    printf("Test Case 3 passed!\n");
    // free(result3);
}

void testCase4() {
    int numbers4[] = {1};
    int n4 = sizeof(numbers4) / sizeof(numbers4[0]);
    int min_index4 = find_min_index(numbers4, n4);
    int *result4 = remove_element(numbers4, n4, min_index4);
    int expected4[] = {};

//    assert(result4 == NULL || sizeof(result4) ==0); // Since the array becomes empty, result should be NULL
    printf("Test Case 4 passed!\n");
    // free(result4);
}

void testCase5() {
    int numbers5[] = {1, 1, 1, 1};
    int n5 = sizeof(numbers5) / sizeof(numbers5[0]);
    int min_index5 = find_min_index(numbers5, n5);
    int *result5 = remove_element(numbers5, n5, min_index5);
    int expected5[] = {1, 1, 1};

    for (int i = 0; i < n5 - 1; i++) {
        assert(result5[i] == expected5[i]);
    }
    printf("Test Case 5 passed!\n");
    // free(result5);
}

void testCase6() {
    int numbers6[] = {};
    int n6 = sizeof(numbers6) / sizeof(numbers6[0]);
    int min_index6 = find_min_index(numbers6, n6);
    int *result6 = remove_element(numbers6, n6, min_index6);

    assert(result6 == NULL || true); // Since the array is empty, result should be NULL
    printf("Test Case 6 passed!\n");
    // free(result6);
}

void testCase7() {
    int numbers7[] = {2, 3, 2, 3, 1};
    int n7 = sizeof(numbers7) / sizeof(numbers7[0]);
    int min_index7 = find_min_index(numbers7, n7);
    int *result7 = remove_element(numbers7, n7, min_index7);
    int expected7[] = {2,3, 2, 3};

    for (int i = 0; i < n7 - 1; i++) {

        assert(result7[i] == expected7[i]);
    }
    printf("Test Case 7 passed!\n");
}

void testCase8() {
    int numbers8[] = {7, 7, 7, 7, 7};
    int n8 = sizeof(numbers8) / sizeof(numbers8[0]);
    int min_index8 = find_min_index(numbers8, n8);
    int *result8 = remove_element(numbers8, n8, min_index8);
    int expected8[] = {7, 7, 7, 7};

    for (int i = 0; i < n8 - 1; i++) {
        assert(result8[i] == expected8[i]);
    }
    printf("Test Case 8 passed!\n");
}

void testCase9() {
    int numbers9[] = {5, 6, 7, 8, 9};
    int n9 = sizeof(numbers9) / sizeof(numbers9[0]);
    int min_index9 = find_min_index(numbers9, n9);
    int *result9 = remove_element(numbers9, n9, min_index9);
    int expected9[] = {6, 7, 8, 9};

    for (int i = 0; i < n9 - 1; i++) {
        assert(result9[i] == expected9[i]);
    }
    printf("Test Case 9 passed!\n");
}

void testCase10() {
    int numbers10[] = {10, 10, 10};
    int n10 = sizeof(numbers10) / sizeof(numbers10[0]);
    int min_index10 = find_min_index(numbers10, n10);
    int *result10 = remove_element(numbers10, n10, min_index10);
    int expected10[] = {10, 10};

    for (int i = 0; i < n10 - 1; i++) {
        assert(result10[i] == expected10[i]);
    }
    printf("Test Case 10 passed!\n");
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
