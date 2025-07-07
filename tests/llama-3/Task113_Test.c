#include <stdio.h>
#include <string.h>

int main() {
    int numStrings;
    printf("Enter the number of strings: ");
    scanf("%d", &numStrings);  // Read the number of strings
    char result[1000] = "";  // Initialize the result buffer to store concatenated strings

    // Loop to collect each string
    for (int i = 0; i < numStrings; i++) {
        char temp[100];  // Temporary buffer to store each string input
        printf("Enter string %d: ", i + 1);
        scanf("%s", temp);  // Read the string from user input
        strcat(result, temp);  // Concatenate the string to the result
        strcat(result, " ");  // Add a space after each string
    }

    // Remove the trailing space at the end of the result
    result[strlen(result) - 1] = '\0';

    // Print the final concatenated string
    printf("Concatenated string: %s\n", result);

    return 0;
}
