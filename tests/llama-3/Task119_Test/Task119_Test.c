#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

// دالة parseCSV كما هي
void parseCSV(const char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Could not open file\n");
        return;
    }
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, ",");
        while (token != NULL) {
            printf("%s ", token);
            token = strtok(NULL, ",");
        }
        printf("\n");
    }
    fclose(file);
}

// دالة مقارنة ملفات لمقارنة الناتج مع المرجع
int compare_files(const char* file1, const char* file2) {
    FILE* f1 = fopen(file1, "r");
    FILE* f2 = fopen(file2, "r");
    if (!f1 || !f2) {
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        return 0;
    }
    int ch1, ch2;
    do {
        ch1 = fgetc(f1);
        ch2 = fgetc(f2);
        if (ch1 != ch2) {
            fclose(f1);
            fclose(f2);
            return 0;
        }
    } while (ch1 != EOF && ch2 != EOF);
    fclose(f1);
    fclose(f2);
    return (ch1 == EOF && ch2 == EOF);
}

int main() {
    const char* testFiles[10] = {
        "/home/kali/CLionProjects/llm_generated_code_c/tests/llama-3/Task119_Test/tc1.csv",
        "/home/kali/CLionProjects/llm_generated_code_c/tests/llama-3/Task119_Test/tc2.csv",
        "/home/kali/CLionProjects/llm_generated_code_c/tests/llama-3/Task119_Test/tc3.csv",
        "/home/kali/CLionProjects/llm_generated_code_c/tests/llama-3/Task119_Test/tc4.csv",
        "/home/kali/CLionProjects/llm_generated_code_c/tests/llama-3/Task119_Test/tc5.csv",
        "/home/kali/CLionProjects/llm_generated_code_c/tests/llama-3/Task119_Test/tc6.csv",
        "/home/kali/CLionProjects/llm_generated_code_c/tests/llama-3/Task119_Test/tc7.csv",
        "/home/kali/CLionProjects/llm_generated_code_c/tests/llama-3/Task119_Test/tc8.csv",
        "/home/kali/CLionProjects/llm_generated_code_c/tests/llama-3/Task119_Test/tc9.csv",
        "/home/kali/CLionProjects/llm_generated_code_c/tests/llama-3/Task119_Test/tc10.csv"
    };

    int passCount = 0;

    for (int i = 0; i < 10; i++) {
        char outputFile[200];
        snprintf(outputFile, sizeof(outputFile), "/home/kali/CLionProjects/llm_generated_code_c/tests/llama-3/Task119_Test/output_tc%d.txt", i + 1);

        FILE* old_stdout = stdout;
        FILE* f = freopen(outputFile, "w", stdout);

        printf("Processing file: %s\n", testFiles[i]);
        parseCSV(testFiles[i]);

        fflush(stdout);
        freopen("/dev/tty", "w", stdout);

        printf("Test case %d (%s): ", i + 1, testFiles[i]);

        char refFile[200];
        snprintf(refFile, sizeof(refFile), "/home/kali/CLionProjects/llm_generated_code_c/tests/llama-3/Task119_Test/output_tc%d_ref.txt", i + 1);

        if (compare_files(outputFile, refFile)) {
            printf("PASS\n");
            passCount++;
        } else {
            printf("FAIL\n");
        }
    }

    printf("Summary: %d/10 tests passed.\n", passCount);

    return (passCount == 10) ? 0 : 1;
}
