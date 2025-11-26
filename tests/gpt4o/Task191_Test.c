#include <stdio.h>
#include <string.h>

int longestValidParentheses(char* s) {
    int stack[30000];
    int stackIndex = 0;
    stack[stackIndex++] = -1;  // Initialize the stack with a sentinel value
    int max_length = 0;

    for (int i = 0; i < strlen(s); i++) {
        if (s[i] == '(') {
            stack[stackIndex++] = i;  // Push the index of '(' onto the stack
        } else {
            stackIndex--;  // Pop the stack for ')'
            if (stackIndex == 0) {
                stack[stackIndex++] = i;  // Reset the stack if it is empty
            } else {
                int current_length = i - stack[stackIndex - 1];  // Calculate the length of valid substring
                if (current_length > max_length) {
                    max_length = current_length;  // Update max_length
                }
            }
        }
    }

    return max_length;  // Return the maximum length of valid parentheses substring
}

int main() {
    char s[] = "(()";  // Example 1
    printf("%d\n", longestValidParentheses(s));  // Output: 2

    char s2[] = ")()())";  // Example 2
    printf("%d\n", longestValidParentheses(s2));  // Output: 4

    char s3[] = "";  // Example 3
    printf("%d\n", longestValidParentheses(s3));  // Output: 0

    return 0;
}
