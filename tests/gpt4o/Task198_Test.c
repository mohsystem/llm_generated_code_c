#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int rand7() {
    return rand() % 7 + 1;
}

int rand10() {
    while (1) {
        int num = (rand7() - 1) * 7 + rand7();  // Generate a number between 1 and 49
        if (num <= 40) {
            return (num - 1) % 10 + 1;  // Map to 1-10
        }
    }
}

// Test case for uniform distribution
void test_rand10_uniform_distribution(int n) {
    int counts[10] = {0};
    for (int i = 0; i < n; i++) {
        int value = rand10();
        counts[value - 1]++;
    }
    for (int value = 1; value <= 10; value++) {
        if (!(counts[value - 1] >= n / 15 && counts[value - 1] <= n / 7)) {
            printf("Test rand10_uniform_distribution: Fail\n");
            return;
        }
    }
    printf("Test rand10_uniform_distribution: Pass\n");
}

// Test case for single call results between 1-10
void test_rand10_single_call() {
    int possible_results[10] = {0};
    for (int i = 0; i < 100; i++) {
        int value = rand10();
        possible_results[value - 1] = 1;
    }
    for (int value = 1; value <= 10; value++) {
        if (!possible_results[value - 1]) {
            printf("Test rand10_single_call: Fail\n");
            return;
        }
    }
    printf("Test rand10_single_call: Pass\n");
}

// Test case to ensure multiple calls of rand10() return valid results between 1-10
void test_rand10_multiple_calls() {
    for (int i = 0; i < 10; i++) {
        int result = rand10();
        if (result < 1 || result > 10) {
            printf("Test rand10_multiple_calls: Fail\n");
            return;
        }
    }
    printf("Test rand10_multiple_calls: Pass\n");
}

// Test case to check performance of rand10
void test_rand10_performance() {
    clock_t start_time = clock();
    int n = 1000000;
    for (int i = 0; i < n; i++) {
        rand10();
    }
    clock_t elapsed_time = clock() - start_time;
    if ((double)elapsed_time / CLOCKS_PER_SEC > 2) {
        printf("Test rand10_performance: Fail\n");
        return;
    }
    printf("Test rand10_performance: Pass\n");
}

// Test case to check for consistency of rand10()
void test_rand10_consistency() {
    int first_result = rand10();
    for (int i = 0; i < 100; i++) {
        int result = rand10();
        if (result < 1 || result > 10) {
            printf("Test rand10_consistency: Fail\n");
            return;
        }
    }
    printf("Test rand10_consistency: Pass\n");
}

// Test case to check boundary conditions for rand10()
void test_rand10_boundary_conditions() {
    for (int i = 0; i < 1000; i++) {
        int result = rand10();
        if (result < 1 || result > 10) {
            printf("Test rand10_boundary_conditions: Fail\n");
            return;
        }
    }
    printf("Test rand10_boundary_conditions: Pass\n");
}

// Test case for randomness - ensures all values 1 to 10 are generated
void test_rand10_randomness() {
    int result_set[10] = {0};
    for (int i = 0; i < 1000; i++) {
        int result = rand10();
        result_set[result - 1] = 1;
    }
    for (int i = 0; i < 10; i++) {
        if (result_set[i] == 0) {
            printf("Test rand10_randomness: Fail\n");
            return;
        }
    }
    printf("Test rand10_randomness: Pass\n");
}

// Test case for extreme values - checks if 1 and 10 can be generated
void test_rand10_extreme_values() {
    for (int i = 0; i < 1000; i++) {
        int result = rand10();
        if (result < 1 || result > 10) {
            printf("Test rand10_extreme_values: Fail\n");
            return;
        }
    }
    printf("Test rand10_extreme_values: Pass\n");
}

// Test case to check multiple executions of rand10
void test_rand10_multiple_executions() {
    for (int i = 0; i < 100; i++) {
        int result = rand10();
        if (result < 1 || result > 10) {
            printf("Test rand10_multiple_executions: Fail\n");
            return;
        }
    }
    printf("Test rand10_multiple_executions: Pass\n");
}

// Test case to check edge cases for rand10 (should be able to generate 1 and 10)
void test_rand10_edge_case_check() {
    int edge_cases[2] = {0};
    for (int i = 0; i < 10000; i++) {
        int result = rand10();
        if (result == 1 || result == 10) {
            edge_cases[result - 1] = 1;
        }
    }
    if (edge_cases[0] == 0 || edge_cases[1] == 0) {
        printf("Test rand10_edge_case_check: Fail\n");
        return;
    }
    printf("Test rand10_edge_case_check: Pass\n");
}

int main() {
    int n = 100000;  // Number of iterations for testing

    // Run all test cases
    test_rand10_uniform_distribution(n);
    test_rand10_single_call();
    test_rand10_multiple_calls();
    test_rand10_performance();
    test_rand10_consistency();
    test_rand10_boundary_conditions();
    test_rand10_randomness();
    test_rand10_extreme_values();
    test_rand10_multiple_executions();
    test_rand10_edge_case_check();

    return 0;
}
