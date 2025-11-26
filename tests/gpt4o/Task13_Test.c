#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


// #include "../../source/output/claude/c/Task13_CLAUDE_claude-3-5-sonnet-20240620.c"  // Include the header for the function
// #include "../../source/output/codestral/c/Task13_MISTRAL_codestral-latest.c"  // Include the header for the function
// #include "../../source/output/gemini/c/Task13_GEMINI_gemini-1.5-pro-001.c"  // Include the header for the function
#include "../../source/output/gpt4o/c/Task13_OPENAI_gpt-4o.c"  // Include the header for the function
// #include "../../source/output/llama-3/c/Task13_PERPLEXITY_llama-3-sonar-large-32k-chat.c"  // Include the header for the function
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
    // Test cases based on Java TCs
    assertTest((char[]){'n', 's', 'n', 's', 'n', 's', 'n', 's', 'n', 's'}, 10, true, "testCase1");
    assertTest((char[]){'w', 'e', 'w', 'e', 'w', 'e', 'w', 'e', 'w', 'e', 'w', 'e'}, 12, false, "testCase2");
    assertTest((char[]){'w'}, 1, false, "testCase3");
    assertTest((char[]){'n', 'n', 'n', 's', 'n', 's', 'n', 's', 'n', 's'}, 10, false, "testCase4");
    assertTest((char[]){'n', 's', 'e', 'w', 'n', 's', 'e', 'w', 'n', 's'}, 10, true, "testCase5");
    assertTest((char[]){'n', 'e', 's', 'w', 'n', 'e', 's', 'w', 'n', 's'}, 10, true, "testCase6");
    assertTest((char[]){'n', 'n', 'n', 'n', 's', 's', 's', 's', 'e', 'w'}, 10, true, "testCase7");
    assertTest((char[]){'n', 's', 'n', 's', 'e', 'w', 'e', 'w', 'n', 's'}, 10, true, "testCase8");
    assertTest((char[]){'n', 'n', 'n', 'n', 'n', 'n', 'n', 'n', 'n', 'n'}, 10, false, "testCase9");
    assertTest((char[]){'n', 'n', 'e', 'e', 's', 's', 'w', 'w', 'n', 's'}, 10, true, "testCase10");

    printf("\nAll test cases completed.\n");

    return 0;
}