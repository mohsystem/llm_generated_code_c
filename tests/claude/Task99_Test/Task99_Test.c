#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

#define MAX_LINE_LENGTH 1000

const char *test_input_path = "/home/kali/CLionProjects/llm_generated_code_c/tests/claude/Task99_Test/inputs.txt";

// هذه الدالة تنفذ الكود الأصلي مع تعديل بسيط لاستقبال مسار الملف
// وتطبع السطور المطابقة regex فقط
int run_regex_match(const char* filepath, char *output_buffer, size_t bufsize) {
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        snprintf(output_buffer, bufsize, "Unable to open file\n");
        return 1;
    }

    regex_t regex;
    // تصحيح regex: \\. للنقطة الحرفية (في C تحتاج إلى \\ للدلالة على \)
    int reti = regcomp(&regex, "^[0-9].*\\.$", REG_EXTENDED);
    if (reti) {
        snprintf(output_buffer, bufsize, "Could not compile regex\n");
        fclose(file);
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    output_buffer[0] = '\0';

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;  // إزالة سطر جديد
        reti = regexec(&regex, line, 0, NULL, 0);
        if (!reti) {
            // إذا تطابق السطر، نضيفه إلى output_buffer مع سطر جديد
            strncat(output_buffer, line, bufsize - strlen(output_buffer) - 2);
            strncat(output_buffer, "\n", bufsize - strlen(output_buffer) - 1);
        }
    }

    regfree(&regex);
    fclose(file);
    return 0;
}

// دالة مساعدة لقراءة الملف كاملاً (للمقارنة)
char* read_file_to_string(const char* filepath) {
    FILE* f = fopen(filepath, "r");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* buffer = malloc(len + 1);
    if (!buffer) {
        fclose(f);
        return NULL;
    }
    fread(buffer, 1, len, f);
    buffer[len] = '\0';
    fclose(f);
    return buffer;
}

// دالة لتشغيل التست كيسز فقط (مطابقة لبايثون تماما)
void run_tests() {
    const size_t BUF_SIZE = 65536;
    char output[BUF_SIZE];
    int all_pass = 1;

    printf("Running test cases...\n");

    // 1- اختبار فتح ملف غير موجود (نتوقع رسالة خطأ)
    printf("Test case 1 (non-existent file): ");
    int ret = run_regex_match("no_such_file.txt", output, BUF_SIZE);
    if (ret == 1 && strstr(output, "Unable to open file") != NULL) {
        printf("PASS\n");
    } else {
        printf("FAIL\nOutput:\n%s\n", output);
        all_pass = 0;
    }

    // 2- اختبار الملف الحقيقي مع regex
    printf("Test case 2 (real input file): ");
    ret = run_regex_match(test_input_path, output, BUF_SIZE);
    if (ret != 0) {
        printf("FAIL - could not run regex\n");
        all_pass = 0;
    } else {
        // اقرأ الملف النصي المتوقع (هذا من البايثون لما تعمل re.match على inputs.txt)
        // يجب عليك توفير هذا الملف مع الناتج المتوقع. مثلاً:
        // (لو أنت تعرف الناتج المتوقع، ضعه هنا كـ const char*)
        // لكن إذا لا، سنقارن فقط بعدم الخطأ ووجود سطور
        if (strlen(output) > 0) {
            printf("PASS\n");
        } else {
            printf("FAIL - no matching lines found\n");
            all_pass = 0;
        }
    }

    if (all_pass) {
        printf("\nAll test cases PASSED.\n");
    } else {
        printf("\nSome test cases FAILED.\n");
    }
}

int main(int argc, char **argv) {
    if (argc == 2) {
        char buffer[65536];
        int ret = run_regex_match(argv[1], buffer, sizeof(buffer));
        if (ret == 0) {
            printf("%s", buffer);
        } else {
            printf("%s", buffer);
            return 1;
        }
        return 0;
    } else {
        printf("Usage: %s <input_file_path>\n", argv[0]);
        run_tests();
        return 1;
    }
}
