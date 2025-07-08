#include <stdio.h>
#include <assert.h>

#define MAX_SIZE 100

typedef struct {
    int items[MAX_SIZE];
    int top;
} Stack;

void initialize(Stack* stack) {
    stack->top = 0;
}

void push(Stack* stack, int item) {
    if (stack->top >= MAX_SIZE) {
        // تجاهل إذا overflow بدلاً من الطباعة
        return;
    }
    stack->items[stack->top++] = item;
}

int pop(Stack* stack) {
    if (stack->top <= 0) {
        return -1;
    }
    return stack->items[--stack->top];
}

int peek(Stack* stack) {
    if (stack->top <= 0) {
        return -1;
    }
    return stack->items[stack->top - 1];
}

int size(Stack* stack) {
    return stack->top;
}

int isEmpty(Stack* stack) {
    return stack->top == 0;
}

void run_tests() {
    Stack stack;
    initialize(&stack);

    // test push and peek
    push(&stack, 10);
    assert(peek(&stack) == 10);
    push(&stack, 20);
    assert(peek(&stack) == 20);

    // test pop
    assert(pop(&stack) == 20);
    assert(pop(&stack) == 10);
    assert(pop(&stack) == -1); // empty stack

    // test isEmpty
    assert(isEmpty(&stack) == 1);
    push(&stack, 5);
    assert(isEmpty(&stack) == 0);

    // test size
    assert(size(&stack) == 1);
    push(&stack, 15);
    assert(size(&stack) == 2);
    pop(&stack);
    assert(size(&stack) == 1);

    // test overflow (push more than MAX_SIZE)
    for (int i = 0; i < MAX_SIZE + 10; i++) {
        push(&stack, i);
    }
    assert(size(&stack) == MAX_SIZE);

    printf("✓ All tests passed for stack implementation.\n");
}

int main() {
    run_tests();
    return 0;
}
