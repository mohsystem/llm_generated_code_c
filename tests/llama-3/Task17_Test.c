#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


//#include "../../source/output/claude/c/Task17_CLAUDE_claude-3-5-sonnet-20240620.c"  // Include the header for the function
// #include "../../source/output/codestral/c/Task17_MISTRAL_codestral-latest.c"  // Include the header for the function
// #include "../../source/output/gemini/c/Task17_GEMINI_gemini-1.5-pro-001.c"  // Include the header for the function
// #include "../../source/output/gpt4o/c/Task17_OPENAI_gpt-4o.c"  // Include the header for the function
#include <stdlib.h>

 #include "../../source/output/llama-3/c/Task17_PERPLEXITY_llama-3-sonar-large-32k-chat.c"  // Include the header for the function





// Test cases
void testToJadenCase() {
    // Test cases
    char* result;

    // result = toJadenCase("how can mirrors be real if our eyes aren't real");
    // assert(result != NULL && strcmp(result, "How Can Mirrors Be Real If Our Eyes Aren't Real") == 0);
    // free(result);

    result = toJadenCase("the karate kid is a great movie");
    assert(result != NULL && strcmp(result, "The Karate Kid Is A Great Movie") == 0);
    free(result);

    result = toJadenCase("after earth was released in 2013");
    assert(result != NULL && strcmp(result, "After Earth Was Released In 2013") == 0);
    free(result);

    result = toJadenCase("i think therefore i am");
    assert(result != NULL && strcmp(result, "I Think Therefore I Am") == 0);
    free(result);

    result = toJadenCase("to be or not to be");
    assert(result != NULL && strcmp(result, "To Be Or Not To Be") == 0);
    free(result);

    result = toJadenCase("life is but a dream");
    assert(result != NULL && strcmp(result, "Life Is But A Dream") == 0);
    free(result);

    result = toJadenCase("it's a beautiful day in the neighborhood");
    assert(result != NULL && strcmp(result, "It's A Beautiful Day In The Neighborhood") == 0);
    free(result);

    result = toJadenCase("null string should return null");
    assert(result != NULL && strcmp(result, "Null String Should Return Null") == 0);
    free(result);

    result = toJadenCase("");
    assert(result == NULL);

    result = toJadenCase(NULL);
    assert(result == NULL);
}

int main() {
    testToJadenCase();
    printf("All test cases passed.\n");
    return 0;
}