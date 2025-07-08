#include <stdio.h>

typedef struct {
    int minutes;
} Clock;

// Function to create a Clock instance based on hours and minutes
Clock create_clock(int hour, int minute) {
    Clock c;
    c.minutes = (minute + hour * 60) % (24 * 60);  // Ensure minutes are within a 24-hour range
    if (c.minutes < 0) {
        c.minutes += 24 * 60;  // Correct negative minute overflow
    }
    return c;
}

// Function to print the clock time in HH:MM format
void print_clock(Clock c) {
    printf("%02d:%02d", c.minutes / 60, c.minutes % 60);
}

// Function to check if two clocks are equal
int are_equal(Clock c1, Clock c2) {
    return c1.minutes == c2.minutes;
}

// Function to add minutes to the clock
Clock add_minutes(Clock c, int minutes) {
    return create_clock(0, c.minutes + minutes);
}

// Function to subtract minutes from the clock
Clock subtract_minutes(Clock c, int minutes) {
    return create_clock(0, c.minutes - minutes);
}

// Test cases to match the Python test cases

void test_initial_time() {
    Clock clock = create_clock(10, 30);
    printf("Test initial time: ");
    print_clock(clock);
    printf("\n");
}

void test_add_minutes() {
    Clock clock = create_clock(10, 30);
    clock = add_minutes(clock, 60);
    printf("Test add minutes: ");
    print_clock(clock);
    printf("\n");
}

void test_subtract_minutes() {
    Clock clock = create_clock(10, 30);
    clock = subtract_minutes(clock, 90);
    printf("Test subtract minutes: ");
    print_clock(clock);
    printf("\n");
}

void test_add_overflow_minutes() {
    Clock clock = create_clock(23, 45);
    clock = add_minutes(clock, 30);
    printf("Test add overflow minutes: ");
    print_clock(clock);
    printf("\n");
}

void test_subtract_underflow_minutes() {
    Clock clock = create_clock(0, 15);
    clock = subtract_minutes(clock, 30);
    printf("Test subtract underflow minutes: ");
    print_clock(clock);
    printf("\n");
}

void test_equality_true() {
    Clock clock1 = create_clock(10, 0);
    Clock clock2 = create_clock(10, 0);
    if (are_equal(clock1, clock2)) {
        printf("Test equality true: Pass\n");
    } else {
        printf("Test equality true: Fail\n");
    }
}

void test_equality_false() {
    Clock clock1 = create_clock(10, 0);
    Clock clock2 = create_clock(9, 0);
    if (!are_equal(clock1, clock2)) {
        printf("Test equality false: Pass\n");
    } else {
        printf("Test equality false: Fail\n");
    }
}

void test_midnight_edge_case() {
    Clock clock = create_clock(0, 0);
    printf("Test midnight edge case: ");
    print_clock(clock);
    printf("\n");
}

void test_full_day_cycle() {
    Clock clock = create_clock(0, 0);
    clock = add_minutes(clock, 1440);  // 1440 minutes = 24 hours
    printf("Test full day cycle: ");
    print_clock(clock);
    printf("\n");
}

void test_negative_overflow() {
    Clock clock = create_clock(1, 0);
    clock = subtract_minutes(clock, 1440);  // Subtract 1440 minutes = 24 hours
    printf("Test negative overflow: ");
    print_clock(clock);
    printf("\n");
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
