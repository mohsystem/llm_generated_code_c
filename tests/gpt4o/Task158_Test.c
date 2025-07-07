#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// دالة تنفذ أمر وتعيد 1 لو نجح، 0 لو فشل
int runTest(const char* input) {
    char command[256];
    strncpy(command, input, sizeof(command) - 1);
    command[sizeof(command) - 1] = '\0';

    // نحذف ال newline لو موجودة
    size_t len = strlen(command);
    if (len > 0 && command[len - 1] == '\n') {
        command[len - 1] = '\0';
    }

    int ret = system(command);
    return ret == 0;
}

int main() {
    // Test 1: أمر echo بسيط
    assert(runTest("echo Hello"));

    // Test 2: أمر echo مع كلمات متعددة
    assert(runTest("echo Hello World"));

    // Test 3: أمر فارغ (يعني enter بدون أمر)
    assert(runTest(""));

    // Test 4: أمر مع رموز خاصة
    assert(runTest("echo !@#$%^&*()"));

    // Test 5: أمر طويل جدًا (255 حرف)
    char longCmd[256];
    for (int i = 0; i < 255; i++) longCmd[i] = 'a';
    longCmd[255] = '\0';
    assert(runTest(longCmd));

    printf("All test cases passed!\n");
    return 0;
}
