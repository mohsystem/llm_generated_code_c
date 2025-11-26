#include <stdio.h>
#include <stdbool.h>

typedef struct {
    int hour;
    int minute;
} Clock;

// Normalize the clock to ensure minutes are between 0 and 59, and hours between 0 and 23
void normalize(Clock *clock) {
    if (clock->minute >= 60) {
        clock->hour += clock->minute / 60;
        clock->minute %= 60;
    }
    while (clock->minute < 0) {
        clock->hour -= 1;
        clock->minute += 60;
    }
    clock->hour = (clock->hour + 24) % 24;
}

// Create a clock with given hours and minutes
Clock createClock(int hour, int minute) {
    Clock clock = {hour, minute};
    normalize(&clock);
    return clock;
}

// Add minutes to the clock and normalize it
void addMinutes(Clock *clock, int minutes) {
    clock->minute += minutes;
    normalize(clock);
}

// Subtract minutes from the clock and normalize it
void subtractMinutes(Clock *clock, int minutes) {
    clock->minute -= minutes;
    normalize(clock);
}

// Check if two clocks are equal
bool areEqual(Clock clk1, Clock clk2) {
    return clk1.hour == clk2.hour && clk1.minute == clk2.minute;
}

// Print the time in HH:MM format
void printClock(Clock clock) {
    printf("%02d:%02d\n", clock.hour, clock.minute);
}

// Test cases for various scenarios

void test_initial_time() {
    Clock clock = createClock(10, 30);
    printf("Test initial time: ");
    printClock(clock);
}

void test_add_minutes() {
    Clock clock = createClock(10, 30);
    addMinutes(&clock, 90);  // 10:30 + 90 minutes = 12:00
    printf("Test add minutes: ");
    printClock(clock);
}

void test_subtract_minutes() {
    Clock clock = createClock(14, 30);
    subtractMinutes(&clock, 70);  // 14:30 - 70 minutes = 13:20
    printf("Test subtract minutes: ");
    printClock(clock);
}

void test_add_overflow_minutes() {
    Clock clock = createClock(23, 45);
    addMinutes(&clock, 30);  // 23:45 + 30 minutes = 00:15
    printf("Test add overflow minutes: ");
    printClock(clock);
}

void test_subtract_underflow_minutes() {
    Clock clock = createClock(0, 15);
    subtractMinutes(&clock, 30);  // 00:15 - 30 minutes = 23:45
    printf("Test subtract underflow minutes: ");
    printClock(clock);
}

void test_equality_true() {
    Clock clock1 = createClock(10, 0);
    Clock clock2 = createClock(10, 0);
    if (areEqual(clock1, clock2)) {
        printf("Test equality true: Pass\n");
    } else {
        printf("Test equality true: Fail\n");
    }
}

void test_equality_false() {
    Clock clock1 = createClock(10, 0);
    Clock clock2 = createClock(9, 0);
    if (!areEqual(clock1, clock2)) {
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
    addMinutes(&clock, 1440);  // 1440 minutes = 24 hours, should go back to 00:00
    printf("Test full day cycle: ");
    printClock(clock);
}

void test_negative_overflow() {
    Clock clock = createClock(1, 0);
    subtractMinutes(&clock, 1440);  // Subtract 1440 minutes = 24 hours, should go back to 01:00
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
