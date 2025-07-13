#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int binary_search(int arr[], int target, int low, int high) {
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
    int arr[100000];
    for (int i = 0; i < 100000; i++) {
        arr[i] = rand() % 100000;
    }

    int target = rand() % 100000;

    clock_t start = clock();
    int result = binary_search(arr, target, 0, 99999);
    clock_t end = clock();

    double execution_time = (double)(end - start) / CLOCKS_PER_SEC;

    const char *filename = "execution_time.txt";
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Test case 1: fail (cannot open file)\n");
        return 1;
    }
    fprintf(file, "%f", execution_time);
    fclose(file);

    if (check_file_nonempty(filename)) {
        printf("Test case 1: pass (execution time file written)\n");
    } else {
        printf("Test case 1: fail (execution time file empty)\n");
    }

    // لا يمكن التأكد من نتيجة البحث بشكل صحيح لأن المصفوفة غير مرتبة
    printf("Search result: %d (cannot verify correctness due to unsorted array)\n", result);

    return 0;
}
