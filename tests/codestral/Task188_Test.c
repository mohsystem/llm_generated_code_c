#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef void (*printFunc)();

typedef struct {
    int n;
    sem_t fooSemaphore;
    sem_t barSemaphore;
    char* output;
    int index;
    pthread_mutex_t mutex;
} FooBar;

FooBar* FooBarCreate(int n) {
    FooBar* obj = (FooBar*)malloc(sizeof(FooBar));
    obj->n = n;
    sem_init(&obj->fooSemaphore, 0, 1);
    sem_init(&obj->barSemaphore, 0, 0);
    obj->output = (char*)malloc(7 * n + 1); // 6 chars per "foobar" + '\0'
    obj->index = 0;
    pthread_mutex_init(&obj->mutex, NULL);
    return obj;
}

void FooBarDestroy(FooBar* obj) {
    sem_destroy(&obj->fooSemaphore);
    sem_destroy(&obj->barSemaphore);
    pthread_mutex_destroy(&obj->mutex);
    free(obj->output);
    free(obj);
}

void printFooFunc(FooBar* obj) {
    pthread_mutex_lock(&obj->mutex);
    obj->output[obj->index++] = 'f';
    obj->output[obj->index++] = 'o';
    obj->output[obj->index++] = 'o';
    pthread_mutex_unlock(&obj->mutex);
}

void printBarFunc(FooBar* obj) {
    pthread_mutex_lock(&obj->mutex);
    obj->output[obj->index++] = 'b';
    obj->output[obj->index++] = 'a';
    obj->output[obj->index++] = 'r';
    pthread_mutex_unlock(&obj->mutex);
}

void* fooThread(void* arg) {
    FooBar* obj = (FooBar*)arg;
    for (int i = 0; i < obj->n; i++) {
        sem_wait(&obj->fooSemaphore);
        printFooFunc(obj);
        sem_post(&obj->barSemaphore);
    }
    return NULL;
}

void* barThread(void* arg) {
    FooBar* obj = (FooBar*)arg;
    for (int i = 0; i < obj->n; i++) {
        sem_wait(&obj->barSemaphore);
        printBarFunc(obj);
        sem_post(&obj->fooSemaphore);
    }
    return NULL;
}

int check_output(FooBar* obj) {
    obj->output[obj->index] = '\0';
    int expected_len = obj->n * 6;
    if (strlen(obj->output) != expected_len) return 0;

    for (int i = 0; i < obj->n; i++) {
        if (strncmp(&obj->output[i * 6], "foobar", 6) != 0) {
            return 0;
        }
    }
    return 1;
}

void run_test(int n) {
    FooBar* fooBar = FooBarCreate(n);

    pthread_t t1, t2;
    pthread_create(&t1, NULL, fooThread, fooBar);
    pthread_create(&t2, NULL, barThread, fooBar);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Input: n = %d\nOutput: %s\n", n, fooBar->output);
    if (check_output(fooBar)) {
        printf("Result: PASS\n\n");
    } else {
        printf("Result: FAIL\n\n");
    }

    FooBarDestroy(fooBar);
}

int main() {
    run_test(1); // Example 1: expected "foobar"
    run_test(2); // Example 2: expected "foobarfoobar"
    return 0;
}
