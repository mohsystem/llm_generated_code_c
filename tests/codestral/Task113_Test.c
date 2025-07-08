#include <stdio.h>
#include <string.h>

#define MAX_LENGTH 100  // Maximum length for input and concatenated string

int main() {
    char concatenatedString[MAX_LENGTH] = "";  // Initialize an empty concatenated string
    char userInput[MAX_LENGTH];  // Temporary buffer for user input

    while (1) {
        printf("Enter a string (or press enter to finish): ");
        fgets(userInput, MAX_LENGTH, stdin);  // Read user input

        userInput[strcspn(userInput, "\n")] = 0;  // Remove trailing newline character
        if (strlen(userInput) == 0) {  // Exit if user presses enter without input
            break;
        }

        // Check if the concatenated string will exceed MAX_LENGTH
        if (strlen(concatenatedString) + strlen(userInput) < MAX_LENGTH) {
            strcat(concatenatedString, userInput);  // Concatenate the input string
        } else {
            printf("Error: Concatenated string exceeds the maximum length!\n");
            break;
        }
    }

    // Print the final concatenated string
    printf("Concatenated string: %s\n", concatenatedString);

    return 0;
}
