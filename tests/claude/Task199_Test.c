#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int hours;
    int minutes;
} Clock;

// Function to create a new clock instance
Clock* Clock_create(int hours, int minutes) {
    Clock* clock = (Clock*)malloc(sizeof(Clock));
    clock->hours = hours % 24;  // Ensure hours is within the 24-hour range
    clock->minutes = minutes % 60;  // Ensure minutes is within the 60-minute range
    return clock;
}

// Function to print time in "HH:MM" format
void Clock_print(Clock* clock) {
    printf("%02d:%02d", clock->hours, clock->minutes);
}

// Operator for adding minutes to the clock
void Clock_add_minutes(Clock* clock, int minutes) {
    clock->minutes += minutes;
    while (clock->minutes >= 60) {
        clock->minutes -= 60;
        clock->hours++;
        if (clock->hours >= 24) {
            clock->hours = 0;
        }
    }
    while (clock->minutes < 0) {
        clock->minutes += 60;
        clock->hours--;
        if (clock->hours < 0) {
            clock->hours = 23;
        }
    }
}

// Operator for subtracting minutes from the clock
void Clock_subtract_minutes(Clock* clock, int minutes) {
    Clock_add_minutes(clock, -minutes);
}

// Equality check for two clocks
bool Clock_equals(Clock* clock1, Clock* clock2) {
    return clock1->hours == clock2->hours && clock1->minutes == clock2->minutes;
}

// Function to free the memory of the clock
void Clock_free(Clock* clock) {
    free(clock);
}

// Test functions to match the Python test cases

void test_initial_time() {
    Clock* clock = Clock_create(10, 30);
    printf("Test initial time: ");
    Clock_print(clock);
    printf("\n");
    Clock_free(clock);
}

void test_add_minutes() {
    Clock* clock = Clock_create(10, 30);
    Clock_add_minutes(clock, 60);
    printf("Test add minutes: ");
    Clock_print(clock);
    printf("\n");
    Clock_free(clock);
}

void test_subtract_minutes() {
    Clock* clock = Clock_create(10, 30);
    Clock_subtract_minutes(clock, 90);
    printf("Test subtract minutes: ");
    Clock_print(clock);
    printf("\n");
    Clock_free(clock);
}

void test_add_overflow_minutes() {
    Clock* clock = Clock_create(23, 45);
    Clock_add_minutes(clock, 30);
    printf("Test add overflow minutes: ");
    Clock_print(clock);
    printf("\n");
    Clock_free(clock);
}

void test_subtract_underflow_minutes() {
    Clock* clock = Clock_create(0, 15);
    Clock_subtract_minutes(clock, 30);
    printf("Test subtract underflow minutes: ");
    Clock_print(clock);
    printf("\n");
    Clock_free(clock);
}

void test_equality_true() {
    Clock* clock1 = Clock_create(10, 0);
    Clock* clock2 = Clock_create(10, 0);
    if (Clock_equals(clock1, clock2)) {
        printf("Test equality true: Pass\n");
    } else {
        printf("Test equality true: Fail\n");
    }
    Clock_free(clock1);
    Clock_free(clock2);
}

void test_equality_false() {
    Clock* clock1 = Clock_create(10, 0);
    Clock* clock2 = Clock_create(9, 0);
    if (!Clock_equals(clock1, clock2)) {
        printf("Test equality false: Pass\n");
    } else {
        printf("Test equality false: Fail\n");
    }
    Clock_free(clock1);
    Clock_free(clock2);
}

void test_midnight_edge_case() {
    Clock* clock = Clock_create(0, 0);
    printf("Test midnight edge case: ");
    Clock_print(clock);
    printf("\n");
    Clock_free(clock);
}

void test_full_day_cycle() {
    Clock* clock = Clock_create(0, 0);
    Clock_add_minutes(clock, 1440);  // 1440 minutes = 24 hours
    printf("Test full day cycle: ");
    Clock_print(clock);
    printf("\n");
    Clock_free(clock);
}

void test_negative_overflow() {
    Clock* clock = Clock_create(1, 0);
    Clock_subtract_minutes(clock, 1440);  // Subtract 1440 minutes = 24 hours
    printf("Test negative overflow: ");
    Clock_print(clock);
    printf("\n");
    Clock_free(clock);
}

int main() {
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
