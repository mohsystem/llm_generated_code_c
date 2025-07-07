#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// تعريف هيكل ZeroEvenOdd
typedef struct {
    int n; // العدد المطلوب طباعته
    int state; // حالة التحكم بين العمليات (0 للصفر, 1 للفردي, 2 للزوجي)
    pthread_mutex_t lock; // قفل لضمان الوصول المتسلسل للمتغيرات المشتركة
    pthread_cond_t condition; // الشرط الذي ينظم التزامن بين الخيوط
} ZeroEvenOdd;

// دالة الطباعة
void printNumber(int x) {
    printf("%d", x);
}

// دالة الخيط الذي يطبع الأصفار
void* zero(void* arg) {
    ZeroEvenOdd* obj = (ZeroEvenOdd*)arg;
    for (int i = 1; i <= obj->n; i++) {
        pthread_mutex_lock(&obj->lock);
        while (obj->state != 0) {
            pthread_cond_wait(&obj->condition, &obj->lock); // الانتظار حتى يكون دور الصفر
        }
        printNumber(0); // طباعة صفر
        obj->state = (i % 2 == 1) ? 1 : 2; // تغيير الحالة بين فردي وزوجي
        pthread_cond_broadcast(&obj->condition); // إرسال إشعار لجميع الخيوط
        pthread_mutex_unlock(&obj->lock);
    }
    return NULL;
}

// دالة الخيط الذي يطبع الأرقام الزوجية
void* even(void* arg) {
    ZeroEvenOdd* obj = (ZeroEvenOdd*)arg;
    for (int i = 2; i <= obj->n; i += 2) {
        pthread_mutex_lock(&obj->lock);
        while (obj->state != 2) {
            pthread_cond_wait(&obj->condition, &obj->lock); // الانتظار حتى يكون دور الأرقام الزوجية
        }
        printNumber(i); // طباعة الرقم الزوجي
        obj->state = 0; // إعادة الحالة للصفر بعد طباعة رقم زوجي
        pthread_cond_broadcast(&obj->condition); // إرسال إشعار لجميع الخيوط
        pthread_mutex_unlock(&obj->lock);
    }
    return NULL;
}

// دالة الخيط الذي يطبع الأرقام الفردية
void* odd(void* arg) {
    ZeroEvenOdd* obj = (ZeroEvenOdd*)arg;
    for (int i = 1; i <= obj->n; i += 2) {
        pthread_mutex_lock(&obj->lock);
        while (obj->state != 1) {
            pthread_cond_wait(&obj->condition, &obj->lock); // الانتظار حتى يكون دور الأرقام الفردية
        }
        printNumber(i); // طباعة الرقم الفردي
        obj->state = 0; // إعادة الحالة للصفر بعد طباعة رقم فردي
        pthread_cond_broadcast(&obj->condition); // إرسال إشعار لجميع الخيوط
        pthread_mutex_unlock(&obj->lock);
    }
    return NULL;
}

// الدالة الرئيسية لإنشاء الخيوط وبدء التنفيذ
int main() {
    int n = 5; // تحديد عدد الأرقام المطلوبة
    ZeroEvenOdd zeroEvenOdd = {n, 0, PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER}; // تهيئة هيكل ZeroEvenOdd

    // إنشاء الخيوط
    pthread_t threadA, threadB, threadC;
    pthread_create(&threadA, NULL, zero, &zeroEvenOdd); // خيط الأصفار
    pthread_create(&threadB, NULL, even, &zeroEvenOdd); // خيط الأرقام الزوجية
    pthread_create(&threadC, NULL, odd, &zeroEvenOdd); // خيط الأرقام الفردية

    // انتظار انتهاء الخيوط
    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);
    pthread_join(threadC, NULL);

    return 0;
}
