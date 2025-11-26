#include <stdio.h>
#include <string.h>

void createCipher(int key, char cipher[][2]) {
    for (int i = 0; i < 26; i++) {
        cipher[i][0] = (char) (65 + i);
        cipher[i][1] = (char) (65 + (i + key) % 26);
        cipher[i+26][0] = (char) (97 + i);
        cipher[i+26][1] = (char) (97 + (i + key) % 26);
    }
}

void encryptMessage(char message[], int key, char cipher[][2]) {
    for (int i = 0; message[i] != '\0'; i++) {
        for (int j = 0; j < 52; j++) {
            if (message[i] == cipher[j][0]) {
                message[i] = cipher[j][1];
                break;
            }
        }
    }
}

// مقارنة نصوص بسيطة
int strings_equal(const char* a, const char* b) {
    return strcmp(a, b) == 0;
}

#define MAX_LENGTH 1000

void run_test_cases() {
    struct TestCase {
        char input[MAX_LENGTH];
        int key;
        const char* expected;
    } tests[] = {
        {"HELLO", 3, "KHOOR"},
        {"HELLO WORLD", 5, "MJQQT BTWQI"},
        {"HELLO, WORLD!", 2, "JGNNQ, YQTNF!"},
        {"", 10, ""},
        {"HELLO123", 4, "LIPPS123"},
        {"PYTHON", 6, "VEDUTS"},
        {"PYTHON", 8, "XAFVQP"},
        {"Python", 7, "Wfaavu"},  // لاحظ حالة الأحرف هنا متوافقة مع الشيفرة
        {"TEST", 9, "CDBC"},
        {"test", 11, "efef"}
    };

    char cipher[52][2];
    char msg_copy[MAX_LENGTH];

    for (int i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        strncpy(msg_copy, tests[i].input, MAX_LENGTH);
        createCipher(tests[i].key, cipher);
        encryptMessage(msg_copy, tests[i].key, cipher);

        int pass = strings_equal(msg_copy, tests[i].expected);
        printf("Test case %d: %s\n", i+1, pass ? "PASS" : "FAIL");
        if (!pass) {
            printf("  Input: \"%s\" with key %d\n", tests[i].input, tests[i].key);
            printf("  Expected: %s\n", tests[i].expected);
            printf("  Got: %s\n", msg_copy);
        }
    }
}

int main() {
    run_test_cases();
    return 0;
}
