#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct Stack {
    int* items;
    int top;
    int maxSize;
} Stack;

Stack* createStack(int size) {
    Stack* stack = (Stack*) malloc(sizeof(Stack));
    stack->items = (int*) malloc(sizeof(int) * size);
    stack->top = -1;
    stack->maxSize = size;
    return stack;
}

void push(Stack* stack, int item) {
    if (stack->top < stack->maxSize - 1) {
        stack->items[++stack->top] = item;
    } else {
        // تجاهل overflow بدلاً من الطباعة
    }
}

int pop(Stack* stack) {
    if (stack->top >= 0) {
        return stack->items[stack->top--];
    }
    return -1;
}

int peek(Stack* stack) {
    if (stack->top >= 0) {
        return stack->items[stack->top];
    }
    return -1;
}

int is_empty(Stack* stack) {
    return stack->top == -1;
}

int size(Stack* stack) {
    return stack->top + 1;
}

void run_tests() {
    Stack* stack = createStack(5);

    assert(is_empty(stack) == 1);
    push(stack, 1);
    push(stack, 2);
    push(stack, 3);
    assert(size(stack) == 3);

    assert(pop(stack) == 3);
    assert(peek(stack) == 2);
    assert(size(stack) == 2);

    // pop remaining
    assert(pop(stack) == 2);
    assert(pop(stack) == 1);
    assert(pop(stack) == -1); // empty

    assert(is_empty(stack) == 1);

    // test overflow ignored
    for (int i = 0; i < 10; i++) {
        push(stack, i);
    }
    assert(size(stack) == 5);  // maxSize

    free(stack->items);
    free(stack);

    printf("✓ All tests passed for dynamic Stack implementation.\n");
}

int main() {
    run_tests();
    return 0;
}
