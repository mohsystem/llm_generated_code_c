#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct {
  int n;
  sem_t zero_sem;
  sem_t even_sem;
  sem_t odd_sem;
  int count;
  int thread_id;
} zero_even_odd_t;

zero_even_odd_t *zero_even_odd_create(int n) {
  zero_even_odd_t *obj = malloc(sizeof(zero_even_odd_t));
  obj->n = n;
  sem_init(&obj->zero_sem, 0, 1);
  sem_init(&obj->even_sem, 0, 0);
  sem_init(&obj->odd_sem, 0, 0);
  obj->count = 1;
  obj->thread_id = 0;
  return obj;
}

void zero(zero_even_odd_t *obj, void (*print_number)(int)) {
  for (int i = 0; i < obj->n; i++) {
    sem_wait(&obj->zero_sem);
    print_number(0);
    obj->count++;
    if (obj->count % 2 == 0) {
      sem_post(&obj->odd_sem);
    } else {
      sem_post(&obj->even_sem);
    }
  }
}

void even(zero_even_odd_t *obj, void (*print_number)(int)) {
  for (int i = 2; i <= obj->n; i += 2) {
    sem_wait(&obj->even_sem);
    print_number(i);
    obj->count++;
    sem_post(&obj->zero_sem);
  }
}

void odd(zero_even_odd_t *obj, void (*print_number)(int)) {
  for (int i = 1; i <= obj->n; i += 2) {
    sem_wait(&obj->odd_sem);
    print_number(i);
    obj->count++;
    sem_post(&obj->zero_sem);
  }
}

void zero_even_odd_destroy(zero_even_odd_t *obj) {
  sem_destroy(&obj->zero_sem);
  sem_destroy(&obj->even_sem);
  sem_destroy(&obj->odd_sem);
  free(obj);
}

void print_number(int x) {
  printf("%d", x);
}

// Wrapper functions for pthread_create
void* zeroWrapper(void* arg) {
    zero_even_odd_t* obj = (zero_even_odd_t*)arg;
    zero(obj, print_number);
    return NULL;
}

void* evenWrapper(void* arg) {
    zero_even_odd_t* obj = (zero_even_odd_t*)arg;
    even(obj, print_number);
    return NULL;
}

void* oddWrapper(void* arg) {
    zero_even_odd_t* obj = (zero_even_odd_t*)arg;
    odd(obj, print_number);
    return NULL;
}

void test_case(int n, const char* expected_output) {
    zero_even_odd_t* obj = zero_even_odd_create(n);

    pthread_t t1, t2, t3;
    pthread_create(&t1, NULL, zeroWrapper, obj);
    pthread_create(&t2, NULL, evenWrapper, obj);
    pthread_create(&t3, NULL, oddWrapper, obj);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    // Create a buffer to store the result output
    char result[100];
    snprintf(result, sizeof(result), "%d", n); // Capture the result (adjust as per actual output logic)

    // Compare output with expected
    if (strcmp(result, expected_output) == 0) {
        printf("Test case n=%d: Pass\n", n);
    } else {
        printf("Test case n=%d: Fail\n", n);
    }

    zero_even_odd_destroy(obj);
}

int main() {
    // Define the test cases and expected outputs
    int test_cases[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const char* expected_outputs[] = {
        "01", "0102", "010203", "01020304", "01020305",
        "010203040506", "01020304050607", "0102030405060708",
        "010203040506070809", "010203040506070809010"
    };

    // Run the test cases
    for (int i = 0; i < 10; i++) {
        printf("Running test case %d...\n", i + 1);
        test_case(test_cases[i], expected_outputs[i]);
    }

    return 0;
}
