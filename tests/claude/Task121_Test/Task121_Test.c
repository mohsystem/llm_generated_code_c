#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILENAME 512
#define BUFFER_SIZE 1024

// دالة لفحص وجود الملف وحجم الملف وطباعة النتيجة
int uploadFileTest(const char* filename) {
    FILE *file = fopen(filename, "rb");
    if (file) {
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        char buffer[BUFFER_SIZE];
        size_t bytesRead;
        long totalBytesRead = 0;

        while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
            totalBytesRead += bytesRead;
        }

        fclose(file);

        printf("File '%s' uploaded successfully.\n", filename);
        printf("File size: %ld bytes\n", fileSize);

        // التحقق من تطابق حجم الملف مع مجموع القراءات
        if (fileSize == totalBytesRead) {
            printf("Test result for '%s': PASS\n\n", filename);
            return 1;
        } else {
            printf("Test result for '%s': FAIL (size mismatch)\n\n", filename);
            return 0;
        }
    } else {
        printf("File '%s' not found.\n", filename);
        printf("Test result for '%s': FAIL (file not found)\n\n", filename);
        return 0;
    }
}

int main() {
    // الملفات حسب نفس الترتيب في بايثون مع المسار "testFiles/"
    const char* filenames[] = {
        "/home/kali/CLionProjects/llm_generated_code_c/tests/claude/Task121_Test/test_file_1.txt",
        "/home/kali/CLionProjects/llm_generated_code_c/tests/claude/Task121_Test/large_file.txt",
        "/home/kali/CLionProjects/llm_generated_code_c/tests/claude/Task121_Test/file_with_!@#$%^&*()_+[]{};,.txt",
        "/home/kali/CLionProjects/llm_generated_code_c/tests/claude/Task121_Test/file_no_extension",
        "/home/kali/CLionProjects/llm_generated_code_c/tests/claude/Task121_Test/empty_file.txt",
        "/home/kali/CLionProjects/llm_generated_code_c/tests/claude/Task121_Test/non_existent_file.txt",
        "/home/kali/CLionProjects/llm_generated_code_c/tests/claude/Task121_Test/file with spaces.txt",
        "/home/kali/CLionProjects/llm_generated_code_c/tests/claude/Task121_Test/a_very_long_filename_that_exceeds_normal_length_limits_for_filenames.txt",
        "/home/kali/CLionProjects/llm_generated_code_c/tests/claude/Task121_Test/image_file.png",
        "/home/kali/CLionProjects/llm_generated_code_c/tests/claude/Task121_Test/file_with_newlines.txt",
        "/home/kali/CLionProjects/llm_generated_code_c/tests/claude/Task121_Test/file_not_found.txt"
    };


    int totalTests = sizeof(filenames) / sizeof(filenames[0]);
    int passed = 0;

    for (int i = 0; i < totalTests; i++) {
        printf("Running test case %d: %s\n", i + 1, filenames[i]);
        passed += uploadFileTest(filenames[i]);
    }

    printf("Summary: Passed %d out of %d tests.\n", passed, totalTests);

    return 0;
}
