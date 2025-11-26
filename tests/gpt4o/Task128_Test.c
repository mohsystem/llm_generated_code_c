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

#define ASSERT_EQUALS(a,b,testname) do { \
if ((a) == (b)) printf("%s: PASS\n", testname); \
else printf("%s: FAIL\n", testname); \
} while(0)

#define ASSERT(cond, testname) do { \
if (cond) printf("%s: PASS\n", testname); \
else printf("%s: FAIL\n", testname); \
} while(0)

// === Test cases ===

// لضمان تكرارية الاختبارات نستخدم بذرة ثابتة
void seedRandForTest() {
    srand(12345);
}

// اختبار نفس الرقم عند نفس البذرة
void test_fixed_seed_same_number() {
    seedRandForTest();
    int num1 = generateRandomNumber();
    seedRandForTest();
    int num2 = generateRandomNumber();
    ASSERT_EQUALS(num1, num2, "test_fixed_seed_same_number");
}

// الرقم ضمن المدى 1-100
void test_number_in_range() {
    seedRandForTest();
    int num = generateRandomNumber();
    ASSERT_RANDOM_IN_RANGE(num, 1, 100, "test_number_in_range");
}

// تكرار توليد 1000 رقم ضمن المدى
void test_multiple_numbers_in_range() {
    seedRandForTest();
    int pass = 1;
    for (int i = 0; i < 1000; i++) {
        int num = generateRandomNumber();
        if (num < 1 || num > 100) {
            pass = 0;
            break;
        }
    }
    ASSERT(pass, "test_multiple_numbers_in_range");
}

// اختلاف الأرقام مع بذور مختلفة
void test_different_seeds_produce_different_numbers() {
    srand(1);
    int num1 = generateRandomNumber();
    srand(2);
    int num2 = generateRandomNumber();
    ASSERT(num1 != num2, "test_different_seeds_produce_different_numbers");
}

int main() {
    test_fixed_seed_same_number();
    test_number_in_range();
    test_multiple_numbers_in_range();
    test_different_seeds_produce_different_numbers();
    return 0;
}
