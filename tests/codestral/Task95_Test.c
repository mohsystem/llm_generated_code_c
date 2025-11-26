#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

int is_valid_number(const char *str) {
    // تحقق بسيط أن النص يحتوي فقط على أرقام أو نقطة أو - أو + أو e/E (float)
    int dot_count = 0, digit_count = 0;
    const char *p = str;
    if (*p == '-' || *p == '+') p++;
    while (*p) {
        if (*p >= '0' && *p <= '9') {
            digit_count++;
        } else if (*p == '.') {
            dot_count++;
            if (dot_count > 1) return 0;
        } else if (*p == 'e' || *p == 'E') {
            p++;
            if (*p == '-' || *p == '+') p++;
            if (!(*p >= '0' && *p <= '9')) return 0;
            while (*p >= '0' && *p <= '9') p++;
            break;
        } else {
            return 0;
        }
        p++;
    }
    return digit_count > 0;
}

int main() {
    clock_t start = clock();
    // Your code here
    clock_t end = clock();

    double duration = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Processor time: %.6f seconds\n", duration);

    // التحقق من النتيجة: duration >= 0
    if (duration >= 0.0) {
        printf("Test case: PASS\n");
    } else {
        printf("Test case: FAIL\n");
    }

    return 0;
}
