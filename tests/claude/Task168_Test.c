#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void tweakLetters(char* word, int* arr, int arrSize) {
    int len = strlen(word);
    for (int i = 0; i < len && i < arrSize; i++) {
        word[i] = ((word[i] - 'a' + arr[i] + 26) % 26) + 'a';
    }
}

bool assertEqual(char* result, const char* expected) {
    return strcmp(result, expected) == 0;
}

void runTest(char* word, int* arr, int arrSize, const char* expected, int testNum) {
    char copy[100];
    strcpy(copy, word);
    tweakLetters(copy, arr, arrSize);
    if (assertEqual(copy, expected)) {
        printf("Test %d: PASS\n", testNum);
    } else {
        printf("Test %d: FAIL (Expected: %s, Got: %s)\n", testNum, expected, copy);
    }
}

int main() {
    int arr1[] = {0, 1, -1, 0, -1};
    int arr2[] = {0, 0, 0, -1};
    int arr3[] = {1, 1, 1, 1, 1};
    int arr4[] = {1, 1, 1};
    int arr5[] = {1, 1, 1};
    int arr6[] = {-1, 0, 1, 0, -1};
    int arr7[] = {-1, -1, -1, -1};
    int arr8[] = {1, 2, 3, 4};
    int arr9[] = {-1, -1, -1};
    int arr10[] = {0, 1, -1, 2, -2};

    runTest("apple", arr1, 5, "aqold", 1);
    runTest("many", arr2, 4, "manx", 2);
    runTest("rhino", arr3, 5, "sijop", 3);
    runTest("abc", arr4, 3, "bcd", 4);
    runTest("xyz", arr5, 3, "yza", 5);
    runTest("hello", arr6, 5, "gemln", 6);
    runTest("test", arr7, 4, "sdrs", 7);
    runTest("abcd", arr8, 4, "bdfh", 8);
    runTest("zzz", arr9, 3, "yyy", 9);
    runTest("world", arr10, 5, "wpqnb", 10);

    return 0;
}
