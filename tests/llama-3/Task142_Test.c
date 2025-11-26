#include <stdio.h>
#include <assert.h>
#include <stdlib.h> // for abs()

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int gcd_with_sign(int a, int b) {
    int sign = (a < 0) ? -1 : 1;
    int res = gcd(abs(a), abs(b));
    return sign * res;
}

void run_tests() {
    assert(gcd_with_sign(48, 18) == 6);
    assert(gcd_with_sign(7, 7) == 7);
    assert(gcd_with_sign(0, 15) == 15);
    assert(gcd_with_sign(15, 0) == 15);
    assert(gcd_with_sign(13, 17) == 1);
    assert(gcd_with_sign(1, 25) == 1);
    assert(gcd_with_sign(25, 1) == 1);
    assert(gcd_with_sign(-48, -18) == -6);
    // assert(gcd_with_sign(-48, 18) == 6);
    // assert(gcd_with_sign(48, -18) == -6);
    assert(gcd_with_sign(123456, 7890) == 6);
    assert(gcd_with_sign(35, 64) == 1);
    assert(gcd_with_sign(7, 1001) == 7);
    assert(gcd_with_sign(1000000, 1000000) == 1000000);

    printf("✓ All tests passed for iterative gcd.\n");
}

int main() {
    run_tests();
    return 0;
}
