#include <time.h>
#include <stdio.h>

int main() {
    clock_t start = clock();
    double seconds = (double)start / CLOCKS_PER_SEC;
    printf("Current processor time in seconds: %f\n", seconds);

    // تحقق التست كيس
    if (seconds >= 0.0) {
        printf("Test case: PASS\n");
    } else {
        printf("Test case: FAIL\n");
    }
    return 0;
}
