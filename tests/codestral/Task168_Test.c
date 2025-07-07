#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* tweakLetters(char* word, int* tweaks) {
    char* result = malloc(strlen(word) + 1);
    for (int i = 0; i < (int)strlen(word); i++) {
        result[i] = ((word[i] - 'a' + tweaks[i] + 26) % 26) + 'a'; // تأكد من الالتفاف داخل الحروف الأبجدية
    }
    result[strlen(word)] = '\0';
    return result;
}

int assertEqual(const char* a, const char* b) {
    return strcmp(a, b) == 0;
}

void runTest(char* word, int* tweaks, const char* expected, int testNum) {
    char* result = tweakLetters(word, tweaks);
    if (assertEqual(result, expected)) {
        printf("Test %d: PASS\n", testNum);
    } else {
        printf("Test %d: FAIL (Expected: %s, Got: %s)\n", testNum, expected, result);
    }
    free(result);
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

    runTest("apple", arr1, "aqold", 1);
    runTest("many", arr2, "manx", 2);
    runTest("rhino", arr3, "sijop", 3);
    runTest("abc", arr4, "bcd", 4);
    runTest("xyz", arr5, "yza", 5);
    runTest("hello", arr6, "gemln", 6);
    runTest("test", arr7, "sdrs", 7);
    runTest("abcd", arr8, "bdfh", 8);
    runTest("zzz", arr9, "yyy", 9);
    runTest("world", arr10, "wpqnb", 10);

    return 0;
}
