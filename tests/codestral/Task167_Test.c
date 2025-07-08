#include <stdio.h>
#include <math.h>
#include <stdbool.h>

double cubeDiagonal(double volume) {
    double sideLength = cbrt(volume);
    double diagonalLength = round(sideLength * sqrt(3) * 100.0) / 100.0;
    return diagonalLength;
}

bool approxEqual(double a, double b, double epsilon) {
    return fabs(a - b) < epsilon;
}

void runTest(double volume, double expected, int testNum) {
    double result = cubeDiagonal(volume);
    if (approxEqual(result, expected, 0.01)) {
        printf("Test %d: PASS\n", testNum);
    } else {
        printf("Test %d: FAIL (Expected: %.2f, Got: %.2f)\n", testNum, expected, result);
    }
}

int main() {
    runTest(8, 3.46, 1);
    runTest(343, 12.12, 2);
    runTest(1157.625, 18.19, 3);
    runTest(27, 5.20, 4);
    runTest(1, 1.73, 5);
    runTest(64, 6.93, 6);
    runTest(216, 10.39, 7);
    runTest(1000, 17.32, 8);
    runTest(0.125, 0.87, 9);
    runTest(512, 13.86, 10);

    return 0;
}
