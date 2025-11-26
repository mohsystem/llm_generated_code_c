#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 10

int main() {
    char userInput[BUFFER_SIZE + 1];  // Allocate space for the buffer, including the null terminator
    printf("Enter a string: ");
    fgets(userInput, BUFFER_SIZE + 1, stdin);  // Read up to BUFFER_SIZE characters

    // If the input exceeds the buffer size, truncate it
    if (strlen(userInput) > BUFFER_SIZE) {
        userInput[BUFFER_SIZE] = '\0';  // Ensure the string is null-terminated
    }

    // Print the truncated input
    printf("Truncated input: %s", userInput);
    return 0;
}
