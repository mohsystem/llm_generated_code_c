#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_CARDS 7

bool checkFlush(char table[][4], int tableSize, char hand[][4], int handSize) {
    int suits[4] = {0}; // S=0, H=1, D=2, C=3

    for (int i = 0; i < tableSize; i++) {
        char suit = table[i][2];
        switch(suit) {
            case 'S': suits[0]++; break;
            case 'H': suits[1]++; break;
            case 'D': suits[2]++; break;
            case 'C': suits[3]++; break;
        }
    }

    for (int i = 0; i < handSize; i++) {
        char suit = hand[i][2];
        switch(suit) {
            case 'S': suits[0]++; break;
            case 'H': suits[1]++; break;
            case 'D': suits[2]++; break;
            case 'C': suits[3]++; break;
        }
    }

    for (int i = 0; i < 4; i++) {
        if (suits[i] >= 5) {
            return true;
        }
    }
    return false;
}

void runTest(char table[][4], int tableSize, char hand[][4], int handSize, bool expected, int testNum) {
    bool result = checkFlush(table, tableSize, hand, handSize);
    if (result == expected) {
        printf("Test case %d: PASS\n", testNum);
    } else {
        printf("Test case %d: FAIL (Expected %s but got %s)\n", testNum, expected ? "true" : "false", result ? "true" : "false");
    }
}

int main() {
    char table1[][4] = {"A_S", "J_H", "7_D", "8_D", "10_D"};
    char hand1[][4] = {"J_D", "3_D"};

    char table2[][4] = {"10_S", "7_S", "9_H", "4_S", "3_S"};
    char hand2[][4] = {"K_S", "Q_S"};

    char table3[][4] = {"3_S", "10_H", "10_D", "10_C", "10_S"};
    char hand3[][4] = {"3_S", "4_D"};

    char table4[][4] = {"2_H", "3_H", "4_H", "5_H", "6_D"};
    char hand4[][4] = {"7_H", "8_H"};

    char table5[][4] = {"9_S", "8_H", "7_D", "6_C", "5_S"};
    char hand5[][4] = {"4_D", "3_H"};

    char table6[][4] = {"K_C", "Q_C", "J_C", "9_C", "2_H"};
    char hand6[][4] = {"8_C", "7_C"};

    char table7[][4] = {"A_S", "2_S", "3_S", "4_H", "5_H"};
    char hand7[][4] = {"6_S", "7_H"};

    char table8[][4] = {"2_D", "3_D", "4_D", "5_D", "6_D"};
    char hand8[][4] = {"7_H", "8_H"};

    char table9[][4] = {"A_S", "K_H", "Q_D", "J_C", "10_S"};
    char hand9[][4] = {"9_H", "8_C"};

    char table10[][4] = {"A_S", "K_S", "Q_S", "J_S", "10_S"};
    char hand10[][4] = {"9_S", "8_S"};

    runTest(table1, 5, hand1, 2, true, 1);
    runTest(table2, 5, hand2, 2, true, 2);
    runTest(table3, 5, hand3, 2, false, 3);
    runTest(table4, 5, hand4, 2, true, 4);
    runTest(table5, 5, hand5, 2, false, 5);
    runTest(table6, 5, hand6, 2, true, 6);
    runTest(table7, 5, hand7, 2, false, 7);
    runTest(table8, 5, hand8, 2, true, 8);
    runTest(table9, 5, hand9, 2, false, 9);
    runTest(table10, 5, hand10, 2, true, 10);

    return 0;
}
