#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct {
    int n;
    int count;
    sem_t z;
    sem_t e;
    sem_t o;
} ZeroEvenOdd;

ZeroEvenOdd* zeroEvenOddCreate(int n) {
    ZeroEvenOdd* obj = malloc(sizeof(ZeroEvenOdd));
    obj->n = n;
    obj->count = 1;
    sem_init(&obj->z, 0, 1);
    sem_init(&obj->e, 0, 0);
    sem_init(&obj->o, 0, 0);
    return obj;
}

void zeroEvenOddZero(ZeroEvenOdd* obj, void (*printNumber)(int)) {
    for (int i = 0; i < obj->n; i++) {
        sem_wait(&obj->z);
        printNumber(0);
        if (obj->count % 2 == 1) {
            sem_post(&obj->o);
        } else {
            sem_post(&obj->e);
        }
    }
}

void zeroEvenOddEven(ZeroEvenOdd* obj, void (*printNumber)(int)) {
    for (int i = 2; i <= obj->n; i += 2) {
        sem_wait(&obj->e);
        printNumber(i);
        obj->count++;
        sem_post(&obj->z);
    }
}

void zeroEvenOddOdd(ZeroEvenOdd* obj, void (*printNumber)(int)) {
    for (int i = 1; i <= obj->n; i += 2) {
        sem_wait(&obj->o);
        printNumber(i);
        obj->count++;
        sem_post(&obj->z);
    }
}

void zeroEvenOddFree(ZeroEvenOdd* obj) {
    sem_destroy(&obj->z);
    sem_destroy(&obj->e);
    sem_destroy(&obj->o);
    free(obj);
}

// printNumber function as per requirement
void printNumber(int x) {
    printf("%d", x);
    fflush(stdout);
}

typedef struct {
    ZeroEvenOdd* obj;
    void (*func)(ZeroEvenOdd*, void (*)(int));
} ThreadArg;

void* threadFunc(void* arg) {
    ThreadArg* t = (ThreadArg*)arg;
    t->func(t->obj, printNumber);
    return NULL;
}

int main() {
    int n = 5;  // Change to test other values

    ZeroEvenOdd* zeo = zeroEvenOddCreate(n);

    pthread_t tid_zero, tid_even, tid_odd;

    ThreadArg args[3] = {
        {zeo, zeroEvenOddZero},
        {zeo, zeroEvenOddEven},
        {zeo, zeroEvenOddOdd}
    };

    pthread_create(&tid_zero, NULL, threadFunc, &args[0]);
    pthread_create(&tid_even, NULL, threadFunc, &args[1]);
    pthread_create(&tid_odd, NULL, threadFunc, &args[2]);

    pthread_join(tid_zero, NULL);
    pthread_join(tid_even, NULL);
    pthread_join(tid_odd, NULL);

    printf("\n");

    zeroEvenOddFree(zeo);

    return 0;
}
