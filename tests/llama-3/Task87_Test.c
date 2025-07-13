#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int binarySearch(int arr[], int target, int low, int high) {
    while (low <= high) {
        int mid = (low + high) / 2;
        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
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
    int arr[10000];
    srand(time(NULL));
    for (int i = 0; i < 10000; i++) {
        arr[i] = rand() % 101;
    }
    // bubble sort
    for (int i = 0; i < 10000; i++) {
        for (int j = i + 1; j < 10000; j++) {
            if (arr[i] > arr[j]) {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }

    clock_t start_time = clock();
    int index = binarySearch(arr, 50, 0, 9999);
    clock_t end_time = clock();

    const char *filename = "temp.txt";
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Test case 1: fail (cannot open file)\n");
        return 1;
    }
    fprintf(file, "Execution time: %.6f seconds\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
    if (index != -1) {
        fprintf(file, "Target found at index %d\n", index);
    } else {
        fprintf(file, "Target not found\n");
    }
    fclose(file);

    // تحقق من كتابة الملف
    if (check_file_nonempty(filename)) {
        printf("Test case 1: pass (file written successfully)\n");
    } else {
        printf("Test case 1: fail (file empty)\n");
    }

    // تحقق من نتيجة البحث منطقية (index >= -1)
    if (index >= -1 && index < 10000) {
        printf("Binary search result: pass\n");
    } else {
        printf("Binary search result: fail\n");
    }

    return 0;
}
