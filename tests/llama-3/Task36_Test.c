#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OUTPUT_SIZE 10000

int read_file_and_capture(const char* filename, char* output, size_t max_output_size) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        snprintf(output, max_output_size, "Unable to open file");
        return 1;
    }
    output[0] = '\0';
    char line[1000];
    while (fgets(line, sizeof(line), file)) {
        if (strlen(output) + strlen(line) >= max_output_size) break;
        strcat(output, line);
    }
    fclose(file);
    return 0;
}

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
        const char* file_content;
        const char* expected_output;
        const char* description;
    } tests[] = {
        {"test_file.txt", "Hello, World!\n", "Hello, World!\n", "test_read_file_success"},
        {"missing_file.txt", NULL, "Unable to open file", "test_file_not_found"},
        {"empty_file.txt", "", "", "test_read_empty_file"},
        {"multiline_file.txt", "Line 1\nLine 2\n", "Line 1\nLine 2\n", "test_multiline_content"},
        {"trailing_spaces.txt", "Trailing spaces    \n", "Trailing spaces    \n", "test_trailing_spaces"},
        {"special_chars.txt", "!@#$%^&*()\n", "!@#$%^&*()\n", "test_special_characters"},
    };

    char output[MAX_OUTPUT_SIZE];
    int n = sizeof(tests)/sizeof(tests[0]);

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
        int ret = read_file_and_capture(tests[i].filename, output, sizeof(output));
        if (strcmp(output, tests[i].expected_output) == 0) {
            printf("%s: PASS\n", tests[i].description);
        } else {
            printf("%s: FAIL\nExpected:\n%s\nGot:\n%s\n", tests[i].description, tests[i].expected_output, output);
        }
    }

    for (int i=0; i<n; i++) {
        remove(tests[i].filename);
    }
}

int main() {
    run_tests();
    return 0;
}
