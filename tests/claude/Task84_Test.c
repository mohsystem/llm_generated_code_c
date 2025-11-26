#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SESSION_ID_LENGTH 36  // مع ال '-'

void generate_session_id(char *session_id) {
    const char charset[] = "0123456789abcdef";
    int i;

    srand(time(NULL));

    for (i = 0; i < SESSION_ID_LENGTH; i++) {
        if (i == 8 || i == 13 || i == 18 || i == 23) {
            session_id[i] = '-';
        } else {
            int index = rand() % (sizeof(charset) - 1);
            session_id[i] = charset[index];
        }
    }
    session_id[SESSION_ID_LENGTH] = '\0';
}

// دالة لفحص صحة الـ session_id
int validate_session_id(const char *session_id) {
    if (strlen(session_id) != SESSION_ID_LENGTH)
        return 0;

    if (session_id[8] != '-' || session_id[13] != '-' || session_id[18] != '-' || session_id[23] != '-')
        return 0;

    for (int i = 0; i < SESSION_ID_LENGTH; i++) {
        if (i == 8 || i == 13 || i == 18 || i == 23)
            continue;
        char c = session_id[i];
        if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f')))
            return 0;
    }
    return 1;
}

void test_case_1() {
    char session_id[SESSION_ID_LENGTH + 1];
    generate_session_id(session_id);
    printf("Generated Session ID: %s\n", session_id);

    if (validate_session_id(session_id)) {
        printf("Test case 1: pass\n");
    } else {
        printf("Test case 1: fail\n");
    }
}

int main() {
    test_case_1();
    return 0;
}
