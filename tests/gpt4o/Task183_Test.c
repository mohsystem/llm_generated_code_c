#include <stdio.h>
#include <string.h>

int findLargestProduct(const char* sequence, int span) {
    int maxProduct = 0;
    int length = strlen(sequence);

    if (span > length || span <= 0) {
        return 0; // أو -1 بناء على متطلبات
    }

    for (int i = 0; i <= length - span; i++) {
        int product = 1;
        for (int j = 0; j < span; j++) {
            product *= sequence[i + j] - '0';
        }
        if (product > maxProduct) {
            maxProduct = product;
        }
    }
    return maxProduct;
}

void test_case(const char* sequence, int span, int expected, int case_num) {
    int result = findLargestProduct(sequence, span);
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
