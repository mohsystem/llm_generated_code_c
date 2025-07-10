#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// الكود الأصلي محفوظ كما هو:
int generateRandomNumber() {
    return rand() % 100 + 1;
}

// === Test framework ===
#define ASSERT_RANDOM_IN_RANGE(num, min, max, testname) do { \
if ((num) >= (min) && (num) <= (max)) printf("%s: PASS\n", testname); \
else printf("%s: FAIL\n", testname); \
} while(0)

// --- Test cases ---

// لتكرارية الاختبارات نستخدم بذرة ثابتة
void seedRandForTest() {
    srand(12345);
}

void test_random_number_in_range() {
    seedRandForTest();
    int num = generateRandomNumber();
    ASSERT_RANDOM_IN_RANGE(num, 1, 100, "test_random_number_in_range");
}

int main() {
    test_random_number_in_range();
    return 0;
}
