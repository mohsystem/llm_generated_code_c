#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

typedef struct {
    sem_t hydrogen_sem;
    sem_t oxygen_sem;
    pthread_barrier_t barrier;
} H2O;

void init(H2O* h2o) {
    sem_init(&h2o->hydrogen_sem, 0, 2);
    sem_init(&h2o->oxygen_sem, 0, 1);
    pthread_barrier_init(&h2o->barrier, NULL, 3);
}

void releaseHydrogen() {
    printf("H");
    fflush(stdout);
}

void releaseOxygen() {
    printf("O");
    fflush(stdout);
}

void* hydrogen_thread(void* arg) {
    H2O* h2o = (H2O*)arg;
    sem_wait(&h2o->hydrogen_sem);
    pthread_barrier_wait(&h2o->barrier);
    releaseHydrogen();
    sem_post(&h2o->hydrogen_sem);
    return NULL;
}

void* oxygen_thread(void* arg) {
    H2O* h2o = (H2O*)arg;
    sem_wait(&h2o->oxygen_sem);
    pthread_barrier_wait(&h2o->barrier);
    releaseOxygen();
    sem_post(&h2o->oxygen_sem);
    return NULL;
}

// Function to run test on given water string like "HOH"
void run_test(const char* water) {
    H2O h2o;
    init(&h2o);

    int len = 0;
    while (water[len] != '\0') len++;

    pthread_t* threads = malloc(len * sizeof(pthread_t));
    int thread_idx = 0;

    // Create threads based on input sequence
    for (int i = 0; i < len; i++) {
        if (water[i] == 'H') {
            pthread_create(&threads[thread_idx++], NULL, hydrogen_thread, &h2o);
        } else if (water[i] == 'O') {
            pthread_create(&threads[thread_idx++], NULL, oxygen_thread, &h2o);
        }
    }

    // Join threads
    for (int i = 0; i < thread_idx; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("\n");

    // Destroy semaphores and barrier
    sem_destroy(&h2o.hydrogen_sem);
    sem_destroy(&h2o.oxygen_sem);
    pthread_barrier_destroy(&h2o.barrier);

    free(threads);
}

int main() {
    printf("Test 1 (Input: \"HOH\") Output: ");
    run_test("HOH");   // Valid outputs: HHO, HOH, OHH

    printf("Test 2 (Input: \"OOHHHH\") Output: ");
    run_test("OOHHHH");  // Outputs like HHOHHO, HOHHHO, etc.

    return 0;
}
