#include <stdio.h>
#include <time.h>

int main() {
    clock_t start = clock();
    // Do something here to use some CPU time
    clock_t end = clock();
    double processorTimeSeconds = (double)(end - start) / CLOCKS_PER_SEC;
    printf("C: %f\n", processorTimeSeconds);

    // تحقق التست كيس
    if (processorTimeSeconds >= 0.0) {
        printf("Test case: PASS\n");
    } else {
        printf("Test case: FAIL\n");
    }
    return 0;
}
