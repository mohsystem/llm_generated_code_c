#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


// #include "../../source/output/claude/c/Task15_CLAUDE_claude-3-5-sonnet-20240620.c"  // Include the header for the function
#include "../../source/output/codestral/c/Task15_MISTRAL_codestral-latest.c"  // Include the header for the function
// #include "../../source/output/gemini/c/Task15_GEMINI_gemini-1.5-pro-001.c"  // Include the header for the function
// #include "../../source/output/gpt4o/c/Task15_OPENAI_gpt-4o.c"  // Include the header for the function
// #include "../../source/output/llama-3/c/Task15_PERPLEXITY_llama-3-sonar-large-32k-chat.c"  // Include the header for the function
// Helper function to run a test case


void freeTower(char** tower, int nFloors) {
    for (int i = 0; i < nFloors; i++) {
        free(tower[i]);
    }
    free(tower);
}

// Helper function to compare two arrays of strings
void assertArrayEquals(char** expected, char** actual, int length, const char* testName) {
    int passed = 1;
    for (int i = 0; i < length; i++) {
        if (strcmp(expected[i], actual[i]) != 0) {
            passed = 0;
            printf("[FAIL] %s: Expected \"%s\", but got \"%s\"\n", testName, expected[i], actual[i]);
        }
    }
    if (passed) {
        printf("[PASS] %s\n", testName);
    }
}

int main() {
    int returnSize;

    // {
    //     char* expected[] = {"*"};
    //     char** result = towerBuilder(1, &returnSize);
    //     assertArrayEquals(expected, result, returnSize, "testOneFloor");
    //     freeTower(result, returnSize);
    // }

    // Test case 2: Two floors
    {
        char* expected[] = {
            " * ",
            "***"
        };
        char** result = towerBuilder(2, &returnSize);
        assertArrayEquals(expected, result, returnSize, "testTwoFloors");
        freeTower(result, returnSize);
    }

    // Test case 4: Four floors
    {
        char* expected[] = {
            "   *   ",
            "  ***  ",
            " ***** ",
            "*******"
        };
        char** result = towerBuilder(4, &returnSize);
        assertArrayEquals(expected, result, returnSize, "testFourFloors");
        freeTower(result, returnSize);
    }

    // Test case 5: Five floors
    {
        char* expected[] = {
            "    *    ",
            "   ***   ",
            "  *****  ",
            " ******* ",
            "*********"
        };
        char** result = towerBuilder(5, &returnSize);
        assertArrayEquals(expected, result, returnSize, "testFiveFloors");
        freeTower(result, returnSize);
    }
    // Test case 6: Six floors
    {
        char* expected[] = {
            "     *     ",
            "    ***    ",
            "   *****   ",
            "  *******  ",
            " ********* ",
            "***********"
        };
        char** result = towerBuilder(6, &returnSize);
        assertArrayEquals(expected, result, returnSize, "testSixFloors");
        freeTower(result, returnSize);
    }

     // Test case 7: Seven floors
    {
        char* expected[] = {
            "      *      ",
            "     ***     ",
            "    *****    ",
            "   *******   ",
            "  *********  ",
            " *********** ",
            "*************"
        };
        char** result = towerBuilder(7, &returnSize);
        assertArrayEquals(expected, result, returnSize, "testSevenFloors");
        freeTower(result, returnSize);
    }

    // Test case 8: Eight floors
    {
        char* expected[] = {
            "       *       ",
            "      ***      ",
            "     *****     ",
            "    *******    ",
            "   *********   ",
            "  ***********  ",
            " ************* ",
            "***************"
        };
        char** result = towerBuilder(8, &returnSize);
        assertArrayEquals(expected, result, returnSize, "testEightFloors");
        freeTower(result, returnSize);
    }

    // Test case 9: Nine floors
    {
        char* expected[] = {
            "        *        ",
            "       ***       ",
            "      *****      ",
            "     *******     ",
            "    *********    ",
            "   ***********   ",
            "  *************  ",
            " *************** ",
            "*****************"
        };
        char** result = towerBuilder(9, &returnSize);
        assertArrayEquals(expected, result, returnSize, "testNineFloors");
        freeTower(result, returnSize);
    }

    // Test case 10: Ten floors
    {
        char* expected[] = {
            "         *         ",
            "        ***        ",
            "       *****       ",
            "      *******      ",
            "     *********     ",
            "    ***********    ",
            "   *************   ",
            "  ***************  ",
            " ***************** ",
            "*******************"
        };
        char** result = towerBuilder(10, &returnSize);
        assertArrayEquals(expected, result, returnSize, "testTenFloors");
        freeTower(result, returnSize);
    }

    printf("\nAll test cases completed.\n");

    return 0;
}