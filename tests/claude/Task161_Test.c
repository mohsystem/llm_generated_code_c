#include <stdio.h>
#include <stdlib.h>

// Function to return the maximum of two integers
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Knapsack function to calculate the maximum value that can be put into the knapsack
int knapsack(int capacity, int* weights, int* values, int n) {
    // Create a DP table to store results of subproblems
    int** dp = (int**)malloc((n + 1) * sizeof(int*));
    for (int i = 0; i <= n; i++) {
        dp[i] = (int*)calloc(capacity + 1, sizeof(int));
    }

    // Fill the DP table using the recursive formula
    for (int i = 1; i <= n; i++) {
        for (int w = 1; w <= capacity; w++) {
            if (weights[i - 1] <= w) {
                dp[i][w] = max(values[i - 1] + dp[i - 1][w - weights[i - 1]], dp[i - 1][w]);
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    int result = dp[n][capacity];

    // Free the allocated memory for DP table
    for (int i = 0; i <= n; i++) {
        free(dp[i]);
    }
    free(dp);

    return result;
}

int main() {
    int capacity, n;

    // Input knapsack capacity and number of items
    printf("Enter knapsack capacity: ");
    scanf("%d", &capacity);

    printf("Enter number of items: ");
    scanf("%d", &n);

    // Allocate memory for weights and values arrays
    int* weights = (int*)malloc(n * sizeof(int));
    int* values = (int*)malloc(n * sizeof(int));

    // Input weights of items
    printf("Enter weights of items: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &weights[i]);
    }

    // Input values of items
    printf("Enter values of items: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &values[i]);
    }

    // Call knapsack function and print the result
    printf("Maximum value: %d\n", knapsack(capacity, weights, values, n));

    // Free the allocated memory for weights and values arrays
    free(weights);
    free(values);

    return 0;
}
