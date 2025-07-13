#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void generateSessionID(char* sessionID) {
    const char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    size_t characters_len = strlen(characters);
    for (int i = 0; i < 16; i++) {
        sessionID[i] = characters[rand() % characters_len];
    }
    sessionID[16] = '\0';
}

// دالة تحقق من صحة الـ sessionID
int validate_session_id(const char* sessionID) {
    if (strlen(sessionID) != 16)
        return 0;

    const char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    size_t characters_len = strlen(characters);

    for (int i = 0; i < 16; i++) {
        int found = 0;
        for (size_t j = 0; j < characters_len; j++) {
            if (sessionID[i] == characters[j]) {
                found = 1;
                break;
            }
        }
        if (!found)
            return 0;
    }
    return 1;
}

void test_case_1() {
    srand(time(0));
    char sessionID[17];
    generateSessionID(sessionID);
    printf("C generated session ID: %s\n", sessionID);

    if (validate_session_id(sessionID)) {
        printf("Test case 1: pass\n");
    } else {
        printf("Test case 1: fail\n");
    }
}

int main() {
    test_case_1();
    return 0;
}
