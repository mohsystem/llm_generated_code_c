#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 10  // Define the buffer size

char buffer[BUFFER_SIZE];  // Declare the buffer
int buffer_index = 0;      // Initialize the index to track where the next character will go

// Function to add a character to the buffer
void add_char(char c) {
    if (buffer_index < BUFFER_SIZE) {
        buffer[buffer_index++] = c;  // Add the character to the buffer and increment the index
    } else {
        printf("Buffer is full.\n");  // Print message if the buffer is full
    }
}

// Function to print the contents of the buffer
void print_buffer() {
    for (int i = 0; i < buffer_index; ++i) {
        printf("%c", buffer[i]);  // Print each character in the buffer
    }
    printf("\n");  // Newline after printing the buffer content
}

int main() {
    printf("Enter characters to fill the buffer:\n");

    // Keep asking for characters until the buffer is full
    while (buffer_index < BUFFER_SIZE) {
        printf("Enter a character: ");
        char ch;
        scanf(" %c", &ch);  // Take a single character input
        add_char(ch);  // Add the character to the buffer
    }

    printf("Buffer contents:\n");
    print_buffer();  // Print the buffer contents

    return 0;
}
