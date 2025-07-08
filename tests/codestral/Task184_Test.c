#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define NAME_LENGTH 5
#define UPPERCASE_LETTERS "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define DIGITS "0123456789"
#define NUM_ROBOTS 1000

char names[NUM_ROBOTS][NAME_LENGTH + 1];
bool used[NUM_ROBOTS]; // لتمييز ما إذا تم استخدام اسم في المصفوفة

// دالة للتحقق إذا الاسم موجود بالفعل بين الأسماء المخزنة
bool is_unique_name(const char* name, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(name, names[i]) == 0) {
            return false;
        }
    }
    return true;
}

void generate_name(char* name, int count) {
    const char* letters = UPPERCASE_LETTERS;
    const char* digits = DIGITS;
    bool unique;
    do {
        for (int i = 0; i < NAME_LENGTH; i++) {
            if (i < 2) {
                name[i] = letters[rand() % strlen(letters)];
            } else {
                name[i] = digits[rand() % strlen(digits)];
            }
        }
        name[NAME_LENGTH] = '\0';
        unique = is_unique_name(name, count);
    } while (!unique);
}

int main() {
    srand(time(NULL));

    for (int i = 0; i < NUM_ROBOTS; i++) {
        generate_name(names[i], i);
    }

    // تحقق من عدم وجود أسماء مكررة (تأكيد)
    for (int i = 0; i < NUM_ROBOTS; i++) {
        for (int j = i + 1; j < NUM_ROBOTS; j++) {
            if (strcmp(names[i], names[j]) == 0) {
                printf("Names are not unique!\n");
                return 1;
            }
        }
    }

    printf("All names are unique.\n");
    return 0;
}
