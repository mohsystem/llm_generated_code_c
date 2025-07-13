#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

// Structure to store key-value pairs
typedef struct {
    char key[MAX_LINE_LENGTH];
    char value[MAX_LINE_LENGTH];
} KeyValuePair;

// Comparison function for qsort() to sort by key
int compare_key_value_pairs(const void* a, const void* b) {
    KeyValuePair* pair_a = (KeyValuePair*)a;
    KeyValuePair* pair_b = (KeyValuePair*)b;
    return strcmp(pair_a->key, pair_b->key);
}

// Original function modified to accept lines instead of file (helper)
void read_and_sort_records_from_lines(const char** lines, int line_count) {
    KeyValuePair* records = NULL;
    int num_records = 0;

    for (int i = 0; i < line_count; i++) {
        const char* line = lines[i];
        char* delimiter = strchr((char*)line, '=');
        if (delimiter != NULL) {
            records = (KeyValuePair*)realloc(records, (num_records + 1) * sizeof(KeyValuePair));
            if (!records) {
                perror("Memory allocation failed");
                return;
            }
            // Copy key
            int key_len = delimiter - line;
            strncpy(records[num_records].key, line, key_len);
            records[num_records].key[key_len] = '\0';

            // Copy value
            strcpy(records[num_records].value, delimiter + 1);

            // Trim trailing spaces in key
            char* end = records[num_records].key + strlen(records[num_records].key) - 1;
            while (end > records[num_records].key && (*end == ' ' || *end == '\t' || *end == '\n')) {
                *end = '\0';
                end--;
            }

            // Trim leading spaces in value
            char* start = records[num_records].value;
            while (*start == ' ' || *start == '\t') start++;
            if (start != records[num_records].value) {
                memmove(records[num_records].value, start, strlen(start) + 1);
            }

            num_records++;
        }
    }

    qsort(records, num_records, sizeof(KeyValuePair), compare_key_value_pairs);

    for (int i = 0; i < num_records; i++) {
        printf("%s=%s\n", records[i].key, records[i].value);
    }

    free(records);
}

// Capture output helpers (POSIX)
#include <unistd.h>
#define CAPTURE_BUF_SIZE 8192

int capture_start(int* saved_stdout) {
    fflush(stdout);
    *saved_stdout = dup(STDOUT_FILENO);
    int pipefd[2];
    if (pipe(pipefd) == -1) return -1;
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[1]);
    return pipefd[0];
}

void capture_end(int saved_stdout, int pipefd, char* buffer, int bufsize) {
    fflush(stdout);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);
    int n = read(pipefd, buffer, bufsize - 1);
    if (n >= 0) buffer[n] = '\0'; else buffer[0] = '\0';
    close(pipefd);
}

void run_tests() {
    printf("Running tests...\n");

    const char* test_input[] = {
        "zoo = value1",
        "book = value2",
        "door = value3",
        "tree = value4"
    };
    const char* expected_output =
        "book=value2\n"
        "door=value3\n"
        "tree=value4\n"
        "zoo=value1\n";

    int saved_stdout, pipefd;
    char buffer[CAPTURE_BUF_SIZE];

    pipefd = capture_start(&saved_stdout);
    read_and_sort_records_from_lines(test_input, 4);
    capture_end(saved_stdout, pipefd, buffer, CAPTURE_BUF_SIZE);

    if (strcmp(buffer, expected_output) == 0) {
        printf("Test case 1: PASS\n");
    } else {
        printf("Test case 1: FAIL\nExpected:\n%s\nGot:\n%s\n", expected_output, buffer);
    }
}

int main() {
    run_tests();
    return 0;
}
