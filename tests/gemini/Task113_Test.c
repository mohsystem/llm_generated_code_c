#include <stdio.h>
#include <string.h>

int main() {
    int num_strings;
    char concatenated_string[1000] = "";  // Assuming a maximum concatenated string length

    // Ask the user for the number of strings they want to input
    printf("Enter the number of strings: ");
    scanf("%d", &num_strings);
    getchar();  // Consume the newline character left by scanf

    // Loop to collect the specified number of strings
    for (int i = 0; i < num_strings; i++) {
        char user_string[100];  // Assuming a maximum user string length
        printf("Enter string %d: ", i + 1);
        fgets(user_string, sizeof(user_string), stdin);  // Read user input
        user_string[strcspn(user_string, "\n")] = 0;  // Remove the newline character

        // Concatenate the user input to the existing string
        strcat(concatenated_string, user_string);
    }

    // Print the final concatenated string
    printf("Concatenated string: %s\n", concatenated_string);

    return 0;
}
