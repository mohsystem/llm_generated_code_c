#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

typedef void (*printNumber_t)(int);

typedef struct {
    int n;
    sem_t zeroSemaphore;
    sem_t evenSemaphore;
    sem_t oddSemaphore;
    char* output;
    int index;
    pthread_mutex_t mutex;
} ZeroEvenOdd;

ZeroEvenOdd* zeroEvenOddCreate(int n) {
    ZeroEvenOdd* obj = (ZeroEvenOdd*)malloc(sizeof(ZeroEvenOdd));
    obj->n = n;
    sem_init(&obj->zeroSemaphore, 0, 1);
    sem_init(&obj->evenSemaphore, 0, 0);
    sem_init(&obj->oddSemaphore, 0, 0);
    obj->output = (char*)malloc(3 * n + 1); // تقريبا 1 digit per print + 0’s
    obj->index = 0;
    pthread_mutex_init(&obj->mutex, NULL);
    return obj;
}

void zeroEvenOddFree(ZeroEvenOdd* obj) {
    sem_destroy(&obj->zeroSemaphore);
    sem_destroy(&obj->evenSemaphore);
    sem_destroy(&obj->oddSemaphore);
    pthread_mutex_destroy(&obj->mutex);
    free(obj->output);
    free(obj);
}

void printNumber(ZeroEvenOdd* obj, int x) {
    pthread_mutex_lock(&obj->mutex);
    // لطباعة الأرقام بصيغة chars
    if (x == 0) {
        obj->output[obj->index++] = '0';
    } else {
        // نطبع الرقم ك string، لأن x ممكن يكون >9
        char buf[12];
        int len = snprintf(buf, sizeof(buf), "%d", x);
        for (int i = 0; i < len; i++) {
            obj->output[obj->index++] = buf[i];
        }
    }
    pthread_mutex_unlock(&obj->mutex);
}

void* zeroEvenOddZero(void* arg) {
    ZeroEvenOdd* obj = (ZeroEvenOdd*)arg;
    for (int i = 0; i < obj->n; i++) {
        sem_wait(&obj->zeroSemaphore);
        printNumber(obj, 0);
        if ((i + 1) % 2 == 1) { // odd index (1-based)
            sem_post(&obj->oddSemaphore);
        } else {
            sem_post(&obj->evenSemaphore);
        }
    }
    return NULL;
}

void* zeroEvenOddEven(void* arg) {
    ZeroEvenOdd* obj = (ZeroEvenOdd*)arg;
    for (int i = 2; i <= obj->n; i += 2) {
        sem_wait(&obj->evenSemaphore);
        printNumber(obj, i);
        sem_post(&obj->zeroSemaphore);
    }
    return NULL;
}

void* zeroEvenOddOdd(void* arg) {
    ZeroEvenOdd* obj = (ZeroEvenOdd*)arg;
    for (int i = 1; i <= obj->n; i += 2) {
        sem_wait(&obj->oddSemaphore);
        printNumber(obj, i);
        sem_post(&obj->zeroSemaphore);
    }
    return NULL;
}

int check_output(ZeroEvenOdd* obj) {
    obj->output[obj->index] = '\0';
    // ننتج السلسلة المتوقعة
    char* expected = (char*)malloc(3 * obj->n + 1);
    int pos = 0;
    for (int i = 1; i <= obj->n; i++) {
        expected[pos++] = '0';
        pos += sprintf(&expected[pos], "%d", i);
    }
    expected[pos] = '\0';

    int result = strcmp(obj->output, expected) == 0;
    free(expected);
    return result;
}

void run_test(int n) {
    ZeroEvenOdd* obj = zeroEvenOddCreate(n);

    pthread_t t_zero, t_even, t_odd;
    pthread_create(&t_zero, NULL, zeroEvenOddZero, obj);
    pthread_create(&t_even, NULL, zeroEvenOddEven, obj);
    pthread_create(&t_odd, NULL, zeroEvenOddOdd, obj);

    pthread_join(t_zero, NULL);
    pthread_join(t_even, NULL);
    pthread_join(t_odd, NULL);

    printf("Input: n = %d\nOutput: %s\n", n, obj->output);
    if (check_output(obj)) {
        printf("Result: PASS\n\n");
    } else {
        printf("Result: FAIL\n\n");
    }

    zeroEvenOddFree(obj);
}

int main() {
    run_test(2);  // Example 1: output "0102"
    run_test(5);  // Example 2: output "0102030405"
    return 0;
}
