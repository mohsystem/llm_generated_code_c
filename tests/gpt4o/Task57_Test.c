#include <stdio.h>

long long factorial_iter(int n) {
    long long factorial = 1;
    for (int i = 1; i <= n; i++) {
        factorial *= i;
    }
    return factorial;
}

int main() {
    int pass = 0, fail = 0;
    long long res;

    // Test 1: factorial(0) == 1 (loop 1 to 0 لن تنفذ، نعتبر 1)
    res = factorial_iter(0);
    if (res == 1) { printf("Test 1: PASS\n"); pass++; } else { printf("Test 1: FAIL\n"); fail++; }

    // Test 2: factorial(1) == 1
    res = factorial_iter(1);
    if (res == 1) { printf("Test 2: PASS\n"); pass++; } else { printf("Test 2: FAIL\n"); fail++; }

    // Test 3: factorial(2) == 2
    res = factorial_iter(2);
    if (res == 2) { printf("Test 3: PASS\n"); pass++; } else { printf("Test 3: FAIL\n"); fail++; }

    // Test 4: factorial(3) == 6
    res = factorial_iter(3);
    if (res == 6) { printf("Test 4: PASS\n"); pass++; } else { printf("Test 4: FAIL\n"); fail++; }

    // Test 5: factorial(4) == 24
    res = factorial_iter(4);
    if (res == 24) { printf("Test 5: PASS\n"); pass++; } else { printf("Test 5: FAIL\n"); fail++; }

    // Test 6: factorial(5) == 120
    res = factorial_iter(5);
    if (res == 120) { printf("Test 6: PASS\n"); pass++; } else { printf("Test 6: FAIL\n"); fail++; }

    // Test 7: factorial(6) == 720
    res = factorial_iter(6);
    if (res == 720) { printf("Test 7: PASS\n"); pass++; } else { printf("Test 7: FAIL\n"); fail++; }

    // Test 8: factorial(10) == 3628800
    res = factorial_iter(10);
    if (res == 3628800) { printf("Test 8: PASS\n"); pass++; } else { printf("Test 8: FAIL\n"); fail++; }

    // Test 9: factorial(-1) -- not supported, will fail
    printf("Test 9: FAIL (negative input not supported)\n");
    fail++;

    // Test 10: factorial(string input) -- not supported in C scanf %d only
    printf("Test 10: FAIL (string input not supported)\n");
    fail++;

    printf("\nTotal Passed: %d\nTotal Failed: %d\n", pass, fail);

    return 0;
}
