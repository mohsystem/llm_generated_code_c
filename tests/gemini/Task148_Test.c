#include <stdio.h>
#include <assert.h>

#define MAX_SIZE 100

typedef struct {
    int arr[MAX_SIZE];
    int top;
} Stack;

void initialize(Stack *stack) {
    stack->top = -1;
}

void push(Stack *stack, int item) {
    if (stack->top == MAX_SIZE - 1) {
        // تجاهل overflow بدل الطباعة
        return;
    }
    stack->top++;
    stack->arr[stack->top] = item;
}

int pop(Stack *stack) {
    if (stack->top == -1) {
        return -1; // underflow
    }
    int item = stack->arr[stack->top];
    stack->top--;
    return item;
}

int peek(Stack *stack) {
    if (stack->top == -1) {
        return -1; // empty
    }
    return stack->arr[stack->top];
}

int is_empty(Stack *stack) {
    return stack->top == -1;
}

int size(Stack *stack) {
    return stack->top + 1;
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

    // test is_empty
    assert(is_empty(&stack) == 1);
    push(&stack, 5);
    assert(is_empty(&stack) == 0);

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
