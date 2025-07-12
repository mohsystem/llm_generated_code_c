#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int generateOTP() {
    return 100000 + rand() % 900000;
}

int verifyOTP(int userOTP, int actualOTP) {
    return userOTP == actualOTP;
}

// دالة مساعدة تحاكي تسجيل الدخول بالمدخلات للتحقق من الاختبارات
int test_login(const char* username, const char* password, int inputOTP, int simulateCorrectOTP) {
    if (strcmp(username, "user") == 0 && strcmp(password, "pass") == 0) {
        int otp = generateOTP();

        // إذا simulateCorrectOTP = 1 نجعل المدخل يساوي otp الحقيقي (ناجح)
        // وإلا نقارن كما هو (غالبًا يفشل)
        if (simulateCorrectOTP) {
            return verifyOTP(otp, otp); // دائماً true
        } else {
            return verifyOTP(inputOTP, otp);
        }
    } else {
        return -1; // اسم مستخدم أو كلمة مرور خاطئة
    }
}

int main() {
    srand(time(NULL));
    int pass = 0, fail = 0;

    // Test 1: username/password صحيح و OTP صحيح
    if (test_login("user", "pass", 0, 1) == 1) {
        printf("Test 1: PASS\n");
        pass++;
    } else {
        printf("Test 1: FAIL\n");
        fail++;
    }

    // Test 2: username/password صحيح و OTP خاطئ
    if (test_login("user", "pass", 123456, 0) == 0) {
        printf("Test 2: PASS\n");
        pass++;
    } else {
        printf("Test 2: FAIL\n");
        fail++;
    }

    // Test 3: username خاطئ
    if (test_login("wronguser", "pass", 123456, 0) == -1) {
        printf("Test 3: PASS\n");
        pass++;
    } else {
        printf("Test 3: FAIL\n");
        fail++;
    }

    // Test 4: password خاطئ
    if (test_login("user", "wrongpass", 123456, 0) == -1) {
        printf("Test 4: PASS\n");
        pass++;
    } else {
        printf("Test 4: FAIL\n");
        fail++;
    }

    // Test 5: username و password فارغين
    if (test_login("", "", 123456, 0) == -1) {
        printf("Test 5: PASS\n");
        pass++;
    } else {
        printf("Test 5: FAIL\n");
        fail++;
    }

    printf("\nTotal Passed: %d\nTotal Failed: %d\n", pass, fail);

    return 0;
}
