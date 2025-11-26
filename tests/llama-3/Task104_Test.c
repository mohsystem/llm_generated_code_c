#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 10

int main() {
    char buffer[BUFFER_SIZE][256];  // Declare a 2D array for the buffer (each element can store up to 255 characters)
    int index = 0;  // Index to keep track of where to insert new input

    while (true) {
        printf("Enter a value (or 'quit' to exit): ");
        char userInput[256];  // Temporary variable to hold the user input
        fgets(userInput, 256, stdin);  // Read the user input
        userInput[strcspn(userInput, "\n")] = 0; // Remove newline character if present

        if (strcmp(userInput, "quit") == 0) {  // Check if the user wants to exit
            break;
        }

        // If buffer is not full, insert the new input at the current index
        if (index < BUFFER_SIZE) {
            strcpy(buffer[index], userInput);
            index++;
        } else {
            // If buffer is full, replace the oldest entry and shift others
            printf("Buffer is full. Oldest value will be replaced.\n");
            for (int i = 0; i < BUFFER_SIZE - 1; i++) {
                strcpy(buffer[i], buffer[i + 1]);  // Shift elements to the left
            }
            strcpy(buffer[BUFFER_SIZE - 1], userInput);  // Insert new value at the last position
        }

        // Display the buffer contents
        printf("Buffer: ");
        for (int i = 0; i < BUFFER_SIZE; i++) {
            if (strlen(buffer[i]) > 0) {  // Only print non-empty values
                printf("%s ", buffer[i]);
            }
        }
        printf("\n");
    }

    return 0;
}
