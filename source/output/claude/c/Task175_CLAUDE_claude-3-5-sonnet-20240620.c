#include <stdio.h>
#include <stdlib.h>

/**
 * This is the MountainArray's API interface.
 * You should not implement it, or speculate about its implementation
 * struct MountainArray;
 * int get(MountainArray *, int index);
 * int length(MountainArray *);
 */

struct MountainArray;

int get(struct MountainArray* mountainArr, int index){return 1;}
int length(struct MountainArray* mountainArr){return 1;}

int findPeakIndex(struct MountainArray* mountainArr) {
    int left = 0;
    int right = length(mountainArr) - 1;
    
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (get(mountainArr, mid) < get(mountainArr, mid + 1)) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    
    return left;
}

int binarySearch(struct MountainArray* mountainArr, int target, int left, int right, int ascending) {
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int midVal = get(mountainArr, mid);
        
        if (midVal == target) {
            return mid;
        }
        
        if (ascending) {
            if (midVal < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        } else {
            if (midVal > target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
    }
    
    return -1;
}

int findInMountainArray(int target, struct MountainArray* mountainArr) {
    int peakIndex = findPeakIndex(mountainArr);
    
    // Search in the ascending part
    int result = binarySearch(mountainArr, target, 0, peakIndex, 1);
    if (result != -1) {
        return result;
    }
    
    // Search in the descending part
    return binarySearch(mountainArr, target, peakIndex + 1, length(mountainArr) - 1, 0);
}