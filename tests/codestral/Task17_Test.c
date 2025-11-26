#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


// #include "../../source/output/claude/c/Task17_CLAUDE_claude-3-5-sonnet-20240620.c"  // Include the header for the function
#include <stdlib.h>

#include "../../source/output/codestral/c/Task17_MISTRAL_codestral-latest.c"  // Include the header for the function
// #include "../../source/output/gemini/c/Task17_GEMINI_gemini-1.5-pro-001.c"  // Include the header for the function
// #include "../../source/output/gpt4o/c/Task17_OPENAI_gpt-4o.c"  // Include the header for the function
// #include "../../source/output/llama-3/c/Task17_PERPLEXITY_llama-3-sonar-large-32k-chat.c"  // Include the header for the function





// Function to test the toJadenCase function
void testToJadenCase() {
    char test1[] = "how can mirrors be real if our eyes aren't real";
    char expected1[] = "How Can Mirrors Be Real If Our Eyes Aren't Real";
    assert(strcmp(toJadenCase(test1), expected1) == 0);

    char test2[] = "the karate kid is a great movie";
    char expected2[] = "The Karate Kid Is A Great Movie";
    assert(strcmp(toJadenCase(test2), expected2) == 0);

    char test3[] = "after earth was released in 2013";
    char expected3[] = "After Earth Was Released In 2013";
    assert(strcmp(toJadenCase(test3), expected3) == 0);

    char test4[] = "i think therefore i am";
    char expected4[] = "I Think Therefore I Am";
    assert(strcmp(toJadenCase(test4), expected4) == 0);

    char test5[] = "to be or not to be";
    char expected5[] = "To Be Or Not To Be";
    assert(strcmp(toJadenCase(test5), expected5) == 0);

    char test6[] = "life is but a dream";
    char expected6[] = "Life Is But A Dream";
    assert(strcmp(toJadenCase(test6), expected6) == 0);

    char test7[] = "it's a beautiful day in the neighborhood";
    char expected7[] = "It's A Beautiful Day In The Neighborhood";
    assert(strcmp(toJadenCase(test7), expected7) == 0);

    char test8[] = "null string should return null";
    char expected8[] = "Null String Should Return Null";
    assert(strcmp(toJadenCase(test8), expected8) == 0);

    char test9[] = "";
    assert(toJadenCase(test9) == test9); // Expecting the empty string to remain unchanged

    char test10[] = "     ";
    char expected10[] = "     "; // Spaces should remain unchanged
    assert(strcmp(toJadenCase(test10), expected10) == 0);
}

// Main function to run the tests
int main() {
    testToJadenCase();
    printf("All test cases passed.\n");
    return 0;
}
