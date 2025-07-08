#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int n;
    pthread_mutex_t lock;
    pthread_cond_t cond;
    bool fooTurn;
    char* output;
    int index;
    pthread_mutex_t output_mutex;
} FooBar;

void* foo(void* arg) {
    FooBar* fooBar = (FooBar*)arg;
    for (int i = 0; i < fooBar->n; ++i) {
        pthread_mutex_lock(&fooBar->lock);
        while (!fooBar->fooTurn) {
            pthread_cond_wait(&fooBar->cond, &fooBar->lock);
        }

        // كتابة في الـ output بدل printf فقط
        pthread_mutex_lock(&fooBar->output_mutex);
        fooBar->output[fooBar->index++] = 'f';
        fooBar->output[fooBar->index++] = 'o';
        fooBar->output[fooBar->index++] = 'o';
        pthread_mutex_unlock(&fooBar->output_mutex);

        printf("foo"); // ممكن تحذفه اذا بدك تكتفي بالـ output فقط
        fooBar->fooTurn = false;
        pthread_cond_signal(&fooBar->cond);
        pthread_mutex_unlock(&fooBar->lock);
    }
    return NULL;
}

void* bar(void* arg) {
    FooBar* fooBar = (FooBar*)arg;
    for (int i = 0; i < fooBar->n; ++i) {
        pthread_mutex_lock(&fooBar->lock);
        while (fooBar->fooTurn) {
            pthread_cond_wait(&fooBar->cond, &fooBar->lock);
        }

        pthread_mutex_lock(&fooBar->output_mutex);
        fooBar->output[fooBar->index++] = 'b';
        fooBar->output[fooBar->index++] = 'a';
        fooBar->output[fooBar->index++] = 'r';
        pthread_mutex_unlock(&fooBar->output_mutex);

        printf("bar"); // ممكن تحذفه اذا بدك تكتفي بالـ output فقط
        fooBar->fooTurn = true;
        pthread_cond_signal(&fooBar->cond);
        pthread_mutex_unlock(&fooBar->lock);
    }
    return NULL;
}

int check_output(FooBar* fooBar) {
    fooBar->output[fooBar->index] = '\0';
    int expected_len = fooBar->n * 6;
    if ((int)strlen(fooBar->output) != expected_len) return 0;
    for (int i = 0; i < fooBar->n; i++) {
        if (strncmp(&fooBar->output[i * 6], "foobar", 6) != 0) {
            return 0;
        }
    }
    return 1;
}

void run_test(int n) {
    FooBar fooBar;
    fooBar.n = n;
    pthread_mutex_init(&fooBar.lock, NULL);
    pthread_cond_init(&fooBar.cond, NULL);
    pthread_mutex_init(&fooBar.output_mutex, NULL);
    fooBar.fooTurn = true;
    fooBar.output = (char*)malloc(n * 6 + 1);
    fooBar.index = 0;

    pthread_t threadA, threadB;
    pthread_create(&threadA, NULL, foo, &fooBar);
    pthread_create(&threadB, NULL, bar, &fooBar);
    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);

    printf("\nInput: n = %d\nOutput buffer: %s\n", n, fooBar.output);
    if (check_output(&fooBar)) {
        printf("Result: PASS\n\n");
    } else {
        printf("Result: FAIL\n\n");
    }

    free(fooBar.output);
    pthread_mutex_destroy(&fooBar.lock);
    pthread_mutex_destroy(&fooBar.output_mutex);
    pthread_cond_destroy(&fooBar.cond);
}

int main() {
    run_test(1); // مثال 1
    run_test(2); // مثال 2

    return 0;
}
