#include <stdio.h>
#include <stdlib.h>

// Clock struct
typedef struct {
    int minutes;
} Clock;

// Function to create a Clock instance based on hours and minutes
Clock createClock(int hours, int minutes) {
    Clock clock;
    clock.minutes = (hours * 60 + minutes) % (24 * 60);  // Ensure minutes are within a 24-hour range
    return clock;
}

// Function to check if two clocks are equal
int areClocksEqual(Clock c1, Clock c2) {
    return c1.minutes == c2.minutes;
}

// Function to add minutes to a clock
Clock addMinutes(Clock clock, int minutes) {
    Clock newClock = createClock(0, clock.minutes + minutes);
    return newClock;
}

// Function to subtract minutes from a clock
Clock subtractMinutes(Clock clock, int minutes) {
    Clock newClock = createClock(0, clock.minutes - minutes);
    return newClock;
}

// Function to print the time in HH:MM format
void printClock(Clock clock) {
    int hours = clock.minutes / 60;
    int minutes = clock.minutes % 60;
    printf("%02d:%02d\n", hours, minutes);
}

// Test cases

void test_initial_time() {
    Clock clock = createClock(10, 30);
    printf("Test initial time: ");
    printClock(clock);
}

void test_add_minutes() {
    Clock clock = createClock(10, 30);
    clock = addMinutes(clock, 60);
    printf("Test add minutes: ");
    printClock(clock);
}

void test_subtract_minutes() {
    Clock clock = createClock(10, 30);
    clock = subtractMinutes(clock, 90);
    printf("Test subtract minutes: ");
    printClock(clock);
}

void test_add_overflow_minutes() {
    Clock clock = createClock(23, 45);
    clock = addMinutes(clock, 30);
    printf("Test add overflow minutes: ");
    printClock(clock);
}

void test_subtract_underflow_minutes() {
    Clock clock = createClock(0, 15);
    clock = subtractMinutes(clock, 30);
    printf("Test subtract underflow minutes: ");
    printClock(clock);
}

void test_equality_true() {
    Clock clock1 = createClock(10, 0);
    Clock clock2 = createClock(10, 0);
    if (areClocksEqual(clock1, clock2)) {
        printf("Test equality true: Pass\n");
    } else {
        printf("Test equality true: Fail\n");
    }
}

void test_equality_false() {
    Clock clock1 = createClock(10, 0);
    Clock clock2 = createClock(9, 0);
    if (!areClocksEqual(clock1, clock2)) {
        printf("Test equality false: Pass\n");
    } else {
        printf("Test equality false: Fail\n");
    }
}

void test_midnight_edge_case() {
    Clock clock = createClock(0, 0);
    printf("Test midnight edge case: ");
    printClock(clock);
}

void test_full_day_cycle() {
    Clock clock = createClock(0, 0);
    clock = addMinutes(clock, 1440);  // 1440 minutes = 24 hours
    printf("Test full day cycle: ");
    printClock(clock);
}

void test_negative_overflow() {
    Clock clock = createClock(1, 0);
    clock = subtractMinutes(clock, 1440);  // Subtract 1440 minutes = 24 hours
    printf("Test negative overflow: ");
    printClock(clock);
}

int main() {
    // Run all test cases
    test_initial_time();
    test_add_minutes();
    test_subtract_minutes();
    test_add_overflow_minutes();
    test_subtract_underflow_minutes();
    test_equality_true();
    test_equality_false();
    test_midnight_edge_case();
    test_full_day_cycle();
    test_negative_overflow();

    return 0;
}
