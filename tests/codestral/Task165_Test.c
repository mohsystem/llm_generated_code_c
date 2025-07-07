#include <stdio.h>
#include <string.h>

int checkFlush(char* table[], int tableSize, char* hand[], int handSize) {
    int suitCount[4] = {0};
    for (int i = 0; i < tableSize; i++) {
        char suit = table[i][strlen(table[i]) - 1];
        if (suit == 'S') suitCount[0]++;
        else if (suit == 'H') suitCount[1]++;
        else if (suit == 'D') suitCount[2]++;
        else if (suit == 'C') suitCount[3]++;
    }
    for (int i = 0; i < handSize; i++) {
        char suit = hand[i][strlen(hand[i]) - 1];
        if (suit == 'S') suitCount[0]++;
        else if (suit == 'H') suitCount[1]++;
        else if (suit == 'D') suitCount[2]++;
        else if (suit == 'C') suitCount[3]++;
    }
    for (int i = 0; i < 4; i++)
        if (suitCount[i] >= 5)
            return 1;
    return 0;
}

void runTest(char* table[], int tableSize, char* hand[], int handSize, int expected, int testNum) {
    int result = checkFlush(table, tableSize, hand, handSize);
    if (result == expected) {
        printf("Test case %d: PASS\n", testNum);
    } else {
        printf("Test case %d: FAIL (Expected %s but got %s)\n", testNum,
               expected ? "true" : "false", result ? "true" : "false");
    }
}

int main() {
    char* table1[] = {"A_S", "J_H", "7_D", "8_D", "10_D"};
    char* hand1[] = {"J_D", "3_D"};

    char* table2[] = {"10_S", "7_S", "9_H", "4_S", "3_S"};
    char* hand2[] = {"K_S", "Q_S"};

    char* table3[] = {"3_S", "10_H", "10_D", "10_C", "10_S"};
    char* hand3[] = {"3_S", "4_D"};

    char* table4[] = {"2_H", "3_H", "4_H", "5_H", "6_D"};
    char* hand4[] = {"7_H", "8_H"};

    char* table5[] = {"9_S", "8_H", "7_D", "6_C", "5_S"};
    char* hand5[] = {"4_D", "3_H"};

    char* table6[] = {"K_C", "Q_C", "J_C", "9_C", "2_H"};
    char* hand6[] = {"8_C", "7_C"};

    char* table7[] = {"A_S", "2_S", "3_S", "4_H", "5_H"};
    char* hand7[] = {"6_S", "7_H"};

    char* table8[] = {"2_D", "3_D", "4_D", "5_D", "6_D"};
    char* hand8[] = {"7_H", "8_H"};

    char* table9[] = {"A_S", "K_H", "Q_D", "J_C", "10_S"};
    char* hand9[] = {"9_H", "8_C"};

    char* table10[] = {"A_S", "K_S", "Q_S", "J_S", "10_S"};
    char* hand10[] = {"9_S", "8_S"};

    runTest(table1, 5, hand1, 2, 1, 1);
    runTest(table2, 5, hand2, 2, 1, 2);
    runTest(table3, 5, hand3, 2, 0, 3);
    runTest(table4, 5, hand4, 2, 1, 4);
    runTest(table5, 5, hand5, 2, 0, 5);
    runTest(table6, 5, hand6, 2, 1, 6);
    runTest(table7, 5, hand7, 2, 0, 7);
    runTest(table8, 5, hand8, 2, 1, 8);
    runTest(table9, 5, hand9, 2, 0, 9);
    runTest(table10, 5, hand10, 2, 1, 10);

    return 0;
}
