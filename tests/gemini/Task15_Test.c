#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


// #include "../../source/output/claude/c/Task15_CLAUDE_claude-3-5-sonnet-20240620.c"  // Include the header for the function
// #include "../../source/output/codestral/c/Task15_MISTRAL_codestral-latest.c"  // Include the header for the function
#include "../../source/output/gemini/c/Task15_GEMINI_gemini-1.5-pro-001.c"  // Include the header for the function
// #include "../../source/output/gpt4o/c/Task15_OPENAI_gpt-4o.c"  // Include the header for the function
// #include "../../source/output/llama-3/c/Task15_PERPLEXITY_llama-3-sonar-large-32k-chat.c"  // Include the header for the function
// Helper function to run a test case




int main() {
    int nFloors = 1;
    char** tower = tower_builder(nFloors);
    for (int i = 0; i < nFloors; i++) {
        printf("%s\n", tower[i]);
        free(tower[i]);
    }
    free(tower);

    printf("\n");

    nFloors = 2;
    tower = tower_builder(nFloors);
    for (int i = 0; i < nFloors; i++) {
        printf("%s\n", tower[i]);
        free(tower[i]);
    }
    free(tower);
    nFloors = 5;
    tower = tower_builder(nFloors);
    for (int i = 0; i < nFloors; i++) {
        printf("%s\n", tower[i]);
        free(tower[i]);
    }
    free(tower);
    nFloors = 7;
    tower = tower_builder(nFloors);
    for (int i = 0; i < nFloors; i++) {
        printf("%s\n", tower[i]);
        free(tower[i]);
    }
    free(tower);
    nFloors = 9;
    tower = tower_builder(nFloors);
    for (int i = 0; i < nFloors; i++) {
        printf("%s\n", tower[i]);
        free(tower[i]);
    }
    free(tower);

    return 0;
}
