#include <stdio.h>
#include <string.h>

int main() {
    char inputStr1[100], inputStr2[100], result[200];

    // Read the first string
    printf("Enter first string: ");
    fgets(inputStr1, sizeof(inputStr1), stdin);

    // Read the second string
    printf("Enter second string: ");
    fgets(inputStr2, sizeof(inputStr2), stdin);

    // Remove newline character from the input strings if present
    inputStr1[strcspn(inputStr1, "\n")] = 0;
    inputStr2[strcspn(inputStr2, "\n")] = 0;

    // Concatenate the two strings into result
    strcpy(result, inputStr1);
    strcat(result, inputStr2);

    // Print the concatenated result
    printf("Concatenated result: %s\n", result);

    return 0;
}
