#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>   // for mkstemp
#include <string.h>   // for strlen

int binary_search(int arr[], int left, int right, int target) {
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

int check_file_nonempty(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    return (size > 0);
}

int main() {
    int arr[] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    int n = sizeof(arr) / sizeof(arr[0]);
    int target = 13;

    clock_t start_time = clock();
    int result = binary_search(arr, 0, n - 1, target);
    clock_t end_time = clock();

    double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    char temp_filename[] = "execution_time_XXXXXX";
    int fd = mkstemp(temp_filename);
    if (fd != -1) {
        FILE* temp_file = fdopen(fd, "w");
        if (temp_file != NULL) {
            fprintf(temp_file, "Execution time: %.6f seconds", execution_time);
            fclose(temp_file);
            printf("Execution time written to: %s\n", temp_filename);

            // تحقق من نجاح كتابة الملف (بحجم أكبر من صفر)
            if (check_file_nonempty(temp_filename)) {
                printf("Test case 1: pass\n");
            } else {
                printf("Test case 1: fail (empty temp file)\n");
            }
        } else {
            printf("Test case 1: fail (could not open temp file stream)\n");
            close(fd);
        }
    } else {
        printf("Test case 1: fail (mkstemp failed)\n");
    }

    // تحقق من نتيجة البحث (مطابقة البايثون: 6)
    if (result == 6) {
        printf("Binary search result correct: pass\n");
    } else {
        printf("Binary search result incorrect: fail\n");
    }

    printf("Result: %d\n", result);

    return 0;
}
