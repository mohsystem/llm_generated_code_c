#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 100
#define MAX_COLS 100
#define MAX_LINE_LENGTH 1000

int transpose_file_and_capture_output(const char* filename, char* output, size_t max_output_size) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return -1; // خطأ فتح الملف
    }

    // تخصيص ديناميكي لمصفوفة ثلاثية الأبعاد
    char ***content = malloc(MAX_ROWS * sizeof(char**));
    for (int i = 0; i < MAX_ROWS; i++) {
        content[i] = malloc(MAX_COLS * sizeof(char*));
        for (int j = 0; j < MAX_COLS; j++) {
            content[i][j] = malloc(MAX_LINE_LENGTH * sizeof(char));
        }
    }

    int row = 0, col = 0, max_col = 0;
    char line[MAX_LINE_LENGTH];
    output[0] = '\0';

    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, " \n");
        col = 0;
        while (token != NULL) {
            strcpy(content[row][col], token);
            token = strtok(NULL, " \n");
            col++;
        }
        if (col > max_col) max_col = col;
        row++;
    }
    fclose(file);

    for (int j = 0; j < max_col; j++) {
        for (int i = 0; i < row; i++) {
            strncat(output, content[i][j], max_output_size - strlen(output) - 1);
            if (i < row - 1)
                strncat(output, " ", max_output_size - strlen(output) - 1);
        }
        strncat(output, "\n", max_output_size - strlen(output) - 1);
    }

    // تحرير الذاكرة المخصصة
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            free(content[i][j]);
        }
        free(content[i]);
    }
    free(content);

    return 0;
}

int create_file_with_content(const char* filename, const char* content) {
    FILE *f = fopen(filename, "w");
    if (!f) return -1;
    fputs(content, f);
    fclose(f);
    return 0;
}

void run_tests() {
    struct {
        const char* file_content;
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
        if (create_file_with_content("file.txt", tests[i].file_content) != 0) {
            printf("%s: FAIL (Cannot create file)\n", tests[i].description);
            continue;
        }
        output[0] = '\0';

        int ret = transpose_file_and_capture_output("file.txt", output, sizeof(output));
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
