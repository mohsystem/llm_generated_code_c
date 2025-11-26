#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_NODES 6

struct Graph {
    int num_nodes;
    int adj_list[MAX_NODES][MAX_NODES];
};

bool visited[MAX_NODES];
int output[MAX_NODES];
int output_index = 0;

void depthFirstSearch(struct Graph *graph, int start_node) {
    visited[start_node] = true;
    output[output_index++] = start_node;

    for (int i = 0; i < graph->num_nodes; i++) {
        if (graph->adj_list[start_node][i] && !visited[i]) {
            depthFirstSearch(graph, i);
        }
    }
}

int arraysEqual(int *a, int *b, int size) {
    for (int i = 0; i < size; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

void printArray(int *arr, int size) {
    for (int i=0; i<size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void runTest() {
    struct Graph graph = {
        .num_nodes = MAX_NODES,
        .adj_list = {
            {0, 1, 1, 0, 0, 0},
            {1, 0, 0, 1, 1, 0},
            {1, 0, 0, 0, 0, 1},
            {0, 1, 0, 0, 0, 0},
            {0, 1, 0, 0, 0, 1},
            {0, 0, 1, 0, 1, 0}
        }
    };

    // reset visited and output
    memset(visited, 0, sizeof(visited));
    output_index = 0;

    int start_node = 0;
    depthFirstSearch(&graph, start_node);

    int expected[] = {0, 1, 3, 4, 5, 2};

    if (output_index == MAX_NODES && arraysEqual(output, expected, MAX_NODES)) {
        printf("Test Passed\n");
    } else {
        printf("Test Failed\nExpected: ");
        printArray(expected, MAX_NODES);
        printf("Got:      ");
        printArray(output, output_index);
    }
}

int main() {
    runTest();
    return 0;
}
