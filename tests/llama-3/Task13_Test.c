#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


// #include "../../source/output/claude/c/Task13_CLAUDE_claude-3-5-sonnet-20240620.c"  // Include the header for the function
// #include "../../source/output/codestral/c/Task13_MISTRAL_codestral-latest.c"  // Include the header for the function
// #include "../../source/output/gemini/c/Task13_GEMINI_gemini-1.5-pro-001.c"  // Include the header for the function
// #include "../../source/output/gpt4o/c/Task13_OPENAI_gpt-4o.c"  // Include the header for the function
#include "../../source/output/llama-3/c/Task13_PERPLEXITY_llama-3-sonar-large-32k-chat.c"  // Include the header for the function
// Helper function to run a test case




// Helper function to run and assert test cases
void assertTest(char walk[], int length, bool expected, const char* testName) {
    bool result = isValidWalk(walk, length);
    if (result == expected) {
        printf("[PASS] %s\n", testName);
    } else {
        printf("[FAIL] %s: Expected '%s', got '%s'\n", testName, expected ? "true" : "false", result ? "true" : "false");
    }
}

int main() {
    // Define test case inputs
    char walk1[] = {'n', 's', 'n', 's', 'n', 's', 'n', 's', 'n', 's'};
    char walk2[] = {'w', 'e', 'w', 'e', 'w', 'e', 'w', 'e', 'w', 'e', 'w', 'e'};
    char walk3[] = {'w'};
    char walk4[] = {'n', 'n', 'n', 's', 'n', 's', 'n', 's', 'n', 's'};
    char walk5[] = {'n', 's', 'e', 'w', 'n', 's', 'e', 'w', 'n', 's'};
    char walk6[] = {'n', 'e', 's', 'w', 'n', 'e', 's', 'w', 'n', 's'};
    char walk7[] = {'n', 'n', 'n', 'n', 's', 's', 's', 's', 'e', 'w'};
    char walk8[] = {'n', 's', 'n', 's', 'e', 'w', 'e', 'w', 'n', 's'};
    char walk9[] = {'n', 'n', 'n', 'n', 'n', 'n', 'n', 'n', 'n', 'n'};
    char walk10[] = {'n', 'n', 'e', 'e', 's', 's', 'w', 'w', 'n', 's'};

    // Test cases
    assertTest(walk1, 10, true, "testCase1");
    assertTest(walk2, 12, false, "testCase2");
    assertTest(walk3, 1, false, "testCase3");
    assertTest(walk4, 10, false, "testCase4");
    assertTest(walk5, 10, true, "testCase5");
    assertTest(walk6, 10, true, "testCase6");
    assertTest(walk7, 10, true, "testCase7");
    assertTest(walk8, 10, true, "testCase8");
    assertTest(walk9, 10, false, "testCase9");
    assertTest(walk10, 10, true, "testCase10");

    printf("\nAll test cases completed.\n");

    return 0;
}