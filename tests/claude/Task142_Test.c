#include <stdio.h>
#include <assert.h>
#include <stdlib.h>  // for abs()

// دالة حساب GCD مع الحفاظ على إشارة النتيجة حسب القيم
int gcd(int a, int b) {
    // لو a أو b سالبين، نطبق الحل حسب سلوك اختبارك:
    // عند السالب نحتفظ بالإشارة على a فقط (كما في اختبارك)
    int sign = (a < 0) ? -1 : 1;
    a = abs(a);
    b = abs(b);

    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return sign * a;
}

void run_tests() {
    // Positive integers
    assert(gcd(48, 18) == 6);

    // Same numbers
    assert(gcd(7, 7) == 7);

    // One number zero
    assert(gcd(0, 15) == 15);
    assert(gcd(15, 0) == 15);

    // Prime numbers
    assert(gcd(13, 17) == 1);

    // One number is one
    assert(gcd(1, 25) == 1);
    assert(gcd(25, 1) == 1);

    // Negative numbers (مراعاة إشارة a فقط)
    assert(gcd(-48, -18) == -6);
    assert(gcd(-48, 18) == 6);
    assert(gcd(48, -18) == -6);

    // Large numbers
    assert(gcd(123456, 7890) == 6);

    // Coprime numbers
    assert(gcd(35, 64) == 1);

    // Small and large numbers
    assert(gcd(7, 1001) == 7);

    // Identical large numbers
    assert(gcd(1000000, 1000000) == 1000000);

    printf("✓ All tests passed for gcd function.\n");
}

int main() {
    run_tests();
    return 0;
}
