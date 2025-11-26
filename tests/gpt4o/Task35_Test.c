#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


// #include "../../source/output/claude/c/Task35_CLAUDE_claude-3-5-sonnet-20240620.c"  // Include the header for the function
// #include "../../source/output/codestral/c/Task35_MISTRAL_codestral-latest.c"  // Include the header for the function
//#include "../../source/output/gemini/c/Task35_GEMINI_gemini-1.5-pro-001.c"  // Include the header for the function
 #include "../../source/output/gpt4o/c/Task35_OPENAI_gpt-4o.c"  // Include the header for the function
// #include "../../source/output/llama-3/c/Task35_PERPLEXITY_llama-3-sonar-large-32k-chat.c"  // Include the header for the function


void testSingleStop() {
    // Test with only one stop where no one gets off
    Stop stops[] = {{10, 0}};
    assert(numberOfPeople(stops, 1) == 10);
}

void testAllGetOff() {
    // Test where everyone gets off at the last stop
    Stop stops[] = {{10, 0}, {3, 5}, {2, 10}};
    assert(numberOfPeople(stops, 3) == 0);
}

void testMoreGetOnThanOff() {
    // Test where more people get on the bus than get off at each stop
    Stop stops[] = {{5, 0}, {7, 3}, {4, 2}};
    assert(numberOfPeople(stops, 3) == 11);
}

void testSameNumberGetOnAndOff() {
    // Test where the same number of people get on and off the bus at each stop
    Stop stops[] = {{3, 0}, {2, 2}, {0, 0}};
    assert(numberOfPeople(stops, 3) == 3);
}

void testAllGetOnAtOnce() {
    // Test where all people get on at the first stop and none get off until the end
    Stop stops[] = {{20, 0}, {0, 0}, {0, 0}};
    assert(numberOfPeople(stops, 3) == 20);
}

void testDecreasingPassengers() {
    // Test where the number of people decreases gradually
    Stop stops[] = {{10, 0}, {0, 3}, {0, 5}, {0, 2}};
    assert(numberOfPeople(stops, 4) == 0);
}

void testEmptyBusStops() {
    // Test with stops that have zero people getting on and off
    Stop stops[] = {{10, 0}, {0, 0}, {0, 0}};
    assert(numberOfPeople(stops, 3) == 10);
}

void testOnePersonLeft() {
    // Test where one person remains on the bus after all stops
    Stop stops[] = {{1, 0}, {0, 0}};
    assert(numberOfPeople(stops, 2) == 1);
}

void testComplexScenario() {
    // Test with a complex scenario with varying numbers
    Stop stops[] = {{10, 0}, {3, 5}, {5, 8}, {10, 2}, {0, 3}, {7, 3}};
    assert(numberOfPeople(stops, 6) == 14);
}

void testLargeNumbers() {
    // Test with large numbers
    Stop stops[] = {{1000, 0}, {500, 300}, {200, 200}, {300, 100}};
    assert(numberOfPeople(stops, 4) == 1400);
}

int main() {
    // Running all test cases
    testSingleStop();
    testAllGetOff();
    testMoreGetOnThanOff();
    testSameNumberGetOnAndOff();
    testAllGetOnAtOnce();
    testDecreasingPassengers();
    testEmptyBusStops();
    testOnePersonLeft();
    testComplexScenario();
    testLargeNumbers();

    printf("All tests passed!\n");

    return 0;
}