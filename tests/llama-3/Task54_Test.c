#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char* generateOTP() {
    static char otp[7];
    srand(time(0));
    sprintf(otp, "%06d", rand() % 900000 + 100000);
    return otp;
}

char* authenticate(char* username, char* password, char* otp) {
    if (strcmp(username, "admin") == 0 && strcmp(password, "password") == 0 && strcmp(otp, generateOTP()) == 0) {
        return "Authentication successful";
    } else {
        return "Authentication failed";
    }
}

// دالة اختبارية تحاكي الحالات
int test_authenticate(char* username, char* password, char* otp, int expectSuccess) {
    char* result = authenticate(username, password, otp);
    if (expectSuccess) {
        return strcmp(result, "Authentication successful") == 0;
    } else {
        return strcmp(result, "Authentication failed") == 0;
    }
}

int main() {
    int pass = 0, fail = 0;

    // Test 1: admin/password مع OTP مطابق (مستحيل بسبب التوليد داخل الدالة)
    // نحاول نستخدم OTP من generateOTP() لكن ستولد جديدة داخل authenticate()
    {
        char* otp = generateOTP();
        if (test_authenticate("admin", "password", otp, 1)) {
            printf("Test 1: PASS\n");
            pass++;
        } else {
            printf("Test 1: FAIL\n");
            fail++;
        }
    }

    // Test 2: admin/password مع OTP خاطئ
    {
        if (test_authenticate("admin", "password", "000000", 0)) {
            printf("Test 2: PASS\n");
            pass++;
        } else {
            printf("Test 2: FAIL\n");
            fail++;
        }
    }

    // Test 3: اسم مستخدم خاطئ
    {
        if (test_authenticate("user", "password", "123456", 0)) {
            printf("Test 3: PASS\n");
            pass++;
        } else {
            printf("Test 3: FAIL\n");
            fail++;
        }
    }

    // Test 4: كلمة مرور خاطئة
    {
        if (test_authenticate("admin", "wrongpass", "123456", 0)) {
            printf("Test 4: PASS\n");
            pass++;
        } else {
            printf("Test 4: FAIL\n");
            fail++;
        }
    }

    // Test 5: اسم مستخدم وكلمة مرور فارغين
    {
        if (test_authenticate("", "", "123456", 0)) {
            printf("Test 5: PASS\n");
            pass++;
        } else {
            printf("Test 5: FAIL\n");
            fail++;
        }
    }

    printf("\nTotal Passed: %d\nTotal Failed: %d\n", pass, fail);

    return 0;
}
