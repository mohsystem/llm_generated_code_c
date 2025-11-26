#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


// #include "../../source/output/claude/c/Task16_CLAUDE_claude-3-5-sonnet-20240620.c"  // Include the header for the function
// #include "../../source/output/codestral/c/Task16_MISTRAL_codestral-latest.c"  // Include the header for the function
// #include "../../source/output/gemini/c/Task16_GEMINI_gemini-1.5-pro-001.c"  // Include the header for the function
// #include "../../source/output/gpt4o/c/Task16_OPENAI_gpt-4o.c"  // Include the header for the function
#include "../../source/output/llama-3/c/Task16_PERPLEXITY_llama-3-sonar-large-32k-chat.c"  // Include the header for the function





void testSolution() {
    assert(solution("abc", "bc") == true);
    assert(solution("abc", "d") == false);
    assert(solution("abc", "") == true);
    assert(solution("", "a") == false);
    assert(solution("hello", "hello") == true);
    assert(solution("short", "longerEnding") == false);
    assert(solution("startmiddleend", "start") == false);
    assert(solution("substring", "str") == false);
    assert(solution("HELLO", "hello") == false);
    assert(solution("testtesttest", "test") == true);
}

int main() {
    testSolution();
    printf("All test cases passed.\n");
    return 0;
}