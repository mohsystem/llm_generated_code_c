#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


//#include "../../source/output/claude/c/Task8_CLAUDE_claude-3-5-sonnet-20240620.c"  // Include the header for the function
 #include "../../source/output/codestral/c/Task8_MISTRAL_codestral-latest.c"  // Include the header for the function
// #include "../../source/output/gemini/c/Task8_GEMINI_gemini-1.5-pro-001.c"  // Include the header for the function
// #include "../../source/output/gpt4o/c/Task8_OPENAI_gpt-4o.c"  // Include the header for the function
// #include "../../source/output/llama-3/c/Task8_PERPLEXITY_llama-3-sonar-large-32k-chat.c"  // Include the header for the function
// Helper function to run a test case



void run_test_case(char* chars, int size, char expected) {
 char result = findMissingLetter(chars, size);
 assert(result == expected);
 printf("Test passed for missing letter: %c\n", result);
}

int main() {
 char arr1[] = {'a', 'b', 'c', 'd', 'f'};
 run_test_case(arr1, 5, 'e');

 char arr2[] = {'b', 'c', 'd', 'f', 'g'};
 run_test_case(arr2, 5, 'e');

 char arr3[] = {'a', 'c', 'd', 'e', 'f', 'g'};
 run_test_case(arr3, 6, 'b');

 char arr4[] = {'O', 'Q', 'R', 'S'};
 run_test_case(arr4, 4, 'P');

 char arr5[] = {'C', 'D', 'E', 'G', 'H'};
 run_test_case(arr5, 5, 'F');

 char arr6[] = {'A', 'B', 'D', 'E', 'F', 'G'};
 run_test_case(arr6, 6, 'C');

 char arr7[] = {'a', 'b', 'd'};
 run_test_case(arr7, 3, 'c');

 char arr8[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'n'};
 run_test_case(arr8, 13, 'm');

 char arr9[] = {'X', 'Z'};
 run_test_case(arr9, 2, 'Y');

 char arr10[] = {'a', 'c'};
 run_test_case(arr10, 2, 'b');

 printf("All test cases passed successfully.\n");
 return 0;
}



