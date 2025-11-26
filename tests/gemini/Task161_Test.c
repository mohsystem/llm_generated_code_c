#include <stdio.h>

int max(int a, int b) {
    return (a > b) ? a : b;
}

int knapsack(int capacity, int weights[], int values[], int n) {
    int dp[n + 1][capacity + 1];

    for (int i = 0; i <= n; i++) {
        for (int w = 0; w <= capacity; w++) {
            if (i == 0 || w == 0) {
                dp[i][w] = 0;
            } else if (weights[i - 1] <= w) {
                dp[i][w] = max(values[i - 1] + dp[i - 1][w - weights[i - 1]], dp[i - 1][w]);
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    return dp[n][capacity];
}

void runTests() {
    // Test Case 1
    int values1[] = {60, 100, 120};
    int weights1[] = {10, 20, 30};
    int capacity1 = 50;
    printf("Test Case 1 - Expected: 220, Got: %d\n", knapsack(capacity1, weights1, values1, 3));

    // Test Case 2 - Exact fit scenario
    int values2[] = {60, 100, 120};
    int weights2[] = {10, 20, 30};
    int capacity2 = 30;
    printf("Test Case 2 - Expected: 160, Got: %d\n", knapsack(capacity2, weights2, values2, 3));

    // Test Case 3 - One item
    int values3[] = {60};
    int weights3[] = {10};
    int capacity3 = 10;
    printf("Test Case 3 - Expected: 60, Got: %d\n", knapsack(capacity3, weights3, values3, 1));

    // Test Case 4 - No items fit
    int values4[] = {60, 100, 120};
    int weights4[] = {10, 20, 30};
    int capacity4 = 5;
    printf("Test Case 4 - Expected: 0, Got: %d\n", knapsack(capacity4, weights4, values4, 3));

    // Test Case 5 - Full Capacity
    int values5[] = {10, 20, 30};
    int weights5[] = {10, 20, 30};
    int capacity5 = 60;
    printf("Test Case 5 - Expected: 60, Got: %d\n", knapsack(capacity5, weights5, values5, 3));

    // Test Case 6 - All items too large
    int values6[] = {10, 20, 30};
    int weights6[] = {40, 50, 60};
    int capacity6 = 10;
    printf("Test Case 6 - Expected: 0, Got: %d\n", knapsack(capacity6, weights6, values6, 3));
}

int main() {
    runTests();
    return 0;
}
