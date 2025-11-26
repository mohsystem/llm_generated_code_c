#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct {
    double radius;
    double x_center;
    double y_center;
} Solution;

Solution* solutionCreate(double radius, double x_center, double y_center) {
    Solution* sol = (Solution*) malloc(sizeof(Solution));
    sol->radius = radius;
    sol->x_center = x_center;
    sol->y_center = y_center;
    srand(time(0));  // Seed the random number generator
    return sol;
}

void randPoint(Solution* sol, double *point) {
    double r = sol->radius * sqrt((double) rand() / RAND_MAX);  // Random radius
    double theta = 2 * M_PI * ((double) rand() / RAND_MAX);  // Random angle
    point[0] = sol->x_center + r * cos(theta);  // x-coordinate
    point[1] = sol->y_center + r * sin(theta);  // y-coordinate
}

// Function to check if the point is inside the circle
int isPointInsideCircle(double x, double y, double radius, double x_center, double y_center) {
    return (x - x_center) * (x - x_center) + (y - y_center) * (y - y_center) <= radius * radius;
}

// Run the test case
void test_case(double radius, double x_center, double y_center, const char* description) {
    Solution* sol = solutionCreate(radius, x_center, y_center);
    double point[2];
    for (int i = 0; i < 100; i++) {
        randPoint(sol, point);
        double x = point[0];
        double y = point[1];
        // Check if the point is inside the circle
        if (isPointInsideCircle(x, y, radius, x_center, y_center)) {
            printf("Test case (%s): Pass\n", description);
        } else {
            printf("Test case (%s): Fail\n", description);
        }
    }
    free(sol);
}

int main() {
    // Run the test cases
    test_case(1.0, 0.0, 0.0, "randPoint_within_circle");
    test_case(1.0, 1.0, 1.0, "randPoint_center_offset");
    test_case(100.0, 0.0, 0.0, "randPoint_large_radius");
    test_case(0.1, 0.0, 0.0, "randPoint_small_radius");
    test_case(1.0, -5.0, -5.0, "randPoint_negative_center");
    test_case(1.0, 1e6, 1e6, "randPoint_large_center");
    test_case(0.0, 0.0, 0.0, "randPoint_zero_radius");
    test_case(1e8, 0.0, 0.0, "randPoint_high_radius");
    test_case(10.0, 10.0, 10.0, "randPoint_non_zero_center");
    test_case(50.0, 0.0, 0.0, "randPoint_multiple_calls");

    return 0;
}
