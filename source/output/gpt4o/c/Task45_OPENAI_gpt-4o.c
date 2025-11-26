#include <stdio.h>
#include <stdlib.h>

int main() {
    int num, doubled;
    char input[10];

    printf("Enter a number: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    if (sscanf(input, "%d", &num) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }

    doubled = num * 2;
    printf("Doubled value: %d\n", doubled);

    return 0;
}
