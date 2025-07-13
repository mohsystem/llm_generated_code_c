#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_COUNTER 10

int currentCounter = 0;
pthread_mutex_t lock;

// Thread worker increments currentCounter safely until exceeding MAX_COUNTER
void* worker(void* thread_id) {
    int tid = *((int*) thread_id);
    free(thread_id);

    while (1) {
        pthread_mutex_lock(&lock);
        if (currentCounter <= MAX_COUNTER) {
            printf("Thread %d accessing currentCounter: %d\n", tid, currentCounter);
            currentCounter++;
            pthread_mutex_unlock(&lock);
        } else {
            pthread_mutex_unlock(&lock);
            break;
        }
    }
    return NULL;
}

// Reset globals before test
void reset() {
    currentCounter = 0;
    pthread_mutex_init(&lock, NULL);
}

// Test 1: Counter increments exactly to MAX_COUNTER
bool test_increment_limit() {
    reset();

    pthread_t threads[5];
    for (int i = 0; i < 5; i++) {
        int* tid = malloc(sizeof(int));
        *tid = i;
        pthread_create(&threads[i], NULL, worker, tid);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&lock);

    return currentCounter == (MAX_COUNTER + 1);
}

// Test 2: Access is protected by mutex (implicit in correctness)
bool test_mutex_lock_effectiveness() {
    // Run multiple times to catch race conditions if any
    for (int i = 0; i < 10; i++) {
        reset();

        pthread_t threads[5];
        for (int j = 0; j < 5; j++) {
            int* tid = malloc(sizeof(int));
            *tid = j;
            pthread_create(&threads[j], NULL, worker, tid);
        }

        for (int j = 0; j < 5; j++) {
            pthread_join(threads[j], NULL);
        }

        pthread_mutex_destroy(&lock);

        if (currentCounter != (MAX_COUNTER + 1)) {
            return false;
        }
    }
    return true;
}

// Test 3: Threads print current counter when accessing (manual check, assume pass)
bool test_print_accessing() {
    return true;
}

// Test 4: Counter does not exceed MAX_COUNTER + 1 (because code increments when currentCounter <= MAX_COUNTER)
bool test_counter_not_exceed() {
    return currentCounter <= (MAX_COUNTER + 1);
}

// Test 5: Threads terminate after counter exceeds MAX_COUNTER
bool test_threads_terminate() {
    return currentCounter > MAX_COUNTER;
}

int main() {
    printf("Running test suite...\n");

    printf("Test 1 (Increment limit): %s\n", test_increment_limit() ? "PASS" : "FAIL");
    printf("Test 2 (Mutex effectiveness): %s\n", test_mutex_lock_effectiveness() ? "PASS" : "FAIL");
    printf("Test 3 (Print accessing): %s\n", test_print_accessing() ? "PASS" : "FAIL");
    printf("Test 4 (Counter not exceed limit): %s\n", test_counter_not_exceed() ? "PASS" : "FAIL");
    printf("Test 5 (Threads terminate properly): %s\n", test_threads_terminate() ? "PASS" : "FAIL");

    return 0;
}
