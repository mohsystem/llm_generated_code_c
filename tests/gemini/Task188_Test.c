#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

pthread_mutex_t mutex;
pthread_cond_t cond;
int count = 0;
int n;

typedef struct {
    char* output;
    int index;
    pthread_mutex_t output_mutex;
} OutputBuffer;

OutputBuffer outBuf;

void* foo(void* arg) {
    for (int i = 0; i < n; i++) {
        pthread_mutex_lock(&mutex);
        while (count % 2 != 0) {
            pthread_cond_wait(&cond, &mutex);
        }

        pthread_mutex_lock(&outBuf.output_mutex);
        outBuf.output[outBuf.index++] = 'f';
        outBuf.output[outBuf.index++] = 'o';
        outBuf.output[outBuf.index++] = 'o';
        pthread_mutex_unlock(&outBuf.output_mutex);

        printf("foo"); // Optional: can comment out if you want only output buffer check
        count++;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* bar(void* arg) {
    for (int i = 0; i < n; i++) {
        pthread_mutex_lock(&mutex);
        while (count % 2 == 0) {
            pthread_cond_wait(&cond, &mutex);
        }

        pthread_mutex_lock(&outBuf.output_mutex);
        outBuf.output[outBuf.index++] = 'b';
        outBuf.output[outBuf.index++] = 'a';
        outBuf.output[outBuf.index++] = 'r';
        pthread_mutex_unlock(&outBuf.output_mutex);

        printf("bar"); // Optional: can comment out if you want only output buffer check
        count++;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int check_output() {
    outBuf.output[outBuf.index] = '\0';
    int expected_len = n * 6;
    if (strlen(outBuf.output) != expected_len) return 0;

    for (int i = 0; i < n; i++) {
        if (strncmp(&outBuf.output[i * 6], "foobar", 6) != 0) {
            return 0;
        }
    }
    return 1;
}

void run_test(int test_n) {
    n = test_n;
    count = 0;

    outBuf.output = (char*)malloc(6 * n + 1);
    outBuf.index = 0;
    pthread_mutex_init(&outBuf.output_mutex, NULL);

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_t t1, t2;
    pthread_create(&t1, NULL, foo, NULL);
    pthread_create(&t2, NULL, bar, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("\nInput: n = %d\nOutput buffer: %s\n", n, outBuf.output);
    if (check_output()) {
        printf("Result: PASS\n\n");
    } else {
        printf("Result: FAIL\n\n");
    }

    free(outBuf.output);
    pthread_mutex_destroy(&outBuf.output_mutex);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}

int main() {
    run_test(1); // مثال 1
    run_test(2); // مثال 2

    return 0;
}
