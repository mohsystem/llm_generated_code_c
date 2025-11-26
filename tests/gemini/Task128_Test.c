#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// الكود الأصلي محفوظ كما هو (main)

// === Test framework ===
#define ASSERT(cond, testname) do { \
if (cond) printf("%s: PASS\n", testname); \
else printf("%s: FAIL\n", testname); \
} while(0)

#define ASSERT_STR_IN_ARRAY(str, arr, arr_size, testname) do { \
int found = 0; \
for (int i = 0; i < arr_size; i++) { \
if (strcmp(str, arr[i]) == 0) { found = 1; break; } \
} \
if (found) printf("%s: PASS\n", testname); \
else printf("%s: FAIL\n", testname); \
} while(0)

#define ASSERT_IN_RANGE(val, min, max, testname) do { \
if ((val) >= (min) && (val) <= (max)) printf("%s: PASS\n", testname); \
else printf("%s: FAIL\n", testname); \
} while(0)

// === Test helpers ===
void seedRandForTest() {
    srand(12345);
}

// === Test cases ===

void test_random_integer_in_range() {
    seedRandForTest();
    int val = rand() % 10 + 1;
    ASSERT_IN_RANGE(val, 1, 10, "test_random_integer_in_range");
}

void test_random_double_in_0_1() {
    seedRandForTest();
    double val = (double)rand() / RAND_MAX;
    ASSERT(val >= 0.0 && val <= 1.0, "test_random_double_in_0_1");
}

void test_random_fruit_in_array() {
    seedRandForTest();
    char *fruits[] = {"apple", "banana", "cherry"};
    int index = rand() % 3;
    char *fruit = fruits[index];
    ASSERT_STR_IN_ARRAY(fruit, fruits, 3, "test_random_fruit_in_array");
}

int main() {
    test_random_integer_in_range();
    test_random_double_in_0_1();
    test_random_fruit_in_array();
    return 0;
}
