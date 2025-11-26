#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int maxProduct(char *input, int span) {
    int maxProd = 0;
    int len = strlen(input);
    if (span > len || span <= 0) return -1;
    for (int i = 0; i <= len - span; i++) {
        char series[span + 1];
        strncpy(series, input + i, span);
        series[span] = '\0';
        int product = 1;
        for (int j = 0; j < span; j++) {
            product *= series[j] - '0';
        }
        if (product > maxProd) {
            maxProd = product;
        }
    }
    return maxProd;
}

void test_case(char* input, int span, int expected, int case_num) {
    int result = maxProduct(input, span);
    if (result == expected) {
        printf("Test case %d: PASS\n", case_num);
    } else {
        printf("Test case %d: FAIL (Expected %d, Got %d)\n", case_num, expected, result);
    }
}

int main() {
    test_case("63915", 3, 162, 1);
    test_case("123456789", 2, 72, 2);
    test_case("00000", 3, 0, 3);
    test_case("987654321", 4, 3024, 4);
    test_case("11111", 5, 1, 5);
    test_case("123", 1, 3, 6);
    test_case("99999", 5, 59049, 7);
    test_case("56789", 2, 72, 8);
    test_case("1234567890", 5, 15120, 9);
    test_case("1", 1, 1, 10);

    return 0;
}
