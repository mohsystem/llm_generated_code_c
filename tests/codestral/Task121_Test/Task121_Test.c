#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define TEST_FILES_DIR "/home/kali/CLionProjects/llm_generated_code_c/tests/codestral/Task121_Test/"

void upload_file(const char* filename) {
    char filepath[1024];
    snprintf(filepath, sizeof(filepath), "%s%s", TEST_FILES_DIR, filename);

    FILE* fp = fopen(filepath, "rb");
    if (fp == NULL) {
        printf("File '%s' not found.\n", filename);
        printf("Result: FAIL\n");
        return;
    }

    // احسب حجم الملف
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // قراءة الملف (اختياري، فقط للتحقق من القدرة على القراءة)
    char* buffer = malloc(size);
    if (!buffer) {
        printf("Memory allocation failed for file '%s'.\n", filename);
        fclose(fp);
        printf("Result: FAIL\n");
        return;
    }

    size_t read_bytes = fread(buffer, 1, size, fp);
    fclose(fp);

    if (read_bytes != size) {
        printf("Error reading file '%s'.\n", filename);
        free(buffer);
        printf("Result: FAIL\n");
        return;
    }

    printf("File '%s' uploaded successfully.\n", filename);
    printf("File size: %ld bytes\n", size);
    printf("Result: PASS\n");

    free(buffer);
}
void run_tests() {
    const char* filenames[] = {
        "test_file_1.txt",
        "large_file.txt",
        "file_with_!@#$%^&*()_+[]{};,.txt",
        "file_no_extension",
        "empty_file.txt",
        "non_existent_file.txt",
        "file with spaces.txt",
        "a_very_long_filename_that_exceeds_normal_length_limits_for_filenames.txt",
        "image_file.png",
        "file_with_newlines.txt",
        "file_not_found.txt"
    };

    int n = sizeof(filenames) / sizeof(filenames[0]);
    for (int i = 0; i < n; i++) {
        upload_file(filenames[i]);
    }
}

int main() {
    run_tests();
    return 0;
}
