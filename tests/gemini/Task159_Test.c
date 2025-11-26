#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int runTest(char** buffer, int size, int index, const char* expected) {
    if (index == -1) {
        // Exit condition
        printf("Test with index -1: Exit\n");
        return 1;
    }
    if (index < 0 || index >= size) {
        printf("Test with index %d: Index out of range.\n", index);
        return 1;  // Pass if handled correctly
    }
    printf("Test with index %d: Data at index: %s\n", index, buffer[index]);
    if (strcmp(buffer[index], expected) == 0) {
        printf("Test PASSED\n");
        return 1;
    } else {
        printf("Test FAILED (expected '%s')\n", expected);
        return 0;
    }
}

int main() {
    int size = 5;  // ثابت لاختبارات صارمة
    char* testInputs[] = {
        "Hello",
        "World",
        "Test123",
        "!@#$%",
        "Data"
    };

    // تخصيص المصفوفة الديناميكية
    char** buffer = (char**)malloc(size * sizeof(char*));
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    // ملأ البيانات
    for (int i = 0; i < size; i++) {
        buffer[i] = (char*)malloc(100 * sizeof(char));
        if (buffer[i] == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            return 1;
        }
        strcpy(buffer[i], testInputs[i]);
    }

    // مصفوفة التيست كيسز: كل عنصر = {index, expected_string}
    struct {
        int index;
        const char* expected;
    } testCases[] = {
        {0, "Hello"},
        {1, "World"},
        {2, "Test123"},
        {3, "!@#$%"},
        {4, "Data"},
        {5, NULL},        // غير صالح - أكبر من الحجم
        {-1, NULL},       // خروج
        {-5, NULL},       // غير صالح - سالب
    };

    int totalTests = sizeof(testCases) / sizeof(testCases[0]);
    int passed = 0;

    for (int i = 0; i < totalTests; i++) {
        int idx = testCases[i].index;
        const char* exp = testCases[i].expected ? testCases[i].expected : "";
        passed += runTest(buffer, size, idx, exp);
        if (idx == -1) break;  // توقف عند اختبار الخروج
    }

    printf("\nPassed %d/%d tests.\n", passed, totalTests);

    // تحرير الذاكرة
    for (int i = 0; i < size; i++) {
        free(buffer[i]);
    }
    free(buffer);

    return 0;
}
