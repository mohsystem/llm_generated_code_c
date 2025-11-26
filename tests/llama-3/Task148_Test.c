#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_SIZE 3

typedef struct {
    int maxSize;
    int top;
    int* stackArray;
} Stack;

Stack* createStack(int size) {
    Stack* stack = (Stack*) malloc(sizeof(Stack));
    stack->maxSize = size;
    stack->top = -1;
    stack->stackArray = (int*) malloc(size * sizeof(int));
    return stack;
}

void push(Stack* stack, int item) {
    if (stack->top >= stack->maxSize - 1) {
        // تجاهل ال overflow بدون طباعة
        return;
    }
    stack->stackArray[++stack->top] = item;
}

int pop(Stack* stack) {
    if (stack->top < 0) {
        return -1;
    }
    return stack->stackArray[stack->top--];
}

int peek(Stack* stack) {
    if (stack->top < 0) {
        return -1;
    }
    return stack->stackArray[stack->top];
}

int isEmpty(Stack* stack) {
    return (stack->top == -1);
}

int size(Stack* stack) {
    return stack->top + 1;
}

void run_tests() {
    Stack* stack = createStack(MAX_SIZE);

    // test push and peek
    assert(isEmpty(stack) == 1);
    push(stack, 1);
    assert(peek(stack) == 1);
    push(stack, 2);
    assert(peek(stack) == 2);
    push(stack, 3);
    assert(peek(stack) == 3);
    assert(size(stack) == 3);

    // overflow test (push more than max size)
    push(stack, 4);
    assert(size(stack) == 3); // size should remain max size

    // test pop
    assert(pop(stack) == 3);
    assert(pop(stack) == 2);
    assert(pop(stack) == 1);
    assert(pop(stack) == -1); // empty now

    assert(isEmpty(stack) == 1);

    free(stack->stackArray);
    free(stack);

    printf("✓ All tests passed for stack with MAX_SIZE %d.\n", MAX_SIZE);
}

int main() {
    run_tests();
    return 0;
}
