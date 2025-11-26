#include <stdio.h>

int maxSubArraySum(int arr[], int n) {
    //error: ‘maxGlobal’ undeclared (first use in this function), just define the variable
    int maxGlobal;
    int maxCurrent = maxGlobal = arr[0];
    for (int i = 1; i < n; i++) {
        maxCurrent = (arr[i] > maxCurrent + arr[i]) ? arr[i] : maxCurrent + arr[i];
        if (maxCurrent > maxGlobal) {
            maxGlobal = maxCurrent;
        }
    }
    return maxGlobal;
}

int main() {
    int n;
    printf("Enter the number of elements: ");
    scanf("%d", &n);
    int arr[n];
    printf("Enter the array elements separated by space: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    printf("Maximum contiguous sum is %d\n", maxSubArraySum(arr, n));
    return 0;
}