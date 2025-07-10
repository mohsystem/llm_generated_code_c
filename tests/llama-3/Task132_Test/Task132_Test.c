#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// الكود الأصلي محفوظ كما هو (logEvent + monitorEvents + main)

// === Test framework ===
#define ASSERT(cond, testname) do { \
if (cond) printf("%s: PASS\n", testname); \
else printf("%s: FAIL\n", testname); \
} while(0)

#define PRINT_FAIL_NOT_IMPLEMENTED(testname) printf("%s: FAIL (Not Implemented)\n", testname)
void logEvent(const char* event) {
    FILE* logFile = fopen("security_log.txt", "a");
    if (logFile != NULL) {
        time_t now = time(NULL);
        fprintf(logFile, "%s - %s\n", ctime(&now), event);
        fclose(logFile);
    } else {
        fprintf(stderr, "Error writing to log file.\n");
    }
}
// === Test cases ===

// اختبار تسجيل حدث واحد
void test_log_single_event() {
    // مسجل يدويًا (لأن monitorEvents تعتمد على input لا يمكن أتمتتها بسهولة)
    logEvent("Test event");
    FILE* f = fopen("security_log.txt", "r");
    int found = 0;
    if (f) {
        char line[512];
        while (fgets(line, sizeof(line), f)) {
            if (strstr(line, "Test event")) {
                found = 1;
                break;
            }
        }
        fclose(f);
    }
    ASSERT(found, "test_log_single_event");
}

// اختبار إدخال 'quit' لإنهاء الحلقة (غير قابل للاختبار تلقائيًا)
void test_monitor_quit_input() {
    PRINT_FAIL_NOT_IMPLEMENTED("test_monitor_quit_input");
}

// اختبار إدخال حدث فارغ (غير قابل للاختبار تلقائيًا بدون تعديل)
void test_monitor_empty_event() {
    PRINT_FAIL_NOT_IMPLEMENTED("test_monitor_empty_event");
}

// اختبار فتح ملف اللوج غير ممكن (تحتاج تعديل صلاحيات النظام)
void test_log_file_open_error() {
    PRINT_FAIL_NOT_IMPLEMENTED("test_log_file_open_error");
}

int main() {
    test_log_single_event();
    test_monitor_quit_input();
    test_monitor_empty_event();
    test_log_file_open_error();
    return 0;
}
