#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char key[50];
    char value[50];
} Record;

int compare(const void *a, const void *b) {
    return strcmp(((Record *)a)->key, ((Record *)b)->key);
}

// Your original function unchanged, but we won't use it directly in tests since it reads file
void read_and_sort(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Unable to open file.\n");
        return;
    }

    Record records[100];
    int count = 0;
    while (fscanf(file, "%s %s", records[count].key, records[count].value) != EOF) {
        count++;
    }
    fclose(file);

    qsort(records, count, sizeof(Record), compare);

    for (int i = 0; i < count; i++) {
        printf("%s %s\n", records[i].key, records[i].value);
    }
}

// New helper to simulate reading from lines (array of strings)
void read_and_sort_from_lines(const char **lines, int line_count) {
    Record records[100];
    int count = 0;

    for (int i = 0; i < line_count; i++) {
        // Parse key and value like fscanf expects: two strings separated by space
        // We'll use sscanf to parse each line
        if (sscanf(lines[i], "%49s %49s", records[count].key, records[count].value) == 2) {
            count++;
        }
    }

    qsort(records, count, sizeof(Record), compare);

    for (int i = 0; i < count; i++) {
        printf("%s %s\n", records[i].key, records[i].value);
    }
}

// Capture stdout helper (POSIX)
#include <unistd.h>
#define CAPTURE_BUF_SIZE 8192

int capture_start(int *saved_stdout) {
    fflush(stdout);
    *saved_stdout = dup(STDOUT_FILENO);
    int pipefd[2];
    if (pipe(pipefd) == -1) return -1;
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[1]);
    return pipefd[0];
}

void capture_end(int saved_stdout, int pipefd, char *buffer, int bufsize) {
    fflush(stdout);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);
    int n = read(pipefd, buffer, bufsize - 1);
    if (n >= 0) buffer[n] = '\0'; else buffer[0] = '\0';
    close(pipefd);
}

void run_tests() {
    printf("Running tests...\n");

    const char *test_input[] = {
        "zoo value1",
        "book value2",
        "door value3",
        "tree value4"
    };

    const char *expected_output =
        "book value2\n"
        "door value3\n"
        "tree value4\n"
        "zoo value1\n";

    int saved_stdout, pipefd;
    char buffer[CAPTURE_BUF_SIZE];

    pipefd = capture_start(&saved_stdout);
    read_and_sort_from_lines(test_input, 4);
    capture_end(saved_stdout, pipefd, buffer, CAPTURE_BUF_SIZE);

    if (strcmp(buffer, expected_output) == 0) {
        printf("Test case 1: PASS\n");
    } else {
        printf("Test case 1: FAIL\nExpected:\n%s\nGot:\n%s\n", expected_output, buffer);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        // No filename provided: run tests only
        run_tests();
        return 0;
    }
    read_and_sort(argv[1]);
    return 0;
}
