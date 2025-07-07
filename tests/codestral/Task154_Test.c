#include <stdio.h>
#include <stdbool.h>

#define MAX_NODES 6

void dfs(int graph[MAX_NODES][MAX_NODES], int node, bool visited[MAX_NODES], int *output, int *index) {
    if (!visited[node]) {
        visited[node] = true;
        output[(*index)++] = node; // حفظ العقدة
        for (int neighbour = 0; neighbour < MAX_NODES; neighbour++) {
            if (graph[node][neighbour] == 1) {
                dfs(graph, neighbour, visited, output, index);
            }
        }
    }
}

int arraysEqual(int *a, int *b, int size) {
    for (int i = 0; i < size; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

void printOutput(int *arr, int size) {
    for (int i=0; i<size; i++) {
        printf("%c ", 'A' + arr[i]);
    }
    printf("\n");
}

void runTest() {
    int graph[MAX_NODES][MAX_NODES] = {
        {0, 1, 1, 0, 0, 0},
        {1, 0, 0, 1, 1, 0},
        {1, 0, 0, 0, 0, 1},
        {0, 1, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 1},
        {0, 0, 1, 0, 1, 0}
    };

    bool visited[MAX_NODES] = {false};
    int output[MAX_NODES];
    int index = 0;

    dfs(graph, 0, visited, output, &index);

    // التسلسل المتوقع للـ DFS بدءًا من 0:
    int expected[] = {0, 1, 3, 4, 5, 2};

    if (index == MAX_NODES && arraysEqual(output, expected, MAX_NODES)) {
        printf("Test Passed\n");
    } else {
        printf("Test Failed\nExpected: ");
        printOutput(expected, MAX_NODES);
        printf("Got:      ");
        printOutput(output, index);
    }
}

int main() {
    runTest();
    return 0;
}
