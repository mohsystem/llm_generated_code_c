#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool isPalindrome(char *s, int left, int right) {
    while (left < right) {
        if (s[left] != s[right]) {
            return false;
        }
        ++left;
        --right;
    }
    return true;
}

char *shortestPalindrome(char *s){
    int n = (int)strlen(s);
    long long base = 26, mod = 1000000007;
    long long hashVal = 0, powBase = 1;
    int best = 0;
    for (int i = n - 1; i >= 0; --i) {
        hashVal = (hashVal * base + (s[i] - 'a')) % mod;
        if (i == 0 || isPalindrome(s, 0, i)) {
            best = i + 1;
        }
    }
    char *res = (char *)malloc(sizeof(char) * (n + n - best + 1));
    int idx = 0;
    for (int i = n - best - 1; i >= 0; --i) {
        res[idx++] = s[i];
    }
    for (int i = 0; i < n; ++i) {
        res[idx++] = s[i];
    }
    res[idx] = '\0';
    return res;
}

void runTest(const char* s, const char* expected, const char* testName) {
    char* res = shortestPalindrome((char*)s);
    if (strcmp(res, expected) == 0) {
        printf("[PASS] %s\n", testName);
    } else {
        printf("[FAIL] %s - Expected: \"%s\", Got: \"%s\"\n", testName, expected, res);
    }
    free(res);
}

int main() {
    runTest("racecar", "racecar", "Test Case 1");
    runTest("aacecaaa", "aaacecaaa", "Test Case 2");
    runTest("abcd", "dcbabcd", "Test Case 3");
    runTest("", "", "Test Case 4");
    runTest("a", "a", "Test Case 5");
    runTest("aaaaaaaa", "aaaaaaaa", "Test Case 6");
    runTest("abcda", "adcbaabcda", "Test Case 7");
    runTest("ababababab", "bababababab", "Test Case 8");
    runTest("xyz", "zyxyz", "Test Case 9");
    runTest("abbaac", "caabbaac", "Test Case 10");

    return 0;
}
