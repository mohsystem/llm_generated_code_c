#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER_SIZE 20
#define INPUT_SIZE 1000

int main() {
    char buffer[BUFFER_SIZE + 1] = {0};  // Buffer initialized to an empty string.
    char input[INPUT_SIZE];

    while (true) {
        printf("Enter text (or 'q' to quit): ");

        // Read input from the user
        if (fgets(input, INPUT_SIZE, stdin) == NULL) {
            break;
        }

        // Remove newline character from input if present
        input[strcspn(input, "\n")] = 0;

        // Check if user wants to quit
        if (strcmp(input, "q") == 0 || strcmp(input, "Q") == 0) {
            break;
        }

        // Check if the input fits in the buffer
        if (strlen(buffer) + strlen(input) <= BUFFER_SIZE) {
            strcat(buffer, input);  // Concatenate input to the buffer
            printf("Buffer content: %s\n", buffer);
        } else {
            printf("Error: Buffer overflow. Cannot add more than %d characters.\n", BUFFER_SIZE);
        }
    }

    printf("Final buffer content: %s\n", buffer);
    return 0;
}
