#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


// #include "../../source/output/claude/c/Task33_CLAUDE_claude-3-5-sonnet-20240620.c"  // Include the header for the function
// #include "../../source/output/codestral/c/Task33_MISTRAL_codestral-latest.c"  // Include the header for the function
// #include "../../source/output/gemini/c/Task33_GEMINI_gemini-1.5-pro-001.c"  // Include the header for the function
// #include "../../source/output/gpt4o/c/Task33_OPENAI_gpt-4o.c"  // Include the header for the function
#include "../../source/output/llama-3/c/Task33_PERPLEXITY_llama-3-sonar-large-32k-chat.c"  // Include the header for the function


void testSingleTill() {
    // All customers have to wait for the single till
    int customers[] = {5, 3, 4};
    assert(queueTime(customers, 3, 1) == 12);
}

void testTwoTills() {
    // The second till finishes before the first
    int customers[] = {10, 2, 3, 3};
    assert(queueTime(customers, 4, 2) == 10);
}

void testTwoTillsDifferentOrder() {
    // The tills have different workloads
    int customers[] = {2, 3, 10};
    assert(queueTime(customers, 3, 2) == 12);
}

void testMultipleTills() {
    // More tills than customers
    int customers[] = {5, 3, 4};
    assert(queueTime(customers, 3, 5) == 5);
}

void testEqualDistribution() {
    // Evenly distributed workload
    int customers[] = {1, 2, 3, 4};
    assert(queueTime(customers, 4, 2) == 6);
}

void testEmptyQueue() {
    // No customers
    int customers[] = {};
    assert(queueTime(customers, 0, 1) == 0);
}

void testOneCustomerMultipleTills() {
    // Single customer with multiple tills
    int customers[] = {5};
    assert(queueTime(customers, 1, 2) == 5);
}

void testLongQueue() {
    // Long queue of customers with minimal tills
    int customers[] = {2, 2, 3, 3, 4, 4, 5, 5};
    assert(queueTime(customers, 8, 1) == 28);
}

void testComplexDistribution() {
    // Complex distribution of customers
    int customers[] = {2, 3, 4, 3, 2, 1};
    assert(queueTime(customers, 6, 3) == 5);
}

void testHighNumberOfTills() {
    // High number of tills, fewer customers
    int customers[] = {1, 1, 1, 1, 1, 1};
    assert(queueTime(customers, 6, 10) == 1);
}

int main() {
    // Running all test cases
    testSingleTill();
    testTwoTills();
    testTwoTillsDifferentOrder();
    testMultipleTills();
    testEqualDistribution();
    testEmptyQueue();
    testOneCustomerMultipleTills();
    testLongQueue();
    testComplexDistribution();
    testHighNumberOfTills();

    printf("All tests passed!\n");

    return 0;
}