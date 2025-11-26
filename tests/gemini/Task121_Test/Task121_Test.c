#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH 512

int upload_file(const char* filePath) {
    FILE *file;
    long fileSize;
    char *buffer;
    size_t bytesRead;

    file = fopen(filePath, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filePath);
        return 0; // FAIL
    }

    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    rewind(file);

    buffer = (char *)malloc(fileSize + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        fclose(file);
        return 0; // FAIL
    }

    bytesRead = fread(buffer, 1, fileSize, file);
    if (bytesRead != fileSize) {
        fprintf(stderr, "Error reading file: %s\n", filePath);
        fclose(file);
        free(buffer);
        return 0; // FAIL
    }
    buffer[fileSize] = '\0';

    fclose(file);
    free(buffer);

    return 1; // PASS
}

int main() {
    const char* basePath = "/home/kali/CLionProjects/llm_generated_code_c/tests/gemini/Task121_Test/";

    const char* filenames[] = {
        "test_file_1.txt",
        "large_file.txt",
        "file_with_!@#$%^&*()_+[]{};,.txt",
        "file_no_extension.txt",
        "empty_file.txt",
        "non_existent_file.txt",
        "file with spaces.txt",
        "a_very_long_filename_that_exceeds_normal_length_limits_for_filenames.txt",
        "image_file.png",
        "file_with_newlines.txt",
        "file_not_found.txt"
    };

    int totalTests = sizeof(filenames) / sizeof(filenames[0]);
    int passed = 0;
    char fullPath[MAX_PATH];

    for (int i = 0; i < totalTests; i++) {
        snprintf(fullPath, sizeof(fullPath), "%s%s", basePath, filenames[i]);
        printf("Testing file: %s\n", fullPath);
        int result = upload_file(fullPath);
        if (result) {
            printf("Test result for '%s': PASS\n\n", fullPath);
            passed++;
        } else {
            printf("Test result for '%s': FAIL\n\n", fullPath);
        }
    }

    printf("Summary: Passed %d out of %d tests.\n", passed, totalTests);
    return 0;
}
