#include <stdio.h>

struct PackedData {
    int value : 8;
    int flag1 : 1;
    int flag2 : 2;
    int value2 : 5;
};

int main() {
    struct PackedData pd;
    pd.value = 100;       // Assigning 8 bits value
    pd.flag1 = 1;         // Assigning 1 bit value
    pd.flag2 = 2;         // Assigning 2 bits value
    pd.value2 = 15;       // Assigning 5 bits value

    // Printing the packed data values
    printf("value: %d\n", pd.value);
    printf("flag1: %d\n", pd.flag1);
    printf("flag2: %d\n", pd.flag2);
    printf("value2: %d\n", pd.value2);

    return 0;
}
