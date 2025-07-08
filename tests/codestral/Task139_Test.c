#include <stdio.h>
#include <math.h>
#include <assert.h>

double circle_area(double radius) {
    if (radius < 0) radius = -radius;  // تعامل مع السالب
    return M_PI * radius * radius;
}

int double_equals(double a, double b, double epsilon) {
    return fabs(a - b) < epsilon;
}

void run_tests() {
    double epsilon = 1e-9;

    assert(double_equals(circle_area(0), 0.0, epsilon));
    assert(double_equals(circle_area(1), M_PI * 1 * 1, epsilon));
    assert(double_equals(circle_area(2.5), M_PI * 2.5 * 2.5, epsilon));
    assert(double_equals(circle_area(10), M_PI * 10 * 10, epsilon));
    assert(double_equals(circle_area(-5), M_PI * 5 * 5, epsilon));
    assert(double_equals(circle_area(1000), M_PI * 1000 * 1000, epsilon));
    assert(double_equals(circle_area(3.1416), M_PI * 3.1416 * 3.1416, epsilon));
    assert(double_equals(circle_area(0.1), M_PI * 0.1 * 0.1, epsilon));
    assert(double_equals(circle_area(1234.567), M_PI * 1234.567 * 1234.567, epsilon));
    assert(double_equals(circle_area(9.99), M_PI * 9.99 * 9.99, epsilon));

    printf("✓ All tests passed for circle area calculation.\n");
}

int main() {
    run_tests();
    return 0;
}
