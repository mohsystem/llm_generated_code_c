#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 100
#define MAX_COLS 100
#define MAX_FIELD_LENGTH 100

int transpose_file(const char* filename, char* output, size_t max_output_size) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return -1;
    }
    char lines[MAX_LINES][MAX_COLS][MAX_FIELD_LENGTH];
    int lineCount = 0;
    int colCount = 0;
    char line[MAX_FIELD_LENGTH];

    while (fgets(line, MAX_FIELD_LENGTH, file) != NULL) {
        char *token = strtok(line, " \n");
        colCount = 0;  // reset colCount per line to fix your logic slightly
        while (token != NULL) {
            strcpy(lines[lineCount][colCount], token);
            token = strtok(NULL, " \n");
            colCount++;
        }
        lineCount++;
        if (lineCount >= MAX_LINES) break;
    }
    fclose(file);

    output[0] = '\0';

    for (int col = 0; col < colCount; col++) {
        for (int line = 0; line < lineCount; line++) {
            strncat(output, lines[line][col], max_output_size - strlen(output) - 1);
            if (line < lineCount -1)
                strncat(output, " ", max_output_size - strlen(output) - 1);
        }
        strncat(output, "\n", max_output_size - strlen(output) - 1);
    }

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
        const char* input_content;
        const char* expected_output;
        const char* description;
    } tests[] = {
        {
            "name age\nalice 21\nryan 30\n",
            "name alice ryan\nage 21 30\n",
            "Test Case 1"
        },
        {
            "a b c\n1 2 3\nx y z\n",
            "a 1 x\nb 2 y\nc 3 z\n",
            "Test Case 2"
        },
        {
            "foo bar\nbaz qux\n",
            "foo baz\nbar qux\n",
            "Test Case 3"
        },
    };

    char output[10000];
    int n = sizeof(tests)/sizeof(tests[0]);

    for (int i = 0; i < n; i++) {
        if (create_file("file.txt", tests[i].input_content) != 0) {
            printf("%s: FAIL (Cannot create file)\n", tests[i].description);
            continue;
        }
        output[0] = '\0';

        int ret = transpose_file("file.txt", output, sizeof(output));
        if (ret != 0) {
            printf("%s: FAIL (File open error)\n", tests[i].description);
            continue;
        }

        if (strcmp(output, tests[i].expected_output) == 0) {
            printf("%s: PASS\n", tests[i].description);
        } else {
            printf("%s: FAIL\nExpected:\n%sGot:\n%s\n", tests[i].description, tests[i].expected_output, output);
        }
    }

    remove("file.txt");
}

int main() {
    run_tests();
    return 0;
}
