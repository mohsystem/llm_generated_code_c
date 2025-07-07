#include <stdio.h>
#include <string.h>

int longestValidParentheses(char *s) {
    int max_len = 0;
    int last_invalid = -1;
    int stack[10000];
    int top = -1;

    for (int i = 0; i < strlen(s); i++) {
        if (s[i] == '(') {
            stack[++top] = i;  // Push index of '(' onto the stack
        } else {
            if (top == -1) {
                last_invalid = i;  // If stack is empty, reset last_invalid to current index
            } else {
                top--;  // Pop the stack for ')'
                if (top == -1) {
                    max_len = max_len > i - last_invalid ? max_len : i - last_invalid;  // Update max_len
                } else {
                    max_len = max_len > i - stack[top] ? max_len : i - stack[top];  // Calculate valid length
                }
            }
        }
    }
    return max_len;  // Return the maximum valid parentheses length
}

int main() {
    char s1[] = "(()";  // Example 1
    printf("%d\n", longestValidParentheses(s1));  // Output: 2

    char s2[] = ")()())";  // Example 2
    printf("%d\n", longestValidParentheses(s2));  // Output: 4

    char s3[] = "";  // Example 3
    printf("%d\n", longestValidParentheses(s3));  // Output: 0

    return 0;
}
