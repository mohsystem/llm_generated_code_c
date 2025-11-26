#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


// #include "../../source/output/claude/c/Task31_CLAUDE_claude-3-5-sonnet-20240620.c"  // Include the header for the function
// #include "../../source/output/codestral/c/Task31_MISTRAL_codestral-latest.c"  // Include the header for the function
// #include "../../source/output/gemini/c/Task31_GEMINI_gemini-1.5-pro-001.c"  // Include the header for the function
#include "../../source/output/gpt4o/c/Task31_OPENAI_gpt-4o.c"  // Include the header for the function
// #include "../../source/output/llama-3/c/Task31_PERPLEXITY_llama-3-sonar-large-32k-chat.c"  // Include the header for the function


void testComplementaryStrand1() {
    char complement[6]; // Enough space for "TAACG" + '\0'
    dnaStrand("ATTGC", complement);
    assert(strcmp(complement, "TAACG") == 0);
}

void testComplementaryStrand2() {
    char complement[5]; // Enough space for "CATA" + '\0'
    dnaStrand("GTAT", complement);
    assert(strcmp(complement, "CATA") == 0);
}

void testSingleCharacterA() {
    char complement[2]; // Enough space for "T" + '\0'
    dnaStrand("A", complement);
    assert(strcmp(complement, "T") == 0);
}

void testSingleCharacterT() {
    char complement[2]; // Enough space for "A" + '\0'
    dnaStrand("T", complement);
    assert(strcmp(complement, "A") == 0);
}

void testSingleCharacterC() {
    char complement[2]; // Enough space for "G" + '\0'
    dnaStrand("C", complement);
    assert(strcmp(complement, "G") == 0);
}

void testSingleCharacterG() {
    char complement[2]; // Enough space for "C" + '\0'
    dnaStrand("G", complement);
    assert(strcmp(complement, "C") == 0);
}

void testRepeatingCharacters() {
    char complement[5]; // Enough space for "TTTT" + '\0'
    dnaStrand("AAAA", complement);
    assert(strcmp(complement, "TTTT") == 0);
}

void testAlternatingCharacters() {
    char complement[5]; // Enough space for "ATAT" + '\0'
    dnaStrand("TATA", complement);
    assert(strcmp(complement, "ATAT") == 0);
}

void testLongStrand() {
    char complement[11]; // Enough space for "CGCGCGCGCG" + '\0'
    dnaStrand("GCGCGCGCGC", complement);
    assert(strcmp(complement, "CGCGCGCGCG") == 0);
}

int main() {
    // Run all the test cases
    testComplementaryStrand1();
    testComplementaryStrand2();
    testSingleCharacterA();
    testSingleCharacterT();
    testSingleCharacterC();
    testSingleCharacterG();
    testRepeatingCharacters();
    testAlternatingCharacters();
    testLongStrand();

    printf("All tests passed!\n");

    return 0;
}