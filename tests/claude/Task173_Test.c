#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 30000

char* longestDupSubstring(char* s) {
    int len = strlen(s);
    char* result = NULL;
    int maxLen = 0;

    for (int i = 0; i < len; i++) {
        for (int j = i + 1; j < len; j++) {
            int k = 0;
            while (j + k < len && s[i + k] == s[j + k]) {
                k++;
            }
            if (k > maxLen) {
                maxLen = k;
                if (result != NULL) free(result);
                result = (char*)malloc((maxLen + 1) * sizeof(char));
                strncpy(result, s + i, maxLen);
                result[maxLen] = '\0';
            }
        }
    }

    if (result == NULL) {
        // No duplicated substring found
        result = (char*)malloc(1);
        result[0] = '\0';
    }
    return result;
}

// دالة مقارنة للنصوص، تستخدم في assert
int strEquals(const char* a, const char* b) {
    return strcmp(a, b) == 0;
}

void runTest(const char* s, const char* expected, const char* testName) {
    char* res = longestDupSubstring((char*)s);
    if (strEquals(res, expected)) {
        printf("[PASS] %s\n", testName);
    } else {
        printf("[FAIL] %s - Expected: \"%s\", Got: \"%s\"\n", testName, expected, res);
    }
    free(res);
}

int main() {
    runTest("abcabcabcd", "abcabc", "Test Case 1");
    runTest("abababcababab", "ababab", "Test Case 2");
    runTest("abacabadabacaba", "abacaba", "Test Case 3");
    runTest("abcdefgabcdefgabcdefg", "abcdefgabcdefg", "Test Case 4");
    runTest("aabbaabbaaccddcc", "aabbaa", "Test Case 5");
    runTest("abcdabcdeabcdabcdeabcd", "abcdabcdeabcd", "Test Case 6");
    runTest("abcdefghijklmnoabcdefghijklmno", "abcdefghijklmno", "Test Case 7");
    runTest("xyzxyzxyzxyzx", "xyzxyzxyzx", "Test Case 8");
    runTest("abcdefgh12345678abcdefgh", "abcdefgh", "Test Case 9");
    runTest("abcxabcyabczabcdabc", "abc", "Test Case 10");

    return 0;
}
