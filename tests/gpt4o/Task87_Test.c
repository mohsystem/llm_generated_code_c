#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int binarySearch(int arr[], int l, int r, int x) {
    while (l <= r) {
        int mid = l + (r - l) / 2;
        if (arr[mid] == x)
            return mid;
        if (arr[mid] < x)
            l = mid + 1;
        else
            r = mid - 1;
    }
    return -1;
}

// تحقق من وجود الملف وحجمه > 0
int check_file_nonempty(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    return (size > 0);
}

int main(void) {
    int arr[] = {2, 3, 4, 10, 40};
    int x = 10;
    int n = sizeof(arr) / sizeof(arr[0]);

    clock_t start = clock();
    int result = binarySearch(arr, 0, n - 1, x);
    clock_t end = clock();
    double execution_time = ((double) (end - start)) / CLOCKS_PER_SEC;

    const char* filename = "binary_search_execution_time.txt";
    FILE *tempFile = fopen(filename, "w");
    if (tempFile != NULL) {
        fprintf(tempFile, "Execution time: %f seconds", execution_time);
        fclose(tempFile);
    } else {
        printf("Test case 1: fail (cannot open file)\n");
        return 1;
    }

    // تحقق من نتيجة البحث
    if (result == 3) { // العنصر 10 في المؤشر 3
        printf("Binary search result: pass\n");
    } else {
        printf("Binary search result: fail\n");
    }

    // تحقق من ملف التنفيذ
    if (check_file_nonempty(filename)) {
        printf("Execution time file write: pass\n");
    } else {
        printf("Execution time file write: fail\n");
    }

    return 0;
}
