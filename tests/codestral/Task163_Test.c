#include <stdio.h>

int longestRun(int arr[], int n) {
    if (n == 0) {
        return 0;
    }
    int longest = 1;
    int current = 1;
    for (int i = 1; i < n; i++) {
        if (arr[i] == arr[i-1] + 1 || arr[i] == arr[i-1] - 1) {
            current++;
        } else {
            longest = (longest > current) ? longest : current;
            current = 1;
        }
    }
    return (longest > current) ? longest : current;
}

int main() {
    int arr1[] = {1, 2, 3, 5, 6, 7, 8, 9};
    int arr2[] = {1, 2, 3, 10, 11, 15};
    int arr3[] = {5, 4, 2, 1};
    int arr4[] = {3, 5, 7, 10, 15};

    printf("%d\n", longestRun(arr1, sizeof(arr1)/sizeof(arr1[0])));  // Expected Output: 5
    printf("%d\n", longestRun(arr2, sizeof(arr2)/sizeof(arr2[0])));  // Expected Output: 3
    printf("%d\n", longestRun(arr3, sizeof(arr3)/sizeof(arr3[0])));  // Expected Output: 2
    printf("%d\n", longestRun(arr4, sizeof(arr4)/sizeof(arr4[0])));  // Expected Output: 1

    return 0;
}
