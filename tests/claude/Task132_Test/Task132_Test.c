#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LENGTH 256

// الكود الأصلي محفوظ كما هو (logEvent + main)
void logEvent(const char* eventType, const char* description) {
    FILE* logFile = fopen("security_events.log", "a");
    if (logFile != NULL) {
        time_t now;
        time(&now);
        char* time_str = ctime(&now);
        time_str[strlen(time_str) - 1] = '\\0';  // Remove newline
        fprintf(logFile, "%s - INFO - Event Type: %s, Description: %s\\n",
                time_str, eventType, description);
        fclose(logFile);
    } else {
        fprintf(stderr, "Unable to open log file!\\n");
    }
}

// === Test framework ===
#define ASSERT(cond, testname) do { \
    if (cond) printf("%s: PASS\n", testname); \
    else printf("%s: FAIL\n", testname); \
} while(0)

#define PRINT_FAIL_NOT_IMPLEMENTED(testname) printf("%s: FAIL (Not Implemented)\n", testname)

// --- Helper: Check if last line in log file contains event ---
int check_log_contains(const char* eventType, const char* description) {
    FILE* fp = fopen("security_events.log", "r");
    if (!fp) return 0;
    char line[1024];
    char lastLine[1024] = "";
    while (fgets(line, sizeof(line), fp)) {
        strcpy(lastLine, line);
    }
    fclose(fp);
    return (strstr(lastLine, eventType) != NULL && strstr(lastLine, description) != NULL);
}

// === Test cases ===

void test_log_single_event() {
    const char* eventType = "LOGIN";
    const char* description = "User admin logged in";
    logEvent(eventType, description);
    ASSERT(check_log_contains(eventType, description), "test_log_single_event");
}

void test_log_multiple_events() {
    const char* eventType1 = "LOGIN";
    const char* description1 = "User admin logged in";
    const char* eventType2 = "LOGOUT";
    const char* description2 = "User admin logged out";
    logEvent(eventType1, description1);
    logEvent(eventType2, description2);
    ASSERT(check_log_contains(eventType2, description2), "test_log_multiple_events");
}

void test_log_empty_event() {
    const char* eventType = "";
    const char* description = "";
    logEvent(eventType, description);
    ASSERT(check_log_contains(eventType, description), "test_log_empty_event");
}

void test_log_long_event() {
    char eventType[MAX_LENGTH];
    char description[MAX_LENGTH];
    memset(eventType, 'A', MAX_LENGTH - 1);
    eventType[MAX_LENGTH - 1] = '\0';
    memset(description, 'B', MAX_LENGTH - 1);
    description[MAX_LENGTH - 1] = '\0';
    logEvent(eventType, description);
    ASSERT(check_log_contains(eventType, description), "test_log_long_event");
}

void test_log_file_unwritable() {
    // صعب محاكاة فتح ملف غير قابل للكتابة بدون تغيير النظام أو الأذونات
    PRINT_FAIL_NOT_IMPLEMENTED("test_log_file_unwritable");
}

int main() {
    test_log_single_event();
    test_log_multiple_events();
    test_log_empty_event();
    test_log_long_event();
    test_log_file_unwritable();
    return 0;
}
