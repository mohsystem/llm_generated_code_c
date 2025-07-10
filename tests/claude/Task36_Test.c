#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1000

// دالة تقوم بمحاكاة سلوك برنامج قراءة الملف وطباعة المحتوى
int read_file_content(const char* filename, char* output, size_t max_output_size) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        snprintf(output, max_output_size, "Error: Unable to open file '%s'\n", filename);
        return 1;
    }

    output[0] = '\0';
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        if (strlen(output) + strlen(line) >= max_output_size) {
            // تجنب تجاوز buffer
            break;
        }
        strcat(output, line);
    }

    fclose(file);
    return 0;
}

// دالة مساعدة لإنشاء ملف بمحتوى معين
int create_file(const char* filename, const char* content) {
    FILE *f = fopen(filename, "w");
    if (!f) return -1;
    fputs(content, f);
    fclose(f);
    return 0;
}

void run_tests() {
    struct {
        const char* filename;
        const char* file_content;  // محتوى الملف
        const char* expected_output; // النص المتوقع طباعته (بما في ذلك السطور الجديدة)
        const char* description;
    } tests[] = {
        {"test_file.txt", "Hello, World!", "Hello, World!", "test_read_file_success"},
        {"missing_file.txt", NULL, "Error: Unable to open file 'missing_file.txt'\n", "test_file_not_found"},
        {"test_file.txt", NULL, "Error: Unable to open file 'test_file.txt'\n", "test_io_error_simulated"}, // نستخدم نفس اسم ملف ولكن نجعله غير قابل للفتح محاكاة
        {"content_file.txt", "File Content", "File Content", "test_read_file_content"},
        {"empty_file.txt", "", "", "test_read_empty_file"},
        {"numeric_file.txt", "12345", "12345", "test_numeric_content"},
        {"special_chars.txt", "Special Characters !@#$%^&*()", "Special Characters !@#$%^&*()", "test_special_characters"},
        {"multiline_file.txt", "Line 1\nLine 2", "Line 1\nLine 2", "test_multiline_content"},
        {"trailing_spaces.txt", "Trailing spaces    ", "Trailing spaces    ", "test_trailing_spaces"},
        {"newlines_file.txt", "File\nWith\nNewlines", "File\nWith\nNewlines", "test_content_with_newlines"},
    };

    char output[10000];
    int n = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < n; i++) {
        // إذا محتوى الملف NULL، نحذف الملف (للمحاكاة أو للتأكد أنه غير موجود)
        if (tests[i].file_content == NULL) {
            remove(tests[i].filename);
        } else {
            if (create_file(tests[i].filename, tests[i].file_content) != 0) {
                printf("%s: FAIL (Cannot create file)\n", tests[i].description);
                continue;
            }
        }
        output[0] = '\0';

        int ret = read_file_content(tests[i].filename, output, sizeof(output));

        if (strcmp(output, tests[i].expected_output) == 0) {
            printf("%s: PASS\n", tests[i].description);
        } else {
            printf("%s: FAIL\nExpected:\n%s\nGot:\n%s\n", tests[i].description, tests[i].expected_output, output);
        }
    }

    // حذف الملفات بعد الاختبارات (اختياري)
    for (int i = 0; i < n; i++) {
        remove(tests[i].filename);
    }
}

int main() {
    run_tests();
    return 0;
}
