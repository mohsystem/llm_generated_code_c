#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int n;
    sem_t foo_sem;
    sem_t bar_sem;
    char* output;
    int index;
    pthread_mutex_t mutex;  // لحماية الكتابة للـ output
} FooBar;

void FooBar_init(FooBar* obj, int n) {
    obj->n = n;
    sem_init(&obj->foo_sem, 0, 1);
    sem_init(&obj->bar_sem, 0, 0);
    obj->output = (char*)malloc(7 * n + 1); // كل "foobar" طوله 6 + '\0'
    obj->index = 0;
    pthread_mutex_init(&obj->mutex, NULL);
}

void FooBar_free(FooBar* obj) {
    sem_destroy(&obj->foo_sem);
    sem_destroy(&obj->bar_sem);
    pthread_mutex_destroy(&obj->mutex);
    free(obj->output);
}

void* foo(void* arg) {
    FooBar* obj = (FooBar*)arg;
    for (int i = 0; i < obj->n; i++) {
        sem_wait(&obj->foo_sem);
        pthread_mutex_lock(&obj->mutex);
        obj->output[obj->index++] = 'f';
        obj->output[obj->index++] = 'o';
        obj->output[obj->index++] = 'o';
        pthread_mutex_unlock(&obj->mutex);
        sem_post(&obj->bar_sem);
    }
    return NULL;
}

void* bar(void* arg) {
    FooBar* obj = (FooBar*)arg;
    for (int i = 0; i < obj->n; i++) {
        sem_wait(&obj->bar_sem);
        pthread_mutex_lock(&obj->mutex);
        obj->output[obj->index++] = 'b';
        obj->output[obj->index++] = 'a';
        obj->output[obj->index++] = 'r';
        pthread_mutex_unlock(&obj->mutex);
        sem_post(&obj->foo_sem);
    }
    return NULL;
}

int check_output(FooBar* obj) {
    obj->output[obj->index] = '\0';
    // النتيجة المتوقعة هي "foobar" مكررة n مرات
    int expected_len = obj->n * 6;
    if (strlen(obj->output) != expected_len) return 0;

    for (int i = 0; i < obj->n; i++) {
        if (strncmp(&obj->output[i*6], "foobar", 6) != 0) {
            return 0;
        }
    }
    return 1;
}

void run_test(int n) {
    FooBar fooBar;
    FooBar_init(&fooBar, n);

    pthread_t t1, t2;
    pthread_create(&t1, NULL, foo, &fooBar);
    pthread_create(&t2, NULL, bar, &fooBar);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Output: %s\n", fooBar.output);
    if (check_output(&fooBar)) {
        printf("Result: PASS\n\n");
    } else {
        printf("Result: FAIL\n\n");
    }

    FooBar_free(&fooBar);
}

int main() {
    run_test(1); // مثال 1
    run_test(2); // مثال 2
    // تقدر تضيف حالات أخرى بنفس الطريقة

    return 0;
}
