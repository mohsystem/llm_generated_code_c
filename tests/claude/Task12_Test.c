#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


#include "../../source/output/claude/c/Task12_CLAUDE_claude-3-5-sonnet-20240620.c"  // Include the header for the function
// #include "../../source/output/codestral/c/Task12_MISTRAL_codestral-latest.c"  // Include the header for the function
// #include "../../source/output/gemini/c/Task12_GEMINI_gemini-1.5-pro-001.c"  // Include the header for the function
// #include "../../source/output/gpt4o/c/Task12_OPENAI_gpt-4o.c"  // Include the header for the function
// #include "../../source/output/llama-3/c/Task12_PERPLEXITY_llama-3-sonar-large-32k-chat.c"  // Include the header for the function
// Helper function to run a test case





// Test cases

void testBasicExample1() {
    // Test basic example with diverse words
    assert(strcmp(high("man i need a taxi up to ubud"), "taxi") == 0);
}

void testBasicExample2() {
    // Test basic example with different set of words
    assert(strcmp(high("what time are we climbing up the volcano"), "volcano") == 0);
}

void testBasicExample3() {
    // Test basic example with a smaller set of words
    assert(strcmp(high("take me to semynak"), "semynak") == 0);
}

void testSingleWord() {
    // Test with a single word
    assert(strcmp(high("hello"), "hello") == 0);
}

void testMultipleWordsSameScore() {
    // Test with multiple words having the same score
    assert(strcmp(high("apple banana cherry"), "cherry") == 0);
}

void testDifferentScores() {
    // Test with words having different scores
    assert(strcmp(high("ant bear zebra"), "zebra") == 0);
}

void testWordsWithOneLetter() {
    // Test with single-letter words
    assert(strcmp(high("a b c d e f g h i j k l m n o p q r s t u v w x y z"), "z") == 0);
}


void testEmptyString() {
    // Test with an empty string
    assert(strcmp(high(""), "") == 0);
}

void testWordsWithSpecialCharacters() {
    // Test with words containing special characters (should be handled as invalid input in this context)
    assert(strcmp(high("hello world"), "world") == 0);
}

void testLongWords() {
    // Test with long words to check performance
    assert(strcmp(high("pneumoultramicroscopicsilicovolcanoconiosis supercalifragilisticexpialidocious antidisestablishmentarianism"), "pneumoultramicroscopicsilicovolcanoconiosis") == 0);
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

    printf("All test cases passed!\n");
    return 0;
}