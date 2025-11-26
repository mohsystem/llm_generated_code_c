#include <stdio.h>
#include <string.h>

int checkFlush(const char* table[], const char* hand[], size_t table_len, size_t hand_len) {
    int suits[4] = {0}; // 0: S, 1: H, 2: D, 3: C

    for (size_t i = 0; i < table_len; i++) {
        if (strstr(table[i], "_S") != NULL) suits[0]++;
        else if (strstr(table[i], "_H") != NULL) suits[1]++;
        else if (strstr(table[i], "_D") != NULL) suits[2]++;
        else if (strstr(table[i], "_C") != NULL) suits[3]++;
    }

    for (size_t i = 0; i < hand_len; i++) {
        if (strstr(hand[i], "_S") != NULL) suits[0]++;
        else if (strstr(hand[i], "_H") != NULL) suits[1]++;
        else if (strstr(hand[i], "_D") != NULL) suits[2]++;
        else if (strstr(hand[i], "_C") != NULL) suits[3]++;
    }

    for (int i = 0; i < 4; i++) {
        if (suits[i] >= 5) return 1;
    }

    return 0;
}

void runTest(const char* table[], const char* hand[], size_t table_len, size_t hand_len, int expected, int testNum) {
    int result = checkFlush(table, hand, table_len, hand_len);
    if (result == expected) {
        printf("Test case %d: PASS\n", testNum);
    } else {
        printf("Test case %d: FAIL (Expected %s but got %s)\n", testNum,
            expected ? "true" : "false", result ? "true" : "false");
    }
}

int main() {
    const char* table1[] = {"A_S", "J_H", "7_D", "8_D", "10_D"};
    const char* hand1[] = {"J_D", "3_D"};

    const char* table2[] = {"10_S", "7_S", "9_H", "4_S", "3_S"};
    const char* hand2[] = {"K_S", "Q_S"};

    const char* table3[] = {"3_S", "10_H", "10_D", "10_C", "10_S"};
    const char* hand3[] = {"3_S", "4_D"};

    const char* table4[] = {"2_H", "3_H", "4_H", "5_H", "6_D"};
    const char* hand4[] = {"7_H", "8_H"};

    const char* table5[] = {"9_S", "8_H", "7_D", "6_C", "5_S"};
    const char* hand5[] = {"4_D", "3_H"};

    const char* table6[] = {"K_C", "Q_C", "J_C", "9_C", "2_H"};
    const char* hand6[] = {"8_C", "7_C"};

    const char* table7[] = {"A_S", "2_S", "3_S", "4_H", "5_H"};
    const char* hand7[] = {"6_S", "7_H"};

    const char* table8[] = {"2_D", "3_D", "4_D", "5_D", "6_D"};
    const char* hand8[] = {"7_H", "8_H"};

    const char* table9[] = {"A_S", "K_H", "Q_D", "J_C", "10_S"};
    const char* hand9[] = {"9_H", "8_C"};

    const char* table10[] = {"A_S", "K_S", "Q_S", "J_S", "10_S"};
    const char* hand10[] = {"9_S", "8_S"};

    runTest(table1, hand1, 5, 2, 1, 1);
    runTest(table2, hand2, 5, 2, 1, 2);
    runTest(table3, hand3, 5, 2, 0, 3);
    runTest(table4, hand4, 5, 2, 1, 4);
    runTest(table5, hand5, 5, 2, 0, 5);
    runTest(table6, hand6, 5, 2, 1, 6);
    runTest(table7, hand7, 5, 2, 0, 7);
    runTest(table8, hand8, 5, 2, 1, 8);
    runTest(table9, hand9, 5, 2, 0, 9);
    runTest(table10, hand10, 5, 2, 1, 10);

    return 0;
}
