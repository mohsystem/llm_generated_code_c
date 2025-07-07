#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* tweakLetters(char* txt, int arr[], int size) {
    char* res = (char*) malloc(sizeof(char) * (strlen(txt) + 1));
    for (int i = 0; i < (int)strlen(txt); i++) {
        if (arr[i] == 1) {
            // التعامل مع الالتفاف 'z' -> 'a'
            res[i] = (txt[i] == 'z') ? 'a' : (txt[i] + 1);
        } else if (arr[i] == -1) {
            // التعامل مع الالتفاف 'a' -> 'z'
            res[i] = (txt[i] == 'a') ? 'z' : (txt[i] - 1);
        } else {
            res[i] = txt[i];
        }
    }
    res[strlen(txt)] = '\0';
    return res;
}

int assertEqual(const char* a, const char* b) {
    return strcmp(a, b) == 0;
}

void runTest(char* input, int* tweaks, int size, char* expected, int testNum) {
    char* result = tweakLetters(input, tweaks, size);
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
