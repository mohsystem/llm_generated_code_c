#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

char* generate_session_id() {
    static char session_id[37];
    srand(time(NULL));
    for(int i = 0; i < 32; i++) {
        session_id[i] = "0123456789abcdef"[rand() % 16];
    }
    session_id[32] = '\0';
    return session_id;
}

int validate_session_id(const char* session_id) {
    if (strlen(session_id) != 32) return 0;

    for (int i = 0; i < 32; i++) {
        char c = session_id[i];
        if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))) {
            return 0;
        }
    }
    return 1;
}

void test_case_1() {
    char* session_id = generate_session_id();
    printf("Generated session ID: %s\n", session_id);

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
