#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

// كودك الأصلي
void log_event(const char *event_type, const char *message) {
    FILE *log_file = fopen("security_events.log", "a");
    if (log_file == NULL) {
        perror("Error opening log file");
        return;
    }

    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);

    fprintf(log_file, "%s - %s - %s\n", timestamp, event_type, message);
    fclose(log_file);
}

void monitor_file_changes(const char *file_path) {
    struct stat file_stat;
    if (stat(file_path, &file_stat) == -1) {
        log_event("Error", "Failed to get file status");
        return;
    }

    time_t last_modified_time = file_stat.st_mtime;

    while (1) {
        sleep(5); // Check every 5 seconds

        if (stat(file_path, &file_stat) == -1) {
            log_event("Error", "Failed to get file status");
            continue;
        }

        if (file_stat.st_mtime != last_modified_time) {
            log_event("File Change Detected", file_path);
            last_modified_time = file_stat.st_mtime;
        }
    }
}

// إطار الاختبارات
#define ASSERT(cond, testname) do { \
    if (cond) printf("%s: PASS\n", testname); \
    else printf("%s: FAIL\n", testname); \
} while(0)

#define PRINT_FAIL_NOT_IMPLEMENTED(testname) printf("%s: FAIL (Not Implemented)\n", testname)

// اختبارات

void test_log_event_basic() {
    log_event("LOGIN", "User admin logged in");
    FILE* f = fopen("security_events.log", "r");
    int found = 0;
    if (f) {
        char line[512];
        while (fgets(line, sizeof(line), f)) {
            if (strstr(line, "LOGIN") && strstr(line, "User admin logged in")) {
                found = 1;
                break;
            }
        }
        fclose(f);
    }
    ASSERT(found, "test_log_event_basic");
}

void test_monitor_file_changes_detection() {
    // غير قابل للتنفيذ تلقائياً بدون تعديل بيئة التشغيل أو الملف
    PRINT_FAIL_NOT_IMPLEMENTED("test_monitor_file_changes_detection");
}

void test_monitor_file_changes_missing_file() {
    // غير قابل للتنفيذ بدون وجود الملف أو التحكم بالبيئة
    PRINT_FAIL_NOT_IMPLEMENTED("test_monitor_file_changes_missing_file");
}

void test_infinite_loop_monitor() {
    // لا يمكن اختبار حلقة لا نهائية بشكل مباشر
    PRINT_FAIL_NOT_IMPLEMENTED("test_infinite_loop_monitor");
}

int main() {
    test_log_event_basic();
    test_monitor_file_changes_detection();
    test_monitor_file_changes_missing_file();
    test_infinite_loop_monitor();
    return 0;
}
