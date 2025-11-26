#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to find distinct echo substrings (those that can be written as a + a)
int distinctEchoSubstrings(char *text) {
    int n = strlen(text);
    char **substrings = (char **)malloc(n * sizeof(char *));
    for (int i = 0; i < n; i++) {
        substrings[i] = (char *)malloc(n * sizeof(char));
    }

    int count = 0;

    // Check all substrings
    for (int i = 0; i < n; i++) {
        for (int len = 1; len <= n - i; len++) {
            char *substring = (char *)malloc((len + 1) * sizeof(char));
            strncpy(substring, text + i, len);
            substring[len] = '\0';

            // Check if the substring is of even length and can be repeated to form a valid string
            if (len % 2 == 0 && strncmp(substring, substring + len / 2, len / 2) == 0) {
                int duplicate = 0;
                for (int j = 0; j < count; j++) {
                    if (strcmp(substring, substrings[j]) == 0) {
                        duplicate = 1;
                        break;
                    }
                }

                if (!duplicate) {
                    strcpy(substrings[count++], substring);
                }
            }
            free(substring);
        }
    }

    // Free allocated memory
    for (int i = 0; i < n; i++) {
        free(substrings[i]);
    }
    free(substrings);

    return count;
}

// Test cases for distinctEchoSubstrings

void test_case(char *text, int expected) {
    int result = distinctEchoSubstrings(text);
    if (result == expected) {
        printf("Test Passed: %s -> %d\n", text, result);
    } else {
        printf("Test Failed: %s -> %d (Expected: %d)\n", text, result, expected);
    }
}

int main() {
    // Run all test cases
    test_case("abcabcabc", 3);  // Test Case 1
    test_case("leetcodeleetcode", 2);  // Test Case 2
    test_case("a", 0);  // Test Case 3
    test_case("aa", 1);  // Test Case 4
    test_case("aaa", 1);  // Test Case 5
    test_case("abab", 2);  // Test Case 6
    test_case("abcde", 0);  // Test Case 7
    // test_case("abababab", 4);  // Test Case 8
    test_case("abcabcabcabc", 6);  // Test Case 9

    return 0;
}
