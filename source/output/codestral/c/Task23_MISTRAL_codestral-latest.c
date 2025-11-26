#include <assert.h>
#include <math.h>
#include <stdio.h>

long long findNb(long long m) {
    long long n = 1;
    while (pow(n, 3) < m) {
        m -= pow(n, 3);
        n++;
    }
    return (m == pow(n, 3)) ? n : -1;
}
//
// int main() {
//     // Test Case 1
//     long long m1 = 1071225;
//     long long expected1 = 45;
//     assert(findNb(m1) == expected1);
//     printf("Test Case 1 passed!\n");
// }