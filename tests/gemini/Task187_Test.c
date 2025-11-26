#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int hCount;
    int oCount;
} H2O;

H2O* H2OCreate() {
    H2O* obj = (H2O*)malloc(sizeof(H2O));
    pthread_mutex_init(&obj->mutex, NULL);
    pthread_cond_init(&obj->cond, NULL);
    obj->hCount = 0;
    obj->oCount = 0;
    return obj;
}

void hydrogen(H2O* obj, void (*releaseHydrogen)()) {
    pthread_mutex_lock(&obj->mutex);
    while (!((obj->hCount < 2 && obj->oCount == 0) || (obj->hCount == 2 && obj->oCount == 1))) {
        pthread_cond_wait(&obj->cond, &obj->mutex);
    }
    releaseHydrogen();
    obj->hCount++;
    if (obj->hCount == 2 && obj->oCount == 1) {
        obj->hCount = 0;
        obj->oCount = 0;
    }
    pthread_cond_broadcast(&obj->cond);
    pthread_mutex_unlock(&obj->mutex);
}

void oxygen(H2O* obj, void (*releaseOxygen)()) {
    pthread_mutex_lock(&obj->mutex);
    while (!(obj->hCount == 2 && obj->oCount == 0)) {
        pthread_cond_wait(&obj->cond, &obj->mutex);
    }
    releaseOxygen();
    obj->oCount++;
    pthread_cond_broadcast(&obj->cond);
    pthread_mutex_unlock(&obj->mutex);
}

void H2ODestroy(H2O* obj) {
    pthread_mutex_destroy(&obj->mutex);
    pthread_cond_destroy(&obj->cond);
    free(obj);
}

// =======================

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

typedef struct {
    H2O* h2o;
    const char* input;
    int idx;
} ThreadArg;

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
    if (h_count != 2 * o_count) return 0;

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
    H2O* h2o = H2OCreate();
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
    H2ODestroy(h2o);
}

int main() {
    // run_test("HOH");
    run_test("OOHHHH");
    return 0;
}
