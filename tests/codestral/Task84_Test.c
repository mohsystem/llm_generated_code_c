#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>
#include <ctype.h>

int validate_uuid(const char *uuid_str) {
    if (strlen(uuid_str) != 36) return 0;

    if (uuid_str[8] != '-' || uuid_str[13] != '-' || uuid_str[18] != '-' || uuid_str[23] != '-')
        return 0;

    for (int i = 0; i < 36; i++) {
        if (i == 8 || i == 13 || i == 18 || i == 23)
            continue;
        if (!isxdigit(uuid_str[i]))
            return 0;
    }
    return 1;
}

char* generateSessionId() {
    uuid_t uuid;
    uuid_generate_random(uuid);
    char *str = (char*)malloc(37);
    uuid_unparse(uuid, str);
    return str;
}

void test_case_1() {
    char *sessionId = generateSessionId();
    printf("Generated Session ID: %s\n", sessionId);

    if (validate_uuid(sessionId)) {
        printf("Test case 1: pass\n");
    } else {
        printf("Test case 1: fail\n");
    }

    free(sessionId);
}

int main() {
    test_case_1();
    return 0;
}
