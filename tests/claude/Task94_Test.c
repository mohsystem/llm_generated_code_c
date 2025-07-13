#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1000
#define MAX_RECORDS 1000

typedef struct {
    char *key;
    char *value;
} KeyValue;

int compare(const void *a, const void *b) {
    return strcmp(((KeyValue *)a)->key, ((KeyValue *)b)->key);
}

void read_and_sort_records(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file '%s'\n", filename);
        return;
    }

    KeyValue records[MAX_RECORDS];
    int record_count = 0;
    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), file) && record_count < MAX_RECORDS) {
        char *colon = strchr(line, ':');
        if (colon != NULL) {
            *colon = '\0';
            records[record_count].key = strdup(line);
            records[record_count].value = strdup(colon + 1);

            // Trim whitespace key
            char *end = records[record_count].key + strlen(records[record_count].key) - 1;
            while (end > records[record_count].key && (*end == ' ' || *end == '\t' || *end == '\n')) *end-- = '\0';

            // Trim whitespace value
            char *start = records[record_count].value;
            while (*start == ' ' || *start == '\t') start++;
            end = start + strlen(start) - 1;
            while (end > start && (*end == ' ' || *end == '\t' || *end == '\n')) *end-- = '\0';

            free(records[record_count].value);
            records[record_count].value = strdup(start);

            record_count++;
        } else {
            printf("Warning: Skipping invalid line: %s", line);
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

// دالة مساعدة لقراءة السجلات من مصفوفة نصوص بدل ملف (لتشغيل التيست كيسز)
void read_and_sort_records_from_lines(const char **lines, int line_count) {
    KeyValue records[MAX_RECORDS];
    int record_count = 0;

    for (int i = 0; i < line_count; i++) {
        char *line = strdup(lines[i]);
        char *colon = strchr(line, ':');
        if (colon != NULL) {
            *colon = '\0';
            records[record_count].key = strdup(line);
            records[record_count].value = strdup(colon + 1);

            // Trim whitespace key
            char *end = records[record_count].key + strlen(records[record_count].key) - 1;
            while (end > records[record_count].key && (*end == ' ' || *end == '\t' || *end == '\n')) *end-- = '\0';

            // Trim whitespace value
            char *start = records[record_count].value;
            while (*start == ' ' || *start == '\t') start++;
            end = start + strlen(start) - 1;
            while (end > start && (*end == ' ' || *end == '\t' || *end == '\n')) *end-- = '\0';

            free(records[record_count].value);
            records[record_count].value = strdup(start);

            record_count++;
        } else {
            printf("Warning: Skipping invalid line: %s\n", line);
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

// دوال لمراقبة وطباعة الخرج من أجل المقارنة (POSIX)
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

// دالة تشغيل التيست كيسز
void run_tests() {
    printf("Running tests...\n");

    const char *test_input[] = {
        "zoo : value1\n",
        "book : value2\n",
        "door : value3\n",
        "tree : value4\n"
    };

    const char *expected_output =
        "book: value2\n"
        "door: value3\n"
        "tree: value4\n"
        "zoo: value1\n";

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

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        run_tests();
        return 1;
    }

    read_and_sort_records(argv[1]);
    return 0;
}
