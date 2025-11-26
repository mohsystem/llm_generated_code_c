#include <stdio.h>

struct BitFields {
    unsigned char bits : 3;
};

int main() {
    struct BitFields bf;
    bf.bits = 5;  // Assigning a value within the 3-bit range (0-7)
    printf("%d\n", bf.bits);  // Printing the value of the 3-bit field
    return 0;
}
