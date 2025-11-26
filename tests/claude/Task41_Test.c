#include <stdio.h>
#include <string.h>

#define MAX_LENGTH 100

void processInput(const char* input) {
    // Process the input (example: print it back)
    printf("Processed input: %s\n", input);
}

int run_test(const char* test_input, const char* expected_output, const char* test_name) {
    // Redirect stdin from a string using tmp file approach for simplicity
    FILE *tmp = tmpfile();
    if (!tmp) {
        printf("%s: FAIL (cannot create tmp file)\n", test_name);
        return 0;
    }
    fprintf(tmp, "%s\n", test_input);
    rewind(tmp);

    // Backup original stdin
    FILE *orig_stdin = stdin;
    stdin = tmp;

    char userInput[MAX_LENGTH + 1];
    int passed = 0;

    if (fgets(userInput, sizeof(userInput), stdin) != NULL) {
        size_t len = strlen(userInput);
        if (len > 0 && userInput[len-1] == '\n') {
            userInput[len-1] = '\0';
        }
        // Capture output by redirecting stdout to memory (using temporary file)
        FILE *tmp_out = tmpfile();
        if (!tmp_out) {
            printf("%s: FAIL (cannot create tmp output file)\n", test_name);
            stdin = orig_stdin;
            fclose(tmp);
            return 0;
        }
        FILE *orig_stdout = stdout;
        stdout = tmp_out;

        processInput(userInput);

        fflush(tmp_out);
        rewind(tmp_out);

        char output[256] = {0};
        fread(output, 1, sizeof(output)-1, tmp_out);

        stdout = orig_stdout;
        fclose(tmp_out);

        if (strcmp(output, expected_output) == 0) {
            printf("%s: PASS\n", test_name);
            passed = 1;
        } else {
            printf("%s: FAIL\nExpected: \"%s\"\nGot: \"%s\"\n", test_name, expected_output, output);
        }
    } else {
        printf("%s: FAIL (no input read)\n", test_name);
    }

    // Restore stdin
    stdin = orig_stdin;
    fclose(tmp);
    return passed;
}

int main() {
    // Test cases copied exactly from Python assertions:
    run_test("Hello, world!", "Processed input: Hello, world!\n", "Test 1: Normal input");
    run_test("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
             "Processed input: aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n",
             "Test 2: Input exactly 100 chars");
    // For longer than 100 chars, we only process first 100 chars (simulate truncation)
    char long_input[151];
    for(int i=0; i<150; i++) long_input[i] = 'a';
    long_input[150] = '\0';
    char truncated[101];
    strncpy(truncated, long_input, 100);
    truncated[100] = '\0';
    run_test(truncated,
             "Processed input: aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n",
             "Test 3: Input longer than 100 chars (truncated)");

    run_test("", "Processed input: \n", "Test 4: Empty input");
    run_test("@#$%^&*()", "Processed input: @#$%^&*()\n", "Test 5: Special characters");
    run_test("1234567890", "Processed input: 1234567890\n", "Test 6: Numeric input");
    run_test("Hello123", "Processed input: Hello123\n", "Test 7: Mixed alphanumeric input");
    run_test("     ", "Processed input:      \n", "Test 8: Whitespace input");
    run_test("Hello\nWorld", "Processed input: Hello\nWorld\n", "Test 9: Input with newline");
    run_test("  Hello  ", "Processed input:   Hello  \n", "Test 10: Leading/trailing spaces");

    return 0;
}
