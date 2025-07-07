#include <stdio.h>
#include <string.h>

// Assume previous code is here or included

int test_unique_names() {
    srand(time(NULL));

    // Create 10 robots (like testing opening accounts)
    for (int i = 0; i < 10; i++) {
        Robot* r = create_robot();
        if (r == NULL) {
            printf("Test Failed: Unable to create robot %d\n", i+1);
            return 1;
        }
    }

    // Check uniqueness of initial robot names
    for (int i = 0; i < robot_count; i++) {
        for (int j = i + 1; j < robot_count; j++) {
            if (strcmp(robots[i].name, robots[j].name) == 0) {
                printf("Test Failed: Duplicate initial name %s found\n", robots[i].name);
                return 1;
            }
        }
    }

    // Reset first robot and check new name is unique and different from old
    char old_name[6];
    strcpy(old_name, robots[0].name);
    reset_robot(&robots[0]);

    if (strcmp(old_name, robots[0].name) == 0) {
        printf("Test Failed: Reset did not change robot name\n");
        return 1;
    }

    for (int i = 1; i < robot_count; i++) {
        if (strcmp(robots[0].name, robots[i].name) == 0) {
            printf("Test Failed: Reset name duplicates another robot's name\n");
            return 1;
        }
    }

    printf("All test cases passed!\n");
    return 0;
}

int main() {
    return test_unique_names();
}
