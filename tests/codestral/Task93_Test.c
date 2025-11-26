#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

int compare(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

// Your original code reading from a file (unchanged)
char** readAndSortFile(const char* filePath, int* numRecords) {
    FILE* file = fopen(filePath, "r");
    if (file == NULL) {
        perror("Unable to open file");
        return NULL;
    }

    char** records = NULL;
    char line[MAX_LINE_LENGTH];
    int capacity = 0;

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        if (*numRecords >= capacity) {
            capacity = (capacity == 0) ? 1 : capacity * 2;
            records = (char**)realloc(records, capacity * sizeof(char*));
        }
        records[*numRecords] = strdup(line);
        (*numRecords)++;
    }

    fclose(file);

    qsort(records, *numRecords, sizeof(char*), compare);

    return records;
}

// New helper: simulate reading from array of lines instead of file
char** readAndSortFromLines(const char** lines, int lineCount, int* numRecords) {
    char** records = NULL;
    int capacity = 0;
    *numRecords = 0;

    for (int i = 0; i < lineCount; i++) {
        if (*numRecords >= capacity) {
            capacity = (capacity == 0) ? 1 : capacity * 2;
            records = (char**)realloc(records, capacity * sizeof(char*));
        }
        records[*numRecords] = strdup(lines[i]);
        (*numRecords)++;
    }

    qsort(records, *numRecords, sizeof(char*), compare);

    return records;
}

// Capture stdout helper (POSIX)
#include <unistd.h>
#include <fcntl.h>

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

// Test runner for the in-memory case
void run_tests() {
    printf("Running tests...\n");

    const char* test_input[] = {
        "zoo : value1\n",
        "book : value2\n",
        "door : value3\n",
        "tree : value4\n"
    };
    const char* expected_output =
        "book : value2\n"
        "door : value3\n"
        "tree : value4\n"
        "zoo : value1\n";

    int numRecords = 0;

    int saved_stdout, pipefd;
    char buffer[CAPTURE_BUF_SIZE];

    pipefd = capture_start(&saved_stdout);

    char** sorted = readAndSortFromLines(test_input, 4, &numRecords);
    for (int i = 0; i < numRecords; i++) {
        printf("%s", sorted[i]);
        free(sorted[i]);
    }
    free(sorted);

    capture_end(saved_stdout, pipefd, buffer, CAPTURE_BUF_SIZE);

    if (strcmp(buffer, expected_output) == 0) {
        printf("Test case 1: PASS\n");
    } else {
        printf("Test case 1: FAIL\n");
        printf("Expected output:\n%s\n", expected_output);
        printf("Actual output:\n%s\n", buffer);
    }
}

int main() {
    run_tests();
    return 0;
}
