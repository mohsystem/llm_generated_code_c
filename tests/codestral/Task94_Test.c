#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* key;
    char* value;
} KeyValue;

int compare(const void* a, const void* b) {
    return strcmp(((KeyValue*)a)->key, ((KeyValue*)b)->key);
}

// Your original file-reading code - keep untouched for real use
void read_and_sort_records(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Unable to open file '%s'\n", filename);
        return;
    }

    KeyValue records[1000];
    int record_count = 0;
    char line[1000];

    while (fgets(line, sizeof(line), file) && record_count < 1000) {
        char* colon = strchr(line, ':');
        if (colon) {
            *colon = '\0';
            records[record_count].key = strdup(line);
            records[record_count].value = strdup(colon + 1);

            // Trim key whitespace
            char* end = records[record_count].key + strlen(records[record_count].key) - 1;
            while (end > records[record_count].key && (*end == ' ' || *end == '\t' || *end == '\n')) *end-- = '\0';

            // Trim value whitespace
            char* start = records[record_count].value;
            while (*start == ' ' || *start == '\t') start++;
            end = start + strlen(start) - 1;
            while (end > start && (*end == ' ' || *end == '\t' || *end == '\n')) *end-- = '\0';

            free(records[record_count].value);
            records[record_count].value = strdup(start);

            record_count++;
        }
    }
    fclose(file);

    qsort(records, record_count, sizeof(KeyValue), compare);

    for (int i = 0; i < record_count; i++) {
        printf("%s: %s\n", records[i].key, records[i].value);
        free(records[i].key);
        free(records[i].value);
    }
}

// Helper: simulate input lines and run the same sorting and printing logic on them (no files)
void read_and_sort_records_from_lines(const char** lines, int count) {
    KeyValue records[1000];
    int record_count = 0;

    for (int i = 0; i < count; i++) {
        char* line = strdup(lines[i]);
        char* colon = strchr(line, ':');
        if (colon) {
            *colon = '\0';
            records[record_count].key = strdup(line);
            records[record_count].value = strdup(colon + 1);

            // Trim key whitespace
            char* end = records[record_count].key + strlen(records[record_count].key) - 1;
            while (end > records[record_count].key && (*end == ' ' || *end == '\t' || *end == '\n')) *end-- = '\0';

            // Trim value whitespace
            char* start = records[record_count].value;
            while (*start == ' ' || *start == '\t') start++;
            end = start + strlen(start) - 1;
            while (end > start && (*end == ' ' || *end == '\t' || *end == '\n')) *end-- = '\0';

            free(records[record_count].value);
            records[record_count].value = strdup(start);

            record_count++;
        }
        free(line);
    }

    qsort(records, record_count, sizeof(KeyValue), compare);

    for (int i = 0; i < record_count; i++) {
        printf("%s: %s\n", records[i].key, records[i].value);
        free(records[i].key);
        free(records[i].value);
    }
}

// Capture stdout for testing (POSIX)
#include <unistd.h>
#define CAPTURE_SIZE 8192

int capture_stdout_start(int* saved_fd) {
    fflush(stdout);
    *saved_fd = dup(STDOUT_FILENO);
    int pipefd[2];
    if (pipe(pipefd) == -1) return -1;
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[1]);
    return pipefd[0];
}

void capture_stdout_end(int saved_fd, int pipefd, char* buffer, int size) {
    fflush(stdout);
    dup2(saved_fd, STDOUT_FILENO);
    close(saved_fd);
    int n = read(pipefd, buffer, size - 1);
    if (n >= 0) buffer[n] = '\0'; else buffer[0] = '\0';
    close(pipefd);
}

// Run the test case exactly as in the python prompt and print pass/fail
void run_tests() {
    printf("Running test cases...\n");

    const char* test_input[] = {
        "zoo : value1\n",
        "book : value2\n",
        "door : value3\n",
        "tree : value4\n"
    };
    const char* expected_output =
        "book: value2\n"
        "door: value3\n"
        "tree: value4\n"
        "zoo: value1\n";

    int saved_fd, pipefd;
    char buffer[CAPTURE_SIZE];

    pipefd = capture_stdout_start(&saved_fd);
    read_and_sort_records_from_lines(test_input, 4);
    capture_stdout_end(saved_fd, pipefd, buffer, CAPTURE_SIZE);

    if (strcmp(buffer, expected_output) == 0) {
        printf("Test case: PASS\n");
    } else {
        printf("Test case: FAIL\nExpected:\n%s\nGot:\n%s\n", expected_output, buffer);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        run_tests();
        return 0;
    }
    read_and_sort_records(argv[1]);
    return 0;
}
