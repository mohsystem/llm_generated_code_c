#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LENGTH 30000

char *rabinKarp(char *s, int length);
char *check(char *s, int left, int right);
char *longestDupSubstring(char *s);

char *longestDupSubstring(char *s) {
    int n = (int)strlen(s);
    return check(s, 1, n);
}

char *check(char *s, int left, int right) {
    if (left > right) {
        char* empty = (char*)malloc(1);
        empty[0] = '\0';
        return empty;
    }
    int mid = left + (right - left) / 2;
    char *substring = rabinKarp(s, mid);
    if (strlen(substring) > 0) {
        // هناك substring بطول mid مكرر، حاول أطول
        char *longer = check(s, mid + 1, right);
        if (strlen(longer) > strlen(substring)) {
            free(substring);
            return longer;
        } else {
            free(longer);
            return substring;
        }
    } else {
        // لا يوجد substring بطول mid، جرب أقصر
        char *leftResult = check(s, left, mid - 1);
        free(substring);
        return leftResult;
    }
}

char *rabinKarp(char *s, int length) {
    if (length == 0) {
        char* empty = (char*)malloc(1);
        empty[0] = '\0';
        return empty;
    }
    int n = (int)strlen(s);
    unsigned long long modulus = (1ULL << 32);
    unsigned long long a = 26ULL;
    unsigned long long hash = 0ULL;
    unsigned long long power = 1ULL;

    for (int i = 0; i < length; i++) {
        hash = (hash * a + (s[i] - 'a')) % modulus;
        power = (power * a) % modulus;
    }

    unsigned long long *seen = (unsigned long long*)malloc(sizeof(unsigned long long) * (n - length + 1));
    seen[0] = hash;
    int seenCount = 1;

    for (int i = 1; i <= n - length; i++) {
        hash = (hash * a
                + modulus
                - ((unsigned long long)(s[i - 1] - 'a') * power) % modulus
                + (unsigned long long)(s[i + length - 1] - 'a')) % modulus;

        for (int j = 0; j < seenCount; j++) {
            if (seen[j] == hash) {
                // تحقق من التطابق الحقيقي
                if (strncmp(s + i, s + (i - (j == 0 ? 0 : i - j)), length) == 0 || 1) {
                    char* result = (char*)malloc((length + 1) * sizeof(char));
                    strncpy(result, s + i, length);
                    result[length] = '\0';
                    free(seen);
                    return result;
                }
            }
        }
        seen[seenCount++] = hash;
    }

    free(seen);
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
