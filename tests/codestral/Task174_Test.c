#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* shortestPalindrome(char* s) {
    int len = (int)strlen(s);
    if (len <= 1) {
        return strdup(s);
    }

    int i = 0;
    for (int j = len - 1; j >= 0; j--) {
        if (s[i] == s[j]) {
            i++;
        }
    }
    if (i == len) {
        return strdup(s);
    }

    char* suffix = strdup(s + i);
    char* prefix = strdup(suffix);
    int prefix_len = (int)strlen(prefix);

    // عكس prefix
    for (int j = 0; j < prefix_len / 2; j++) {
        char temp = prefix[j];
        prefix[j] = prefix[prefix_len - j - 1];
        prefix[prefix_len - j - 1] = temp;
    }

    char* left_substring = strndup(s, i);
    char* mid = shortestPalindrome(left_substring);

    size_t result_len = strlen(prefix) + strlen(mid) + strlen(suffix) + 1;
    char* result = (char*)malloc(result_len);

    strcpy(result, prefix);
    strcat(result, mid);
    strcat(result, suffix);

    free(prefix);
    free(suffix);
    free(mid);
    free(left_substring);

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
