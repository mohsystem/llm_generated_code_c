#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char* generate_session_id(size_t length) {
    const char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    char *session_id = malloc(length + 1);
    if (session_id == NULL) {
        return NULL;
    }

    srand(time(NULL));
    for (size_t i = 0; i < length; ++i) {
        session_id[i] = characters[rand() % (sizeof(characters) - 1)];
    }
    session_id[length] = '\0';

    return session_id;
}

// تحقق أن الـ session_id صحيح: طوله length وأن كل حرف ضمن allowed charset
int validate_session_id(const char *session_id, size_t length) {
    if (strlen(session_id) != length)
        return 0;

    const char allowed[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

    for (size_t i = 0; i < length; i++) {
        int found = 0;
        for (size_t j = 0; j < sizeof(allowed)-1; j++) {
            if (session_id[i] == allowed[j]) {
                found = 1;
                break;
            }
        }
        if (!found) return 0;
    }
    return 1;
}

void test_case_1() {
    size_t length = 32;  // طول session ID المطلوب (يمكن تغيره حسب متطلباتك)
    char *session_id = generate_session_id(length);
    if (!session_id) {
        printf("Test case 1: fail (malloc failed)\n");
        return;
    }

    printf("Generated session ID: %s\n", session_id);

    if (validate_session_id(session_id, length)) {
        printf("Test case 1: pass\n");
    } else {
        printf("Test case 1: fail\n");
    }

    free(session_id);
}

int main() {
    test_case_1();
    return 0;
}
