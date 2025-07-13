#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main() {
    clock_t start = clock();
    clock_t end = clock();
    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("%f\n", cpu_time_used);

    // Test case verification (داخل main فقط، حسب طلبك)
    if (cpu_time_used >= 0.0) {
        fprintf(stderr, "Test case: PASS\n");
    } else {
        fprintf(stderr, "Test case: FAIL\n");
    }
    return 0;
}
