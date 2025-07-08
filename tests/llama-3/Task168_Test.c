#include <stdio.h>
#include <string.h>

void tweakLetters(char* s, int tweaks[], char* result) {
    int len = strlen(s);
    for (int i = 0; i < len; i++) {
        result[i] = s[i] + tweaks[i];
    }
    result[len] = '\0';
}

int assertEqual(const char* a, const char* b) {
    return strcmp(a, b) == 0;
}

void runTest(const char* input, int tweaks[], const char* expected, int testNum) {
    char result[100];
    tweakLetters((char*)input, tweaks, result);
    if (assertEqual(result, expected)) {
        printf("Test %d: PASS\n", testNum);
    } else {
        printf("Test %d: FAIL (Expected: %s, Got: %s)\n", testNum, expected, result);
    }
}

int main() {
    runTest("apple", (int[]){0, 1, -1, 0, -1}, "aqold", 1);
    runTest("many", (int[]){0, 0, 0, -1}, "manx", 2);
    runTest("rhino", (int[]){1, 1, 1, 1, 1}, "sijop", 3);
    runTest("abc", (int[]){1, 1, 1}, "bcd", 4);
    runTest("xyz", (int[]){1, 1, 1}, "yz{", 5); // Note: no wrap-around in your code, so 'z'+1='{' ASCII 123
    runTest("hello", (int[]){-1, 0, 1, 0, -1}, "gemln", 6);
    runTest("test", (int[]){-1, -1, -1, -1}, "sdrs", 7);
    runTest("abcd", (int[]){1, 2, 3, 4}, "bdfh", 8);
    runTest("zzz", (int[]){-1, -1, -1}, "yyy", 9);
    runTest("world", (int[]){0, 1, -1, 2, -2}, "wpqnb", 10);

    return 0;
}
