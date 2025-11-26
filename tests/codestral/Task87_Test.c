#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int binarySearch(int arr[], int l, int r, int x) {
    if (r >= l) {
        int mid = l + (r - l) / 2;
        if (arr[mid] == x)
            return mid;
        if (arr[mid] > x)
            return binarySearch(arr, l, mid - 1, x);
        return binarySearch(arr, mid + 1, r, x);
    }
    return -1;
}

// تحقق من وجود الملف وحجمه أكبر من صفر
int check_file_nonempty(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    return (size > 0);
}

int main() {
    int arr[] = {1, 3, 4, 10, 12, 90};
    int n = sizeof(arr) / sizeof(arr[0]);
    int x = 10;
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    int result = binarySearch(arr, 0, n - 1, x);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    const char* filename = "execution_time.txt";
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        printf("Error opening file!\n");
        printf("Test case 1: fail (file open error)\n");
        return 1;
    }
    fprintf(f, "%f", cpu_time_used);
    fclose(f);

    // تحقق من نتيجة البحث
    if (result == 3) {  // حسب المصفوفة، العنصر 10 في المؤشر 3
        printf("Binary search result: pass\n");
    } else {
        printf("Binary search result: fail\n");
    }

    // تحقق من نجاح كتابة ملف الزمن
    if (check_file_nonempty(filename)) {
        printf("Execution time file write: pass\n");
    } else {
        printf("Execution time file write: fail\n");
    }

    return 0;
}
