#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int longestValidParentheses(char* s) {
    int len = strlen(s);
    int* stack = (int*)malloc((len + 1) * sizeof(int));  // Allocate memory for stack
    int top = 0;
    int maxLength = 0;

    stack[top++] = -1;  // Initialize stack with -1 to handle edge cases

    for (int i = 0; i < len; i++) {
        if (s[i] == '(') {
            stack[top++] = i;  // Push index of '(' onto the stack
        } else {
            top--;  // Pop the stack for ')'
            if (top == 0) {
                stack[top++] = i;  // Reset stack if it is empty
            } else {
                maxLength = MAX(maxLength, i - stack[top - 1]);  // Calculate valid substring length
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
