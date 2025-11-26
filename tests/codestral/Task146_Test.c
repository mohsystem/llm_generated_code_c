#include <stdio.h>
#include <assert.h>

// دالة إيجاد العدد المفقود، حيث الأرقام من 1 إلى n+1 وعدد عناصر المصفوفة n
int findMissing(int arr[], int n) {
    int total = (n + 1) * (n + 2) / 2;
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }
    return total - sum;
}

void run_tests() {
    int arr1[] = {2, 3, 4, 5, 6};
    assert(findMissing(arr1, 5) == 1);

    int arr2[] = {1, 2, 3, 4, 5};
    assert(findMissing(arr2, 5) == 6);

    int arr3[] = {1, 2, 4, 5, 6};
    assert(findMissing(arr3, 5) == 3);

    // مصفوفة كبيرة مع الرقم المفقود 5000
    int arr4[9999];
    for (int i = 1, j = 0; i <= 10000; i++) {
        if (i != 5000) arr4[j++] = i;
    }
    assert(findMissing(arr4, 9999) == 5000);

    int arr5[] = {1};
    assert(findMissing(arr5, 1) == 2);

    int arr6[9999];
    for (int i = 2, j = 0; i <= 10000; i++) {
        arr6[j++] = i;
    }
    assert(findMissing(arr6, 9999) == 1);

    int arr7[9999];
    for (int i = 1, j = 0; i <= 10000; i++) {
        if (i != 5000) arr7[j++] = i;
    }
    assert(findMissing(arr7, 9999) == 5000);

    int arr8[] = {10, 11, 13, 14, 15};
    // هذا الاختبار غير صالح لدالة findMissing هذه، لأن الأرقام ليست من 1 إلى n+1
    // لذا لن نستخدمه هنا، أو يمكن حذفه.

    int arr9[] = {1, 2, 3, 4};
    assert(findMissing(arr9, 4) == 5);

    int arr10[] = {2, 3, 4, 5};
    assert(findMissing(arr10, 4) == 1);

    printf("✓ All tests passed for findMissing.\n");
}

int main() {
    run_tests();
    return 0;
}
