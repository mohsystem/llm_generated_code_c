#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define MAX_SIZE 20000  // زودت الحجم ليتوافق مع التستات الكبيرة

typedef struct {
    int items[MAX_SIZE];
    int top;
} Stack;

void initialize(Stack *s) {
    s->top = -1;
}

bool isEmpty(Stack *s) {
    return s->top == -1;
}

bool isFull(Stack *s) {
    return s->top == MAX_SIZE - 1;
}

void push(Stack *s, int item) {
    if (isFull(s)) {
        // في التستات مافيش overflow، فقط نتجاهل الpush اذا صار overflow
        return;
    }
    s->items[++(s->top)] = item;
}

int pop(Stack *s) {
    if (isEmpty(s)) {
        return -1;  // نستخدم -1 للدلالة على stack فارغ (التست يستخدم None في البايثون)
    }
    return s->items[(s->top)--];
}

int peek(Stack *s) {
    if (isEmpty(s)) {
        return -1;  // نستخدم -1 للدلالة على stack فارغ
    }
    return s->items[s->top];
}

int size(Stack *s) {
    return s->top + 1;
}

// اختبارات صارمة تحاكي التست كيسز
void run_tests() {
    Stack stack;
    initialize(&stack);

    // test_push_multiple_elements
    for (int i = 0; i < 1000; i++) push(&stack, i);
    assert(peek(&stack) == 999);

    // test_pop_all_elements
    for (int i = 999; i >= 0; i--) {
        assert(pop(&stack) == i);
    }
    assert(isEmpty(&stack));

    // test_peek_after_pop
    push(&stack, 1);
    push(&stack, 2);
    assert(pop(&stack) == 2);
    assert(peek(&stack) == 1);

    // test_is_empty_after_operations
    initialize(&stack);
    assert(isEmpty(&stack) == true);
    push(&stack, 1);
    push(&stack, 2);
    assert(isEmpty(&stack) == false);
    pop(&stack);
    pop(&stack);
    assert(isEmpty(&stack) == true);

    // test_size_increment
    initialize(&stack);
    assert(size(&stack) == 0);
    push(&stack, 1);
    assert(size(&stack) == 1);
    push(&stack, 2);
    assert(size(&stack) == 2);
    pop(&stack);
    assert(size(&stack) == 1);

    // test_pop_empty_stack
    initialize(&stack);
    assert(pop(&stack) == -1);

    // test_peek_empty_stack
    assert(peek(&stack) == -1);

    // test_push_and_pop_large_elements (simulated with int max value)
    initialize(&stack);
    int large_element = 2147483647; // max int
    push(&stack, large_element);
    assert(peek(&stack) == large_element);
    assert(pop(&stack) == large_element);

    // test_size_with_large_operations
    initialize(&stack);
    for (int i = 0; i < 10000; i++) push(&stack, i);
    assert(size(&stack) == 10000);
    for (int i = 0; i < 5000; i++) pop(&stack);
    assert(size(&stack) == 5000);

    // test_multiple_peeks
    initialize(&stack);
    push(&stack, 1);
    push(&stack, 2);
    push(&stack, 3);
    assert(peek(&stack) == 3);
    pop(&stack);
    assert(peek(&stack) == 2);
    pop(&stack);
    assert(peek(&stack) == 1);

    // test_operations_on_large_stack
    initialize(&stack);
    for (int i = 0; i < 10000; i++) push(&stack, i);
    for (int i = 0; i < 5000; i++) pop(&stack);
    assert(peek(&stack) == 4999);

    printf("✓ All tests passed for Stack implementation.\n");
}

int main() {
    run_tests();
    return 0;
}
