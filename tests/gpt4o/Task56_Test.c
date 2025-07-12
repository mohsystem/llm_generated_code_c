#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

void generateToken(unsigned char token[16]) {
    for (int i = 0; i < 16; ++i) {
        token[i] = rand() % 256;
    }
}

int validateHexOutput(const char* hexString) {
    for (int i = 0; i < 32; i++) {
        char c = hexString[i];
        if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))) {
            return 0;
        }
    }
    return 1;
}


int main() {
    unsigned char token[16];
    char hexOutput[33]; // 32 chars + null terminator
    srand(time(0));

    generateToken(token);

    // تحويل البايتات إلى hex في مصفوفة نصية
    for (int i = 0; i < 16; ++i) {
        sprintf(&hexOutput[i*2], "%02x", token[i]);
    }
    hexOutput[32] = '\0';

    printf("C Generated Token: %s\n", hexOutput);

    int pass = 0, fail = 0;

    // Test 1: تحقق طول الإخراج 32
    if (strlen(hexOutput) == 32) {
        printf("Test 1: PASS (Correct hex string length)\n");
        pass++;
    } else {
        printf("Test 1: FAIL (Incorrect hex string length)\n");
        fail++;
    }

    // Test 2: تحقق صلاحية أحرف hex
    if (validateHexOutput(hexOutput)) {
        printf("Test 2: PASS (Valid hex characters)\n");
        pass++;
    } else {
        printf("Test 2: FAIL (Invalid hex characters)\n");
        fail++;
    }

    printf("\nTotal Passed: %d\nTotal Failed: %d\n", pass, fail);
    return 0;
}
