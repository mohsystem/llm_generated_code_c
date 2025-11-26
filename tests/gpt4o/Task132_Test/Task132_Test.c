#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_EVENTS 100

typedef struct {
    char event[256];
    char timestamp[20];
} SecurityEvent;

typedef struct {
    SecurityEvent events[MAX_EVENTS];
    int count;
} SecurityEventLogger;

void currentDateTime(char* buffer) {
    time_t now = time(NULL);
    struct tm* tstruct = localtime(&now);
    strftime(buffer, 20, "%Y-%m-%d %H:%M:%S", tstruct);
}

void logEvent(SecurityEventLogger* logger, const char* event) {
    if (logger->count >= MAX_EVENTS) {
        printf("Event log is full\n");
        return;
    }
    strcpy(logger->events[logger->count].event, event);
    currentDateTime(logger->events[logger->count].timestamp);
    logger->count++;
}

void showEvents(const SecurityEventLogger* logger) {
    for (int i = 0; i < logger->count; i++) {
        printf("%s - %s\n", logger->events[i].timestamp, logger->events[i].event);
    }
}

// === Test framework ===
#define ASSERT(cond, testname) do { \
    if (cond) printf("%s: PASS\n", testname); \
    else printf("%s: FAIL\n", testname); \
} while(0)

#define PRINT_FAIL_NOT_IMPLEMENTED(testname) printf("%s: FAIL (Not Implemented)\n", testname)

// === Test cases ===

void test_log_single_event() {
    SecurityEventLogger logger = {.count = 0};
    logEvent(&logger, "Test event");
    ASSERT(logger.count == 1, "test_log_single_event");
    ASSERT(strcmp(logger.events[0].event, "Test event") == 0, "test_log_single_event_content");
}

void test_log_multiple_events() {
    SecurityEventLogger logger = {.count = 0};
    logEvent(&logger, "Event 1");
    logEvent(&logger, "Event 2");
    ASSERT(logger.count == 2, "test_log_multiple_events");
    ASSERT(strcmp(logger.events[1].event, "Event 2") == 0, "test_log_multiple_events_content");
}

void test_log_event_overflow() {
    SecurityEventLogger logger = {.count = MAX_EVENTS};
    logEvent(&logger, "Overflow event");
    ASSERT(logger.count == MAX_EVENTS, "test_log_event_overflow");
}

void test_show_events_output() {
    // في C من الصعب التقاط printf بسهولة بدون إعادة توجيه
    PRINT_FAIL_NOT_IMPLEMENTED("test_show_events_output");
}

int main() {
    test_log_single_event();
    test_log_multiple_events();
    test_log_event_overflow();
    test_show_events_output();
    return 0;
}
