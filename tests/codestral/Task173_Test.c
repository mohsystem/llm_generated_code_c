#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 30000
#define MOD 0xFFFFFFFFULL
#define ALPHABET_SIZE 26

unsigned long long hash_values[MAX_SIZE];
unsigned long long powers[MAX_SIZE];

char* longestDupSubstring(char* s) {
    int n = (int)strlen(s);
    int* nums = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        nums[i] = s[i] - 'a';
    }

    int check(int length) {
        unsigned long long h = 0;
        for (int i = 0; i < length; i++) {
            h = (h * ALPHABET_SIZE + nums[i]) & MOD;
        }
        hash_values[0] = h;
        powers[0] = 1;
        for (int i = 1; i <= length; i++) {
            powers[i] = (powers[i - 1] * ALPHABET_SIZE) & MOD;
        }
        for (int start = 1; start <= n - length; start++) {
            h = (h * ALPHABET_SIZE
                 - nums[start - 1] * powers[length]
                 + nums[start + length - 1]) & MOD;
            hash_values[start] = h;
            for (int j = 0; j < start; j++) {
                if (hash_values[start] == hash_values[j]) {
                    return j;
                }
            }
        }
        return -1;
    }

    int start = -1, length = 0;
    for (int k = 1; k < n; k++) {
        int pos = check(k);
        if (pos != -1 && k > length) {
            start = pos;
            length = k;
        }
    }
    free(nums);

    if (start != -1) {
        char* result = (char*)malloc((length + 1) * sizeof(char));
        strncpy(result, s + start, length);
        result[length] = '\0';
        return result;
    }
    // رجع نص فارغ إذا ما في substring مكرر
    char* empty = (char*)malloc(1);
    empty[0] = '\0';
    return empty;
}

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
