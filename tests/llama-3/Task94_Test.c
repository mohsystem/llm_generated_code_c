// test_runner.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 8192

int run_command_capture_output(const char *cmd, char *output, size_t size) {
    FILE *pipe = popen(cmd, "r");
    if (!pipe) return -1;
    size_t len = fread(output, 1, size - 1, pipe);
    output[len] = '\0';
    pclose(pipe);
    return 0;
}

int main() {
    char output[BUFFER_SIZE];

    printf("Test case 1 (no filename argument): FAIL not implemented (fixed filename)\n");
    printf("Test case 2 (file not found): FAIL not implemented (no error handling)\n");
    printf("Test case 3 (invalid line format): FAIL not implemented (no error handling)\n");

    // Test case 4: valid file test
    const char *test_content =
        "zoo=value1\n"
        "book=value2\n"
        "door=value3\n"
        "tree=value4\n";

    FILE *f = fopen("input.txt", "w");
    if (!f) {
        printf("Test case 4: FAIL could not create input.txt\n");
        return 1;
    }
    fputs(test_content, f);
    fclose(f);

    if (run_command_capture_output("./sort_records", output, sizeof(output)) != 0) {
        printf("Test case 4: FAIL could not run program\n");
        return 1;
    }

    const char *expected_output =
        "book=value2\n"
        "door=value3\n"
        "tree=value4\n"
        "zoo=value1\n";

    if (strcmp(output, expected_output) == 0) {
        printf("Test case 4: PASS\n");
    } else {
        printf("Test case 4: FAIL\nExpected:\n%s\nGot:\n%s\n", expected_output, output);
    }

    remove("input.txt");

    return 0;
}
