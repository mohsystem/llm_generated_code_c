#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* key;
    char* value;
} Record;

int compare(const void* a, const void* b) {
    Record* ra = (Record*)a;
    Record* rb = (Record*)b;
    return strcmp(ra->key, rb->key);
}

// Your original file reading code unchanged (for real files)
int read_and_sort_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return 1;
    }
    Record records[100];
    int count = 0;
    char line[1024];
    while (fgets(line, 1024, file)) {
        char* pos = strchr(line, '=');
        if (pos != NULL) {
            *pos = '\0';
            records[count].key = strdup(line);
            records[count].value = strdup(pos + 1);
            count++;
        }
    }
    fclose(file);

    qsort(records, count, sizeof(Record), compare);

    for (int i = 0; i < count; i++) {
        printf("%s=%s", records[i].key, records[i].value);
        if(records[i].value[strlen(records[i].value)-1] != '\n') printf("\n");
    }
    for (int i = 0; i < count; i++) {
        free(records[i].key);
        free(records[i].value);
    }
    return 0;
}

// Helper: simulate parsing from array of lines instead of file
void read_and_sort_from_lines(const char** lines, int line_count) {
    Record records[100];
    int count = 0;

    for (int i = 0; i < line_count; i++) {
        char* line = strdup(lines[i]);
        char* pos = strchr(line, '=');
        if (pos != NULL) {
            *pos = '\0';
            records[count].key = strdup(line);
            records[count].value = strdup(pos + 1);
            count++;
        }
        free(line);
    }

    qsort(records, count, sizeof(Record), compare);

    for (int i = 0; i < count; i++) {
        printf("%s=%s", records[i].key, records[i].value);
        if(records[i].value[strlen(records[i].value)-1] != '\n') printf("\n");
    }

    for (int i = 0; i < count; i++) {
        free(records[i].key);
        free(records[i].value);
    }
}

// Capture stdout (POSIX)
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
        "zoo=value1\n",
        "book=value2\n",
        "door=value3\n",
        "tree=value4\n"
    };

    const char* expected_output =
        "book=value2\n"
        "door=value3\n"
        "tree=value4\n"
        "zoo=value1\n";

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
        // No file specified: run tests only
        run_tests();
        return 0;
    }
    return read_and_sort_file(argv[1]);
}
