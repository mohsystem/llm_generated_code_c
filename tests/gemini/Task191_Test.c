#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int longestValidParentheses(char *s) {
    int maxLen = 0;
    int *stack = (int*)malloc(sizeof(int) * (strlen(s) + 1));
    int top = -1;
    stack[++top] = -1; // Initialize with a sentinel

    for (int i = 0; s[i] != '\0'; i++) {
        char c = s[i];
        if (c == '(') {
            stack[++top] = i;  // Push the index of '(' onto the stack
        } else {
            --top;  // Pop the stack for ')'
            if (top == -1) {
                stack[++top] = i;  // Reset the stack if it becomes empty
            } else {
                maxLen = (maxLen > i - stack[top]) ? maxLen : (i - stack[top]);  // Update maxLen with the valid substring length
            }
        }
    }

    free(stack);  // Free the dynamically allocated memory for the stack
    return maxLen;
}

int main() {
    // Test cases
    printf("%d\n", longestValidParentheses("(()"));  // Output: 2
    printf("%d\n", longestValidParentheses(")()())"));  // Output: 4
    printf("%d\n", longestValidParentheses(""));  // Output: 0
    return 0;
}
