#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int n;
    sem_t zeroSemaphore;
    sem_t evenSemaphore;
    sem_t oddSemaphore;
} ZeroEvenOdd;

ZeroEvenOdd* zeroEvenOddCreate(int n) {
    ZeroEvenOdd* obj = (ZeroEvenOdd*)malloc(sizeof(ZeroEvenOdd));
    obj->n = n;
    sem_init(&obj->zeroSemaphore, 0, 1);
    sem_init(&obj->evenSemaphore, 0, 0);
    sem_init(&obj->oddSemaphore, 0, 0);
    return obj;
}

void zero(ZeroEvenOdd* obj, void (*printNumber)(int)) {
    for (int i = 0; i < obj->n; i++) {
        sem_wait(&obj->zeroSemaphore);
        printNumber(0);
        if (i % 2 == 0) {
            sem_post(&obj->oddSemaphore);
        } else {
            sem_post(&obj->evenSemaphore);
        }
    }
}

void even(ZeroEvenOdd* obj, void (*printNumber)(int)) {
    for (int i = 2; i <= obj->n; i += 2) {
        sem_wait(&obj->evenSemaphore);
        printNumber(i);
        sem_post(&obj->zeroSemaphore);
    }
}

void odd(ZeroEvenOdd* obj, void (*printNumber)(int)) {
    for (int i = 1; i <= obj->n; i += 2) {
        sem_wait(&obj->oddSemaphore);
        printNumber(i);
        sem_post(&obj->zeroSemaphore);
    }
}

void printNumber(int x) {
    printf("%d", x);
}

// Wrapper functions for pthread_create
void* zeroWrapper(void* arg) {
    ZeroEvenOdd* obj = (ZeroEvenOdd*)arg;
    zero(obj, printNumber);
    return NULL;
}

void* evenWrapper(void* arg) {
    ZeroEvenOdd* obj = (ZeroEvenOdd*)arg;
    even(obj, printNumber);
    return NULL;
}

void* oddWrapper(void* arg) {
    ZeroEvenOdd* obj = (ZeroEvenOdd*)arg;
    odd(obj, printNumber);
    return NULL;
}

void test_case(int n, const char* expected_output) {
    ZeroEvenOdd* obj = zeroEvenOddCreate(n);

    pthread_t t1, t2, t3;
    // Create threads for zero, even, and odd
    pthread_create(&t1, NULL, zeroWrapper, obj);
    pthread_create(&t2, NULL, evenWrapper, obj);
    pthread_create(&t3, NULL, oddWrapper, obj);

    // Wait for threads to finish
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    // Compare output with expected
    char result[100];
    snprintf(result, sizeof(result), "%d", n);  // Adjust as per actual output logic
    if (strcmp(result, expected_output) == 0) {
        printf("Test case n=%d: Pass\n", n);
    } else {
        printf("Test case n=%d: Fail\n", n);
    }

    free(obj);
}

int main() {
    // Define the test cases and expected outputs
    int test_cases[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const char* expected_outputs[] = {
        "01", "0102", "010203", "01020304", "01020305",
        "010203040506", "01020304050607", "0102030405060708",
        "010203040506070809", "010203040506070809010"
    };

    // Run the test cases
    for (int i = 0; i < 10; i++) {
        printf("Running test case %d...\n", i + 1);
        test_case(test_cases[i], expected_outputs[i]);
    }

    return 0;
}
