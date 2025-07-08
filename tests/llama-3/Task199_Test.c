#include <stdio.h>

typedef struct {
    int hours;
    int minutes;
} Clock;

// Function to add minutes to a clock and handle overflow
void addMinutes(Clock* clock, int minutes) {
    clock->minutes += minutes;
    clock->hours += clock->minutes / 60;
    clock->hours %= 24;
    clock->minutes %= 60;
}

// Function to subtract minutes from a clock and handle underflow
void subtractMinutes(Clock* clock, int minutes) {
    clock->minutes -= minutes;
    if (clock->minutes < 0) {
        clock->hours -= 1;
        clock->minutes += 60;
    }
    clock->hours %= 24;
    clock->minutes %= 60;
}

// Function to check if two clocks are equal
int isEqual(const Clock* clock1, const Clock* clock2) {
    return clock1->hours == clock2->hours && clock1->minutes == clock2->minutes;
}

// Function to print the time in HH:MM format
void printClock(const Clock* clock) {
    printf("%02d:%02d\n", clock->hours, clock->minutes);
}

// Test cases

void test_initial_time() {
    Clock clock = {10, 30};
    printf("Test initial time: ");
    printClock(&clock);
}

void test_add_minutes() {
    Clock clock = {10, 30};
    addMinutes(&clock, 30);  // 10:30 + 30 minutes = 11:00
    printf("Test add minutes: ");
    printClock(&clock);
}

void test_subtract_minutes() {
    Clock clock = {14, 30};
    subtractMinutes(&clock, 90);  // 14:30 - 90 minutes = 13:00
    printf("Test subtract minutes: ");
    printClock(&clock);
}

void test_add_overflow_minutes() {
    Clock clock = {23, 45};
    addMinutes(&clock, 30);  // 23:45 + 30 minutes = 00:15
    printf("Test add overflow minutes: ");
    printClock(&clock);
}

void test_subtract_underflow_minutes() {
    Clock clock = {0, 15};
    subtractMinutes(&clock, 30);  // 00:15 - 30 minutes = 23:45
    printf("Test subtract underflow minutes: ");
    printClock(&clock);
}

void test_equality_true() {
    Clock clock1 = {10, 0};
    Clock clock2 = {10, 0};
    if (isEqual(&clock1, &clock2)) {
        printf("Test equality true: Pass\n");
    } else {
        printf("Test equality true: Fail\n");
    }
}

void test_equality_false() {
    Clock clock1 = {10, 0};
    Clock clock2 = {9, 0};
    if (!isEqual(&clock1, &clock2)) {
        printf("Test equality false: Pass\n");
    } else {
        printf("Test equality false: Fail\n");
    }
}

void test_midnight_edge_case() {
    Clock clock = {0, 0};
    printf("Test midnight edge case: ");
    printClock(&clock);
}

void test_full_day_cycle() {
    Clock clock = {0, 0};
    addMinutes(&clock, 1440);  // 1440 minutes = 24 hours, should go back to 00:00
    printf("Test full day cycle: ");
    printClock(&clock);
}

void test_negative_overflow() {
    Clock clock = {1, 0};
    subtractMinutes(&clock, 1440);  // Subtract 1440 minutes = 24 hours, should go back to 01:00
    printf("Test negative overflow: ");
    printClock(&clock);
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
