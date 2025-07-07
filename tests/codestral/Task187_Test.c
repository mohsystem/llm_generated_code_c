#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

typedef struct {
    sem_t hSem;
    sem_t oSem;
} H2O;

H2O* H2O_create() {
    H2O* obj = (H2O*)malloc(sizeof(H2O));
    sem_init(&obj->hSem, 0, 2);
    sem_init(&obj->oSem, 0, 0);
    return obj;
}

void hydrogen(H2O* obj, void (*releaseHydrogen)()) {
    sem_wait(&obj->hSem);
    releaseHydrogen();
    sem_post(&obj->oSem);
}

void oxygen(H2O* obj, void (*releaseOxygen)()) {
    sem_wait(&obj->oSem);
    sem_wait(&obj->oSem);
    releaseOxygen();
    sem_post(&obj->hSem);
    sem_post(&obj->hSem);
}

// Synchronization wrapper for testing
typedef struct {
    H2O* h2o;
    const char* input;
    int idx;
} ThreadArg;

pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;
char output_buffer[100];
int output_pos = 0;

void releaseHydrogen() {
    pthread_mutex_lock(&print_mutex);
    output_buffer[output_pos++] = 'H';
    pthread_mutex_unlock(&print_mutex);
}

void releaseOxygen() {
    pthread_mutex_lock(&print_mutex);
    output_buffer[output_pos++] = 'O';
    pthread_mutex_unlock(&print_mutex);
}

void* thread_func(void* arg) {
    ThreadArg* t = (ThreadArg*)arg;
    if (t->input[t->idx] == 'H') {
        hydrogen(t->h2o, releaseHydrogen);
    } else if (t->input[t->idx] == 'O') {
        oxygen(t->h2o, releaseOxygen);
    }
    return NULL;
}

int check_valid_water(const char* s) {
    int h_count = 0, o_count = 0;
    int len = (int)strlen(s);
    for (int i = 0; i < len; i++) {
        if (s[i] == 'H') h_count++;
        else if (s[i] == 'O') o_count++;
    }
    if (h_count != 2 * o_count) return 0; // invalid counts

    // Check every group of 3 chars contains 2H and 1O
    for (int i = 0; i < len; i += 3) {
        int h = 0, o = 0;
        for (int j = i; j < i + 3 && j < len; j++) {
            if (s[j] == 'H') h++;
            else if (s[j] == 'O') o++;
        }
        if (!(h == 2 && o == 1)) return 0;
    }
    return 1;
}

void run_test(const char* input) {
    int n = (int)strlen(input);
    output_pos = 0;
    H2O* h2o = H2O_create();
    pthread_t* threads = malloc(n * sizeof(pthread_t));
    ThreadArg* args = malloc(n * sizeof(ThreadArg));

    for (int i = 0; i < n; i++) {
        args[i].h2o = h2o;
        args[i].input = input;
        args[i].idx = i;
        pthread_create(&threads[i], NULL, thread_func, &args[i]);
    }

    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }
    output_buffer[output_pos] = '\0';

    int valid = check_valid_water(output_buffer);
    printf("Input: %s\nOutput: %s\nResult: %s\n\n", input, output_buffer, valid ? "pass" : "fail");

    free(threads);
    free(args);
    free(h2o);
}

int main() {
    run_test("HOH");      // test case 1
    run_test("OOHHHH");   // test case 2
    return 0;
}
