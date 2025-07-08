#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ROBOTS 10000
#define NAME_LENGTH 6

char used_names[MAX_ROBOTS][NAME_LENGTH];
int used_count = 0;

// تحقق إذا الاسم موجود مسبقًا
int is_name_used(const char* name) {
    for (int i = 0; i < used_count; i++) {
        if (strcmp(used_names[i], name) == 0) {
            return 1;
        }
    }
    return 0;
}

void generate_name(char *name) {
    const char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char digits[] = "0123456789";

    do {
        name[0] = letters[rand() % 26];
        name[1] = letters[rand() % 26];
        name[2] = digits[rand() % 10];
        name[3] = digits[rand() % 10];
        name[4] = digits[rand() % 10];
        name[5] = '\0';
    } while (is_name_used(name));

    strcpy(used_names[used_count++], name);
}

typedef struct {
    char name[NAME_LENGTH];
} Robot;

void robot_init(Robot *robot) {
    generate_name(robot->name);
}

void robot_reset(Robot *robot) {
    generate_name(robot->name);
}

int main() {
    srand(time(NULL));

    Robot robot;
    robot_init(&robot);
    printf("Initial name: %s\n", robot.name);

    robot_reset(&robot);
    printf("After reset: %s\n", robot.name);

    // تأكد أن الاسم مختلف بعد إعادة التعيين
    if (strcmp(robot.name, used_names[used_count-2]) == 0) {
        printf("Fail: Duplicate name generated!\n");
    } else {
        printf("Pass: Unique name generated after reset.\n");
    }

    return 0;
}
