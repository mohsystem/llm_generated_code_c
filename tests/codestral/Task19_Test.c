#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


//#include "../../source/output/claude/c/Task19_CLAUDE_claude-3-5-sonnet-20240620.c"  // Include the header for the function
 #include "../../source/output/codestral/c/Task19_MISTRAL_codestral-latest.c"  // Include the header for the function
 //#include "../../source/output/gemini/c/Task19_GEMINI_gemini-1.5-pro-001.c"  // Include the header for the function
 //#include "../../source/output/gpt4o/c/Task19_OPENAI_gpt-4o.c"  // Include the header for the function
 // #include "../../source/output/llama-3/c/Task19_PERPLEXITY_llama-3-sonar-large-32k-chat.c"  // Include the header for the function





int main() {
    char s1[] = "Hey fellow warriors";
    printf("Result: ");
    spinWords(s1);  // Expected: "Hey wollef sroirraw"
    printf("\n");

    char s2[] = "This is a test";
    printf("Result: ");
    spinWords(s2);  // Expected: "This is a test"
    printf("\n");

    char s3[] = "This is another test";
    printf("Result: ");
    spinWords(s3);  // Expected: "This is rehtona test"
    printf("\n");

    char s4[] = "Welcome to the jungle";
    printf("Result: ");
    spinWords(s4);  // Expected: "emocleW to the elgnuj"
    printf("\n");

    char s5[] = "You are amazing";
    printf("Result: ");
    spinWords(s5);  // Expected: "You are gnizama"
    printf("\n");

    char s6[] = "Just a simple example";
    printf("Result: ");
    spinWords(s6);  // Expected: "Just a elpmis elpmaxe"
    printf("\n");

    char s7[] = "Short words are not reversed";
    printf("Result: ");
    spinWords(s7);  // Expected: "trohS sdrow are not desrever"
    printf("\n");

    char s8[] = "Reverse only long words";
    printf("Result: ");
    spinWords(s8);  // Expected: "esreveR only long sdrow"
    printf("\n");

    char s9[] = "Exceptional cases should work too";
    printf("Result: ");
    spinWords(s9);  // Expected: "lanoitpecxE sesac dluohs work too"
    printf("\n");

    char s10[] = "This sentence contains multiple long words";
    printf("Result: ");
    spinWords(s10);  // Expected: "This ecnetnes sniatnoc elpitlum long sdrow"
    printf("\n");

    return 0;
}