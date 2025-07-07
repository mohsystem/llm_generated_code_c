#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

// Function to check if a string can be written as a + a (concatenation of some string with itself)
bool repeatedSubstringPattern(char* s) {
    int n = strlen(s);
    for (int i = 1; i <= n / 2; i++) {
        if (n % i == 0) {
            char* sub = (char*)malloc(i + 1);
            strncpy(sub, s, i);
            sub[i] = '\0';
            char* concat = (char*)malloc(n + 1);
            strcpy(concat, "");
            for (int j = 0; j < n / i; j++) {
                strcat(concat, sub);
            }
            if (strcmp(concat, s) == 0) {
                free(sub);
                free(concat);
                return true;
            }
            free(sub);
            free(concat);
        }
    }
    return false;
}

// Function to count the distinct substrings of text that can be written as a + a
int numDistinct(char* text) {
    int n = strlen(text);
    int count = 0;
    for (int i = 1; i <= n / 2; i++) {
        char* sub = (char*)malloc(i + 1);
        strncpy(sub, text, i);
        sub[i] = '\0';
        if (repeatedSubstringPattern(sub)) {
            count++;
        }
        free(sub);
    }
    return count;
}

// Test cases
void test_case(char* text, int expected) {
    int result = numDistinct(text);
    if (result == expected) {
        printf("Test Passed: %s -> %d\n", text, result);
    } else {
        printf("Test Failed: %s -> %d (Expected: %d)\n", text, result, expected);
    }
}

int main() {
    test_case("abcabcabc", 3);  // Test Case 1
    test_case("leetcodeleetcode", 2);  // Test Case 2
    test_case("a", 0);  // Test Case 3
    test_case("aa", 1);  // Test Case 4
    test_case("aaa", 1);  // Test Case 5
    test_case("abab", 2);  // Test Case 6
    test_case("abcde", 0);  // Test Case 7
    test_case("abababab", 4);  // Test Case 8
    test_case("abcabcabcabc", 6);  // Test Case 9

    return 0;
}
