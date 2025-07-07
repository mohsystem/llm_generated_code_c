#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct {
    char name[6];
} Robot;

static char usedNames[100000][6] = {0};
static int usedNamesCount = 0;

void generateName(Robot* robot) {
    // Remove repeated srand(time(NULL)) calls here:
    // srand(time(NULL));  // <-- Only call once in main

    while (true) {
        char name[6];
        name[0] = 'A' + rand() % 26;
        name[1] = 'A' + rand() % 26;
        sprintf(name + 2, "%03d", rand() % 1000);
        int exists = 0;
        for (int i = 0; i < usedNamesCount; i++) {
            if (strcmp(usedNames[i], name) == 0) {
                exists = 1;
                break;
            }
        }
        if (!exists) {
            strcpy(robot->name, name);
            strcpy(usedNames[usedNamesCount], name);
            usedNamesCount++;
            return;
        }
    }
}

void reset(Robot* robot) {
    generateName(robot);
}

// === Test Harness ===
int main() {
    srand(time(NULL));

    Robot robot;
    generateName(&robot);
    printf("Initial name: %s\n", robot.name);

    char oldName[6];
    strcpy(oldName, robot.name);

    reset(&robot);
    printf("Reset name: %s\n", robot.name);

    if (strcmp(oldName, robot.name) == 0) {
        printf("FAIL: Reset name did not change\n");
        return 1;
    }

    for (int i = 0; i < usedNamesCount - 1; i++) {
        if (strcmp(usedNames[i], robot.name) == 0) {
            printf("FAIL: Reset name duplicates existing name\n");
            return 1;
        }
    }

    // Create multiple robots, check all names unique
    for (int i = usedNamesCount; i < 20; i++) {
        Robot r;
        generateName(&r);
    }

    for (int i = 0; i < usedNamesCount; i++) {
        for (int j = i + 1; j < usedNamesCount; j++) {
            if (strcmp(usedNames[i], usedNames[j]) == 0) {
                printf("FAIL: Duplicate name found: %s\n", usedNames[i]);
                return 1;
            }
        }
    }

    printf("All test cases passed!\n");
    return 0;
}
