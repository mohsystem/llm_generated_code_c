#include <stdio.h>
#include <assert.h>
#include "../../source/output/claude/c/Task1_CLAUDE_claude-3-5-sonnet-20240620.c"  // Include the header for the function

// Helper function to run a test case
void run_test_case(const int stops[][2], int num_stops, int expected) {
    int result = numberOfPeopleOnBus(stops, num_stops);
    assert(result == expected);
    printf("Test passed with result: %d\n", result);
}

int main() {
    // Test Case 1: Basic scenario with a few stops
    int stops1[][2] = { {3, 0}, {2, 1}, {5, 2}, {1, 4} };
    run_test_case(stops1, 4, 4);

    // Test Case 2: More people get off than get on at some stops
    int stops2[][2] = { {10, 0}, {3, 5}, {2, 1}, {7, 10} };
    run_test_case(stops2, 4, 6);

    // Test Case 3: No people getting on or off the bus
    int stops3[][2] = { {0, 0}, {0, 0}, {0, 0}, {0, 0} };
    run_test_case(stops3, 4, 0);

    // Test Case 4: Mixed scenario with various numbers of people
    int stops4[][2] = { {5, 0}, {3, 2}, {8, 4}, {2, 1} };
    run_test_case(stops4, 4, 11);

    // Test Case 5: Some stops with no changes
    int stops5[][2] = { {7, 0}, {0, 0}, {5, 3}, {3, 7}, {4, 0} };
    run_test_case(stops5, 5, 9);

    // Test Case 6: Balancing out to zero at some stops
    int stops6[][2] = { {1, 0}, {0, 0}, {1, 1}, {0, 0}, {1, 0}, {0, 1} };
    run_test_case(stops6, 6, 1);

    // Test Case 7: Handling large numbers
    int stops7[][2] = { {100, 0}, {50, 30}, {25, 25}, {10, 15}, {0, 10} };
    run_test_case(stops7, 5, 105);

    // Test Case 8: Different numbers of people getting on and off
    int stops8[][2] = { {3, 0}, {0, 2}, {2, 1}, {0, 1}, {1, 0} };
    run_test_case(stops8, 5, 2);

    // Test Case 9: Bus ends up empty after the final stop
    int stops9[][2] = { {50, 0}, {0, 50} };
    run_test_case(stops9, 2, 0);

    // Test Case 10: Varied inputs with different stop values
    int stops10[][2] = { {6, 0}, {2, 3}, {1, 5}, {7, 2} };
    run_test_case(stops10, 4, 6);

    printf("All test cases passed!\n");
    return 0;
}