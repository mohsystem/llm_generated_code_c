#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int n;
    int counter;
    pthread_mutex_t mutex;
    pthread_cond_t zeroCond;
    pthread_cond_t evenOddCond;
    int zeroTurn;
} ZeroEvenOdd;

void printNumber(int x) {
    printf("%d", x);
}

void* zero(void* arg) {
    ZeroEvenOdd* obj = (ZeroEvenOdd*) arg;
    for (int i = 0; i < obj->n; ++i) {
        pthread_mutex_lock(&obj->mutex);
        while (!obj->zeroTurn) {
            pthread_cond_wait(&obj->zeroCond, &obj->mutex);
        }
        printNumber(0);
        obj->zeroTurn = 0;
        pthread_cond_signal(&obj->evenOddCond);
        pthread_mutex_unlock(&obj->mutex);
    }
    return NULL;
}

void* even(void* arg) {
    ZeroEvenOdd* obj = (ZeroEvenOdd*) arg;
    for (int i = 2; i <= obj->n; i += 2) {
        pthread_mutex_lock(&obj->mutex);
        while (obj->zeroTurn || obj->counter % 2 != 0) {
            pthread_cond_wait(&obj->evenOddCond, &obj->mutex);
        }
        printNumber(obj->counter++);
        obj->zeroTurn = 1;
        pthread_cond_signal(&obj->zeroCond);
        pthread_mutex_unlock(&obj->mutex);
    }
    return NULL;
}

void* odd(void* arg) {
    ZeroEvenOdd* obj = (ZeroEvenOdd*) arg;
    for (int i = 1; i <= obj->n; i += 2) {
        pthread_mutex_lock(&obj->mutex);
        while (obj->zeroTurn || obj->counter % 2 == 0) {
            pthread_cond_wait(&obj->evenOddCond, &obj->mutex);
        }
        printNumber(obj->counter++);
        obj->zeroTurn = 1;
        pthread_cond_signal(&obj->zeroCond);
        pthread_mutex_unlock(&obj->mutex);
    }
    return NULL;
}

void test_case(int n, const char* expected_output) {
    ZeroEvenOdd obj;
    obj.n = n;
    obj.counter = 1;
    obj.zeroTurn = 1;
    pthread_mutex_init(&obj.mutex, NULL);
    pthread_cond_init(&obj.zeroCond, NULL);
    pthread_cond_init(&obj.evenOddCond, NULL);

    // Create threads for zero, even, and odd
    pthread_t threadA, threadB, threadC;
    pthread_create(&threadA, NULL, zero, &obj);
    pthread_create(&threadB, NULL, even, &obj);
    pthread_create(&threadC, NULL, odd, &obj);

    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);
    pthread_join(threadC, NULL);

    // Capture the output (in a real implementation, you'd redirect output to a buffer)
    char result[100];
    snprintf(result, sizeof(result), "%d", obj.counter); // Adjust the result capture logic
    // Compare output with expected
    if (strcmp(result, expected_output) == 0) {
        printf("Test case n=%d: Pass\n", n);
    } else {
        printf("Test case n=%d: Fail\n", n);
    }

    pthread_mutex_destroy(&obj.mutex);
    pthread_cond_destroy(&obj.zeroCond);
    pthread_cond_destroy(&obj.evenOddCond);
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
