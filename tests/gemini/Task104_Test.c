#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1024

// Function to handle user input and ensure buffer size is respected
int handleUserInput(char *buffer) {
    printf("Enter your input (max %d characters): ", BUFFER_SIZE);

    // Get the user input with fgets
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
        return 0;  // Error while reading input
    }

    // Remove the trailing newline character from the input
    buffer[strcspn(buffer, "\n")] = 0;

    return 1;  // Success
}

int main() {
    char buffer[BUFFER_SIZE];  // Create a buffer of fixed size

    // Handle the user input
    if (handleUserInput(buffer)) {
        // If the input is successfully handled, print the result
        printf("\nYou entered:\n%s\n", buffer);
    } else {
        // In case of an error
        printf("Error reading input\n");
    }

    return 0;
}

