#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


// #include "../../source/output/claude/c/Task12_CLAUDE_claude-3-5-sonnet-20240620.c"  // Include the header for the function
// #include "../../source/output/codestral/c/Task12_MISTRAL_codestral-latest.c"  // Include the header for the function
// #include "../../source/output/gemini/c/Task12_GEMINI_gemini-1.5-pro-001.c"  // Include the header for the function
// #include "../../source/output/gpt4o/c/Task12_OPENAI_gpt-4o.c"  // Include the header for the function
#include "../../source/output/llama-3/c/Task12_PERPLEXITY_llama-3-sonar-large-32k-chat.c"  // Include the header for the function
// Helper function to run a test case




// Global variables to track test results
int tests_passed = 0;
int tests_failed = 0;

// Helper function to handle assertions and print test results
void assertTest(bool condition, const char* testName) {
    if (condition) {
        printf("[PASS] %s\n", testName);
        tests_passed++;
    } else {
        printf("[FAIL] %s\n", testName);
        tests_failed++;
    }
}

// Test cases

void testBasicExample1() {
    char inputString[] = "man i need a taxi up to ubud";
    assertTest(strcmp(high(inputString), "taxi") == 0, "testBasicExample1");
}

void testBasicExample2() {
    char inputString[] = "what time are we climbing up the volcano";
    assertTest(strcmp(high(inputString), "volcano") == 0, "testBasicExample2");
}

void testBasicExample3() {
    char inputString[] = "take me to semynak";
    assertTest(strcmp(high(inputString), "semynak") == 0, "testBasicExample3");
}

void testSingleWord() {
    char inputString[] = "hello";
    assertTest(strcmp(high(inputString), "hello") == 0, "testSingleWord");
}

void testMultipleWordsSameScore() {
    char inputString[] = "apple banana cherry";
    assertTest(strcmp(high(inputString), "cherry") == 0, "testMultipleWordsSameScore");
}

void testDifferentScores() {
    char inputString[] = "ant bear zebra";
    assertTest(strcmp(high(inputString), "zebra") == 0, "testDifferentScores");
}

void testWordsWithOneLetter() {
    char inputString[] = "a b c d e f g h i j k l m n o p q r s t u v w x y z";
    assertTest(strcmp(high(inputString), "z") == 0, "testWordsWithOneLetter");
}

void testEmptyString() {
    char inputString[] = "";
    assertTest(strcmp(high(inputString), "") == 0, "testEmptyString");
}

void testWordsWithSpecialCharacters() {
    char inputString[] = "hello world";
    assertTest(strcmp(high(inputString), "world") == 0, "testWordsWithSpecialCharacters");
}

void testLongWords() {
    char inputString[] = "pneumoultramicroscopicsilicovolcanoconiosis supercalifragilisticexpialidocious antidisestablishmentarianism";
    assertTest(strcmp(high(inputString), "pneumoultramicroscopicsilicovolcanoconiosis") == 0, "testLongWords");
}

int main() {
    testBasicExample1();
    testBasicExample2();
    testBasicExample3();
    testSingleWord();
    testMultipleWordsSameScore();
    testDifferentScores();
    testWordsWithOneLetter();
    testEmptyString();
    testWordsWithSpecialCharacters();
    testLongWords();

    printf("\nTest Results:\n");
    printf("Passed: %d\n", tests_passed);
    printf("Failed: %d\n", tests_failed);

    return tests_failed == 0 ? 0 : 1;
}
