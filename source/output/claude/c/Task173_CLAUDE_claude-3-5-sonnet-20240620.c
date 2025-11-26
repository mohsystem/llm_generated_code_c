#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 30001

int sa[MAX_N], tmp[MAX_N], pos[MAX_N];
int lcp[MAX_N], gap, N;
char str[MAX_N];

int cmp_suffix(const void* a, const void* b) {
    int* x = (int*)a, *y = (int*)b;
    if (pos[*x] != pos[*y])
        return pos[*x] - pos[*y];
    x += gap;
    y += gap;
    return (x < sa + N ? pos[*x] : -1) - (y < sa + N ? pos[*y] : -1);
}

void build_suffix_array() {
    int i;
    for (i = 0; i < N; i++) {
        sa[i] = i;
        pos[i] = str[i];
    }
    for (gap = 1;; gap *= 2) {
        qsort(sa, N, sizeof(int), cmp_suffix);
        for (i = 0; i < N - 1; i++)
            tmp[i + 1] = tmp[i] + cmp_suffix(sa + i, sa + i + 1);
        for (i = 0; i < N; i++)
            pos[sa[i]] = tmp[i];
        if (tmp[N - 1] == N - 1)
            break;
    }
}

void build_lcp() {
    int i, j, k = 0;
    for (i = 0; i < N; i++) pos[sa[i]] = i;
    for (i = 0; i < N; i++) {
        if (pos[i] == N - 1) {
            k = 0;
            continue;
        }
        j = sa[pos[i] + 1];
        while (i + k < N && j + k < N && str[i + k] == str[j + k])
            k++;
        lcp[pos[i]] = k;
        if (k) k--;
    }
}

char* longestDupSubstring(char* s) {
    N = strlen(s);
    strcpy(str, s);
    
    build_suffix_array();
    build_lcp();
    
    int max_len = 0, start = 0;
    for (int i = 0; i < N - 1; i++) {
        if (lcp[i] > max_len) {
            max_len = lcp[i];
            start = sa[i];
        }
    }
    
    if (max_len == 0) {
        return "";
    }
    
    char* result = (char*)malloc((max_len + 1) * sizeof(char));
    strncpy(result, s + start, max_len);
    result[max_len] = '\0';
    
    return result;
}

int main() {
    char s[MAX_N];
    scanf("%s", s);
    
    char* result = longestDupSubstring(s);
    printf("%s\n", result);
    
    free(result);
    return 0;
}