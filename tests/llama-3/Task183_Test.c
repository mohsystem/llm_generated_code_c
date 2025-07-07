#include <stdio.h>
#include <string.h>

int largestProduct(const char* inputStr, int span) {
    int maxProduct = 0;
    int len = strlen(inputStr);
    if (span > len || span <= 0) return 0; // حماية من الحالات غير الصحيحة

    for (int i = 0; i <= len - span; i++) {
        int product = 1;
        for (int j = 0; j < span; j++) {
            product *= inputStr[i + j] - '0';
        }
        if (product > maxProduct) {
            maxProduct = product;
        }
    }
    return maxProduct;
}

void test(const char* inputStr, int span, int expected, int caseNum) {
    int result = largestProduct(inputStr, span);
    if (result == expected) {
        printf("Test case %d: PASS\n", caseNum);
    } else {
        printf("Test case %d: FAIL (Expected %d, Got %d)\n", caseNum, expected, result);
    }
}

int main() {
    test("63915", 3, 162, 1);
    test("123456789", 2, 72, 2);
    test("00000", 3, 0, 3);
    test("987654321", 4, 3024, 4);
    test("11111", 5, 1, 5);
    test("123", 1, 3, 6);
    test("99999", 5, 59049, 7);
    test("56789", 2, 72, 8);
    test("1234567890", 5, 15120, 9);
    test("1", 1, 1, 10);
    return 0;
}
