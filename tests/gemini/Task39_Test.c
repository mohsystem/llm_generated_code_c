#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILENAME 100
#define MAX_OUTPUT_SIZE 10000

// دالة تجمع محتوى الملف عبر الأمر cat باستخدام popen
int cat_file_and_capture(const char* filename, char* output, size_t max_output_size) {
    char command[150] = "cat ";
    strcat(command, filename);

    FILE* pipe = popen(command, "r");
    if (!pipe) {
        snprintf(output, max_output_size, "Error executing command\n");
        return 1;
    }

    output[0] = '\0';
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe)) {
        if (strlen(output) + strlen(buffer) < max_output_size) {
            strcat(output, buffer);
        } else {
            break;
        }
    }

    int status = pclose(pipe);
    if (status != 0) return 1;
    return 0;
}

// دالة لإنشاء ملف
int create_file(const char* filename, const char* content) {
    FILE* f = fopen(filename, "w");
    if (!f) return -1;
    fputs(content, f);
    fclose(f);
    return 0;
}

void run_tests() {
    struct {
        const char* filename;
        const char* file_content;  // NULL يعني الملف غير موجود
        const char* expected_output;
        const char* description;
    } tests[] = {
        {"test_file.txt", "Hello, World!\n", "Hello, World!\n", "Test 1: Existing file"},
        {"empty_file.txt", "", "", "Test 2: Empty file"},
        {"nonexistent.txt", NULL, "Error executing command\n", "Test 3: Non-existent file"},
        {"multiline.txt", "Line 1\nLine 2\nLine 3\n", "Line 1\nLine 2\nLine 3\n", "Test 4: Multi-line file"},
    };

    char output[MAX_OUTPUT_SIZE];
    int n = sizeof(tests) / sizeof(tests[0]);

    for (int i=0; i<n; i++) {
        if (tests[i].file_content == NULL) {
            remove(tests[i].filename);
        } else {
            if (create_file(tests[i].filename, tests[i].file_content) != 0) {
                printf("%s: FAIL (Cannot create file)\n", tests[i].description);
                continue;
            }
        }

        output[0] = '\0';
        int ret = cat_file_and_capture(tests[i].filename, output, sizeof(output));
        printf("%s... ", tests[i].description);

        if (ret == 0 && strcmp(output, tests[i].expected_output) == 0) {
            printf("PASS\n");
        } else if (ret != 0 && strcmp(tests[i].expected_output, "Error executing command\n") == 0) {
            printf("PASS\n");
        } else {
            printf("FAIL\nExpected:\n%sGot:\n%s\n", tests[i].expected_output, output);
        }
    }

    // تنظيف الملفات
    for (int i=0; i<n; i++) {
        remove(tests[i].filename);
    }
}

int main() {
    run_tests();
    return 0;
}
