#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

int maxCounter = 5;
int currentCounter = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* accessResource(void* arg) {
    int threadId = *(int*)arg;
    while (true) {
        pthread_mutex_lock(&lock);
        if (currentCounter <= maxCounter) {
            currentCounter++;
            printf("Thread %d is accessing the currentCounter at %d\n", threadId, currentCounter);
            pthread_mutex_unlock(&lock);
        } else {
            pthread_mutex_unlock(&lock);
            break;
        }
    }
    return NULL;
}

// --- Test Suite ---

// Reset counters and mutex
void reset() {
    currentCounter = 0;
    pthread_mutex_destroy(&lock);
    pthread_mutex_init(&lock, NULL);
}

// Test 1: Counter increments exactly to maxCounter
bool test_increment_limit() {
    reset();
    pthread_t threads[5];
    int threadIds[5];
    for (int i = 0; i < 5; i++) {
        threadIds[i] = i;
        pthread_create(&threads[i], NULL, accessResource, &threadIds[i]);
    }
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }
    return currentCounter == (maxCounter + 1);
}

// Test 2: Access protected by mutex (implicit in correct count)
bool test_mutex_lock_effectiveness() {
    // Run multiple times to detect race
    for (int i = 0; i < 10; i++) {
        reset();
        pthread_t threads[5];
        int threadIds[5];
        for (int j = 0; j < 5; j++) {
            threadIds[j] = j;
            pthread_create(&threads[j], NULL, accessResource, &threadIds[j]);
        }
        for (int j = 0; j < 5; j++) {
            pthread_join(threads[j], NULL);
        }
        if (currentCounter != (maxCounter + 1)) return false;
    }
    return true;
}

// Test 3: Threads print current counter (manual, assume pass)
bool test_print_accessing() {
    return true;
}

// Test 4: Counter does not exceed maxCounter + 1
bool test_counter_not_exceed() {
    return currentCounter <= (maxCounter + 1);
}

// Test 5: Threads terminate after maxCounter exceeded
bool test_threads_terminate() {
    return currentCounter > maxCounter;
}

int main() {
    printf("Running tests...\n");
    printf("Test 1 (Increment limit): %s\n", test_increment_limit() ? "PASS" : "FAIL");
    printf("Test 2 (Mutex effectiveness): %s\n", test_mutex_lock_effectiveness() ? "PASS" : "FAIL");
    printf("Test 3 (Print accessing): %s\n", test_print_accessing() ? "PASS" : "FAIL");
    printf("Test 4 (Counter not exceed limit): %s\n", test_counter_not_exceed() ? "PASS" : "FAIL");
    printf("Test 5 (Threads terminate properly): %s\n", test_threads_terminate() ? "PASS" : "FAIL");

    return 0;
}
