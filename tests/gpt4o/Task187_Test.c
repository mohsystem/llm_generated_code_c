#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>

typedef struct H2O {
    sem_t hydrogen_semaphore;
    sem_t oxygen_semaphore;
    pthread_barrier_t barrier;
    char* output;
    int index;
    pthread_mutex_t print_mutex; // لحماية كتابة الحروف بالتتابع
} H2O;

void init(H2O* h2o, size_t length) {
    sem_init(&h2o->hydrogen_semaphore, 0, 2);
    sem_init(&h2o->oxygen_semaphore, 0, 1);
    pthread_barrier_init(&h2o->barrier, NULL, 3);
    h2o->output = (char*)malloc(length + 1);
    h2o->index = 0;
    pthread_mutex_init(&h2o->print_mutex, NULL);
}

void destroy(H2O* h2o) {
    sem_destroy(&h2o->hydrogen_semaphore);
    sem_destroy(&h2o->oxygen_semaphore);
    pthread_barrier_destroy(&h2o->barrier);
    pthread_mutex_destroy(&h2o->print_mutex);
    free(h2o->output);
}

void* hydrogen(void* arg) {
    H2O* h2o = (H2O*)arg;
    sem_wait(&h2o->hydrogen_semaphore);
    pthread_barrier_wait(&h2o->barrier);

    pthread_mutex_lock(&h2o->print_mutex);
    h2o->output[h2o->index++] = 'H';
    pthread_mutex_unlock(&h2o->print_mutex);

    sem_post(&h2o->hydrogen_semaphore);
    return NULL;
}

void* oxygen(void* arg) {
    H2O* h2o = (H2O*)arg;
    sem_wait(&h2o->oxygen_semaphore);
    pthread_barrier_wait(&h2o->barrier);

    pthread_mutex_lock(&h2o->print_mutex);
    h2o->output[h2o->index++] = 'O';
    pthread_mutex_unlock(&h2o->print_mutex);

    sem_post(&h2o->oxygen_semaphore);
    return NULL;
}

// دالة للمقارنة مع نتائج مقبولة
int is_valid_output(const char* output, const char* valid_outputs[], int valid_count) {
    for (int i = 0; i < valid_count; i++) {
        if (strcmp(output, valid_outputs[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void run_test_case(const char* water, const char* valid_outputs[], int valid_count) {
    size_t n = strlen(water);
    H2O h2o;
    init(&h2o, n);

    pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * n);

    for (size_t i = 0; i < n; i++) {
        if (water[i] == 'H') {
            pthread_create(&threads[i], NULL, hydrogen, &h2o);
        } else if (water[i] == 'O') {
            pthread_create(&threads[i], NULL, oxygen, &h2o);
        }
    }

    for (size_t i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    h2o.output[h2o.index] = '\0'; // نهاية السلسلة

    printf("Input: %s\nOutput: %s\n", water, h2o.output);

    if (is_valid_output(h2o.output, valid_outputs, valid_count)) {
        printf("Result: PASS\n\n");
    } else {
        printf("Result: FAIL\n\n");
    }

    destroy(&h2o);
    free(threads);
}

int main() {
    // الأمثلة مع النتائج المقبولة لكل حالة
    const char* test1 = "HOH";
    const char* valid1[] = {
        "HHO",
        "HOH",
        "OHH"
    };

    const char* test2 = "OOHHHH";
    const char* valid2[] = {
        "HHOHHO",
        "HOHHHO",
        "OHHHHO",
        "HHOHOH",
        "HOHHOH",
        "OHHHOH",
        "HHOOHH",
        "HOHOHH",
        "OHHOHH"
    };

    run_test_case(test1, valid1, sizeof(valid1)/sizeof(valid1[0]));
    run_test_case(test2, valid2, sizeof(valid2)/sizeof(valid2[0]));

    return 0;
}
