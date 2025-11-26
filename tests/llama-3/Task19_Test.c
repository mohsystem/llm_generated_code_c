#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


//#include "../../source/output/claude/c/Task19_CLAUDE_claude-3-5-sonnet-20240620.c"  // Include the header for the function
// #include "../../source/output/codestral/c/Task19_MISTRAL_codestral-latest.c"  // Include the header for the function
// #include "../../source/output/gemini/c/Task19_GEMINI_gemini-1.5-pro-001.c"  // Include the header for the function
// #include "../../source/output/gpt4o/c/Task19_OPENAI_gpt-4o.c"  // Include the header for the function
 #include "../../source/output/llama-3/c/Task19_PERPLEXITY_llama-3-sonar-large-32k-chat.c"  // Include the header for the function

int main() {
char s1[] = "Hey fellow warriors";
char* result1 = reverseWords(s1);
printf("Result: %s\n", result1);  // Expected: "Hey wollef sroirraw"

char s2[] = "This is a test";
char* result2 = reverseWords(s2);
printf("Result: %s\n", result2);  // Expected: "This is a test"

char s3[] = "This is another test";
char* result3 = reverseWords(s3);
printf("Result: %s\n", result3);  // Expected: "This is rehtona test"

char s4[] = "Welcome to the jungle";
char* result4 = reverseWords(s4);
printf("Result: %s\n", result4);  // Expected: "emocleW to the elgnuj"

char s5[] = "You are amazing";
char* result5 = reverseWords(s5);
printf("Result: %s\n", result5);  // Expected: "You are gnizama"

char s6[] = "Just a simple example";
char* result6 = reverseWords(s6);
printf("Result: %s\n", result6);  // Expected: "Just a elpmis elpmaxe"

char s7[] = "Short words are not reversed";
char* result7 = reverseWords(s7);
printf("Result: %s\n", result7);  // Expected: "trohS sdrow are not desrever"

char s8[] = "Reverse only long words";
char* result8 = reverseWords(s8);
printf("Result: %s\n", result8);  // Expected: "esreveR only long sdrow"

char s9[] = "Exceptional cases should work too";
char* result9 = reverseWords(s9);
printf("Result: %s\n", result9);  // Expected: "lanoitpecxE sesac dluohs work too"

char s10[] = "This sentence contains multiple long words";
char* result10 = reverseWords(s10);
printf("Result: %s\n", result10);  // Expected: "This ecnetnes sniatnoc elpitlum long sdrow"

// Free allocated memory
free(result1);
free(result2);
free(result3);
free(result4);
free(result5);
free(result6);
free(result7);
free(result8);
free(result9);
free(result10);

return 0;
}