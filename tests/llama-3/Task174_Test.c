#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* shortestPalindrome(char* s) {
    int len = (int)strlen(s);
    if (len == 0) {
        return strdup("");
    }
    int n = 2 * len + 1;
    char* a = (char*)malloc(n + 1);
    strcpy(a, s);
    strcat(a, "#");
    // نضيف النص بالعكس بعد #
    for (int i = len - 1; i >= 0; i--) {
        int pos = len + 1 + (len - 1 - i);
        a[pos] = s[i];
    }
    a[n] = '\0';

    int* k = (int*)calloc(n, sizeof(int));
    k[0] = 0;
    for (int i = 1; i < n; i++) {
        int j = k[i - 1];
        while (j > 0 && a[i] != a[j]) {
            j = k[j - 1];
        }
        k[i] = j + (a[i] == a[j] ? 1 : 0);
    }

    int prefix_len = k[n - 1];
    int to_add_len = len - prefix_len;

    char* res = (char*)malloc(len + to_add_len + 1);
    // نضيف حروف بالعكس من نهاية s إلى بداية s + s
    for (int i = 0; i < to_add_len; i++) {
        res[i] = s[len - 1 - i];
    }
    strcpy(res + to_add_len, s);

    free(a);
    free(k);
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
