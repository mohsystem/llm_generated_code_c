#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// الكود الأصلي محفوظ كما هو:
int generateRandomNumber(int minVal, int maxVal) {
    return rand() % (maxVal - minVal + 1) + minVal;
}

void generateRandomToken(char *token, int length) {
    const char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int charactersLength = strlen(characters);

    for (int i = 0; i < length; i++) {
        token[i] = characters[rand() % charactersLength];
    }
    token[length] = '\0';
}

// === Test framework ===
#define ASSERT(cond, testname) do { \
    if (cond) printf("%s: PASS\n", testname); \
    else printf("%s: FAIL\n", testname); \
} while(0)

#define ASSERT_STR_LEN(str, len, testname) do { \
    if ((int)strlen(str) == (len)) printf("%s: PASS\n", testname); \
    else printf("%s: FAIL\n", testname); \
} while(0)

#define ASSERT_RANDOM_IN_RANGE(num, min, max, testname) do { \
    if ((num) >= (min) && (num) <= (max)) printf("%s: PASS\n", testname); \
    else printf("%s: FAIL\n", testname); \
} while(0)

// === Test cases ===

// نستخدم قيمة ثابتة للبذرة لضمان تكرار النتائج في الاختبارات
void seedRandForTest() {
    srand(12345);
}

void test_generateRandomNumber_in_range() {
    seedRandForTest();
    int num = generateRandomNumber(1, 100);
    ASSERT_RANDOM_IN_RANGE(num, 1, 100, "test_generateRandomNumber_in_range");
}

void test_generateRandomToken_length() {
    seedRandForTest();
    char token[21];
    generateRandomToken(token, 20);
    ASSERT_STR_LEN(token, 20, "test_generateRandomToken_length");
}

void test_generateRandomToken_characters() {
    seedRandForTest();
    char token[11];
    generateRandomToken(token, 10);
    const char allowed[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int valid = 1;
    for (int i = 0; i < 10; i++) {
        if (strchr(allowed, token[i]) == NULL) {
            valid = 0;
            break;
        }
    }
    ASSERT(valid, "test_generateRandomToken_characters");
}

int main() {
    test_generateRandomNumber_in_range();
    test_generateRandomToken_length();
    test_generateRandomToken_characters();
    return 0;
}
