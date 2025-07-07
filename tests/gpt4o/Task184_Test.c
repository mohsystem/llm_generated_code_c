#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define NAME_LENGTH 5
#define MAX_ROBOTS 1000

typedef struct Robot {
    char name[NAME_LENGTH + 1];
} Robot;

char usedNames[MAX_ROBOTS][NAME_LENGTH + 1];
int usedCount = 0;

void generateRandomName(char* name) {
    const char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char digits[] = "0123456789";

    for(int i = 0; i < 2; i++) {
        name[i] = letters[rand() % 26];
    }
    for(int i = 2; i < 5; i++) {
        name[i] = digits[rand() % 10];
    }
    name[NAME_LENGTH] = '\0';
}

int isNameUnique(const char* name) {
    for(int i = 0; i < usedCount; i++) {
        if(strcmp(usedNames[i], name) == 0) {
            return 0;
        }
    }
    return 1;
}

void setName(Robot* robot) {
    do {
        generateRandomName(robot->name);
    } while(!isNameUnique(robot->name));
    strcpy(usedNames[usedCount++], robot->name);
}

void reset(Robot* robot) {
    setName(robot);
}

// ==== Test harness ====

int main() {
    srand(time(NULL));

    Robot robot;
    setName(&robot);
    printf("Initial name: %s\n", robot.name);

    // Reset and check the name changes and remains unique
    char oldName[NAME_LENGTH + 1];
    strcpy(oldName, robot.name);

    reset(&robot);
    printf("Reset name: %s\n", robot.name);

    if (strcmp(oldName, robot.name) == 0) {
        printf("FAIL: Reset name did not change\n");
        return 1;
    }

    for (int i = 0; i < usedCount - 1; i++) {
        if (strcmp(usedNames[i], robot.name) == 0 && i != usedCount - 1) {
            printf("FAIL: Reset name duplicates an existing name\n");
            return 1;
        }
    }

    // Create multiple robots and check all names unique
    for (int i = usedCount; i < 10; i++) {
        Robot r;
        setName(&r);
    }

    for (int i = 0; i < usedCount; i++) {
        for (int j = i + 1; j < usedCount; j++) {
            if (strcmp(usedNames[i], usedNames[j]) == 0) {
                printf("FAIL: Duplicate name found: %s\n", usedNames[i]);
                return 1;
            }
        }
    }

    printf("All test cases passed!\n");
    return 0;
}
