#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

typedef struct {
    double radius;
    double x_center;
    double y_center;
} Solution;

Solution* solutionCreate(double radius, double x_center, double y_center) {
    Solution* obj = (Solution*) malloc(sizeof(Solution));
    obj->radius = radius;
    obj->x_center = x_center;
    obj->y_center = y_center;
    srand(time(NULL));  // Seed the random number generator
    return obj;
}

double* solutionRandPoint(Solution* obj, int* retSize) {
    double* point = (double*) malloc(2 * sizeof(double));
    double angle = ((double)rand() / RAND_MAX) * 2 * M_PI;  // Random angle between 0 and 2 * pi
    double r = sqrt(((double)rand() / RAND_MAX)) * obj->radius;  // Random radius
    point[0] = obj->x_center + r * cos(angle);  // x-coordinate
    point[1] = obj->y_center + r * sin(angle);  // y-coordinate
    *retSize = 2;
    return point;
}

void solutionFree(Solution* obj) {
    free(obj);
}

// Function to check if the point is inside the circle
int isPointInsideCircle(double x, double y, double radius, double x_center, double y_center) {
    return (x - x_center) * (x - x_center) + (y - y_center) * (y - y_center) <= radius * radius;
}

// Run the test case
void test_case(double radius, double x_center, double y_center, const char* description) {
    Solution* solution = solutionCreate(radius, x_center, y_center);
    int retSize;
    for (int i = 0; i < 100; i++) {
        double* point = solutionRandPoint(solution, &retSize);
        double x = point[0];
        double y = point[1];
        // Check if the point is inside the circle
        if (isPointInsideCircle(x, y, radius, x_center, y_center)) {
            printf("Test case (%s): Pass\n", description);
        } else {
            printf("Test case (%s): Fail\n", description);
        }
        free(point);
    }
    solutionFree(solution);
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
