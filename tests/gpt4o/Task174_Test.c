#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverse(char *str) {
    int n = (int)strlen(str);
    for (int i = 0; i < n / 2; i++) {
        char tmp = str[i];
        str[i] = str[n - i - 1];
        str[n - i - 1] = tmp;
    }
}

char* shortestPalindrome(char* s) {
    static char rev_s[50000];
    strcpy(rev_s, s);
    reverse(rev_s);
    int len_s = (int)strlen(s);
    for (int i = 0; i <= len_s; i++) {
        if (strncmp(s, rev_s + i, len_s - i) == 0) {
            char *result = (char*)malloc(len_s + i + 1);
            strncpy(result, rev_s, i);
            result[i] = '\0';
            strcat(result, s);
            return result;
        }
    }
    char* empty = (char*)malloc(1);
    empty[0] = '\0';
    return empty;
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
