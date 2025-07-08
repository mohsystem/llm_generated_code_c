#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRINGS 100
#define MAX_LENGTH 1000

int main() {
    char strings[MAX_STRINGS][MAX_LENGTH];  // Array to store strings
    char result[MAX_STRINGS * MAX_LENGTH] = "";  // To store the concatenated result
    int count = 0;

    // Loop to receive input until the user presses Enter
    while (1) {
        printf("Enter a string (or press Enter to finish): ");
        fgets(strings[count], MAX_LENGTH, stdin);

        // Remove newline character
        strings[count][strcspn(strings[count], "\n")] = 0;

        if (strings[count][0] == '\0') {
            break;  // Exit if the user presses Enter without entering a string
        }

        count++;
        if (count >= MAX_STRINGS) {
            printf("Maximum number of strings reached.\n");
            break;
        }
    }

    // Concatenate all strings
    for (int i = 0; i < count; i++) {
        if (i > 0) {
            strcat(result, " ");  // Add space between strings
        }
        strcat(result, strings[i]);  // Append string to result
    }

    // Print the concatenated result
    printf("Concatenated string: %s\n", result);

    return 0;
}
