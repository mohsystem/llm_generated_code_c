#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* shortestPalindrome(char* s) {
    int len = (int)strlen(s);
    char* rev = (char*)malloc((len + 1) * sizeof(char));
    for (int i = 0; i < len; i++) {
        rev[i] = s[len - 1 - i];
    }
    rev[len] = '\0';

    char* temp = (char*)malloc((2 * len + 2) * sizeof(char));
    sprintf(temp, "%s#%s", s, rev);

    int* lps = (int*)calloc(2 * len + 1, sizeof(int));

    for (int i = 1; i < 2 * len + 1; i++) {
        int j = lps[i - 1];
        while (j > 0 && temp[i] != temp[j]) {
            j = lps[j - 1];
        }
        if (temp[i] == temp[j]) {
            j++;
        }
        lps[i] = j;
    }

    int addLen = len - lps[2 * len];
    char* result = (char*)malloc((len + addLen + 1) * sizeof(char));
    strncpy(result, rev, addLen);
    strcpy(result + addLen, s);

    free(rev);
    free(temp);
    free(lps);
    return result;
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
