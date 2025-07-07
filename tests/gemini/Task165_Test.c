#include <stdio.h>

int checkFlush(char table[5][4], char hand[2][4]) {
  int suits[4] = {0}; // S, H, D, C
  for (int i = 0; i < 5; i++) {
    if (table[i][2] == 'S') suits[0]++;
    else if (table[i][2] == 'H') suits[1]++;
    else if (table[i][2] == 'D') suits[2]++;
    else if (table[i][2] == 'C') suits[3]++;
  }
  for (int i = 0; i < 2; i++) {
    if (hand[i][2] == 'S') suits[0]++;
    else if (hand[i][2] == 'H') suits[1]++;
    else if (hand[i][2] == 'D') suits[2]++;
    else if (hand[i][2] == 'C') suits[3]++;
  }
  for (int i = 0; i < 4; i++) {
    if (suits[i] >= 5) {
      return 1; // true
    }
  }
  return 0; // false
}

void runTest(char table[5][4], char hand[2][4], int expected, int testNum) {
    int result = checkFlush(table, hand);
    if (result == expected) {
        printf("Test case %d: PASS\n", testNum);
    } else {
        printf("Test case %d: FAIL (Expected %s but got %s)\n", testNum,
            expected ? "true" : "false", result ? "true" : "false");
    }
}

int main() {
    char table1[5][4] = {"A_S", "J_H", "7_D", "8_D", "10_D"};
    char hand1[2][4] = {"J_D", "3_D"};

    char table2[5][4] = {"10_S", "7_S", "9_H", "4_S", "3_S"};
    char hand2[2][4] = {"K_S", "Q_S"};

    char table3[5][4] = {"3_S", "10_H", "10_D", "10_C", "10_S"};
    char hand3[2][4] = {"3_S", "4_D"};

    char table4[5][4] = {"2_H", "3_H", "4_H", "5_H", "6_D"};
    char hand4[2][4] = {"7_H", "8_H"};

    char table5[5][4] = {"9_S", "8_H", "7_D", "6_C", "5_S"};
    char hand5[2][4] = {"4_D", "3_H"};

    char table6[5][4] = {"K_C", "Q_C", "J_C", "9_C", "2_H"};
    char hand6[2][4] = {"8_C", "7_C"};

    char table7[5][4] = {"A_S", "2_S", "3_S", "4_H", "5_H"};
    char hand7[2][4] = {"6_S", "7_H"};

    char table8[5][4] = {"2_D", "3_D", "4_D", "5_D", "6_D"};
    char hand8[2][4] = {"7_H", "8_H"};

    char table9[5][4] = {"A_S", "K_H", "Q_D", "J_C", "10_S"};
    char hand9[2][4] = {"9_H", "8_C"};

    char table10[5][4] = {"A_S", "K_S", "Q_S", "J_S", "10_S"};
    char hand10[2][4] = {"9_S", "8_S"};

    runTest(table1, hand1, 1, 1);
    runTest(table2, hand2, 1, 2);
    runTest(table3, hand3, 0, 3);
    runTest(table4, hand4, 1, 4);
    runTest(table5, hand5, 0, 5);
    runTest(table6, hand6, 1, 6);
    runTest(table7, hand7, 0, 7);
    runTest(table8, hand8, 1, 8);
    runTest(table9, hand9, 0, 9);
    runTest(table10, hand10, 1, 10);

    return 0;
}
