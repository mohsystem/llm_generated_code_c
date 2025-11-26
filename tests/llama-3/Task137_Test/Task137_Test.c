#include <stdio.h>
#include <string.h>

typedef struct {
    char name[20];
    int price;
    int stock;
} Product;

Product productTable[] = {
    {"apple", 10, 100},
    {"banana", 5, 200},
    {"orange", 15, 50}
};

int main() {
    char productName[20];
    while (1) {
        printf("Enter product name (or 'q' to quit): ");
        if (scanf("%19s", productName) != 1) {
            // Input failure
            break;
        }
        if (strcmp(productName, "q") == 0) {
            // Exit on 'q'
            break;
        }

        int i;
        int found = 0;
        for (i = 0; i < sizeof(productTable) / sizeof(Product); i++) {
            if (strcmp(productTable[i].name, productName) == 0) {
                printf("Product: %s\n", productTable[i].name);
                printf("Price: %d\n", productTable[i].price);
                printf("Stock: %d\n", productTable[i].stock);
                found = 1;
                printf("Test case: PASS\n");
                break;
            }
        }
        if (!found) {
            printf("Product not found\n");
            printf("Test case: FAIL\n");
        }
    }
    return 0;
}
