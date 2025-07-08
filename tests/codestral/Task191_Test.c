#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>

int longestValidParentheses(char *s) {
    int *stack = (int *)malloc(sizeof(int) * (strlen(s) + 1));
    int top = -1;
    stack[++top] = -1;  // Initialize stack with -1 to handle edge cases
    int maxLength = 0;

    for (int i = 0; i < strlen(s); i++) {
        if (s[i] == '(') {
            stack[++top] = i;  // Push index of '(' onto the stack
        } else {
            if (top > 0) {
                top--;  // Pop the stack for ')'
                maxLength = fmax(maxLength, i - stack[top]);  // Calculate valid substring length
            } else {
                stack[++top] = i;  // Reset stack if it is empty
            }
        }
    }

    free(stack);  // Free dynamically allocated memory
    return maxLength;
}

int main() {
    // Test cases
    printf("%d\n", longestValidParentheses("(()"));  // Output: 2
    printf("%d\n", longestValidParentheses(")()())"));  // Output: 4
    printf("%d\n", longestValidParentheses(""));  // Output: 0
    return 0;
}
