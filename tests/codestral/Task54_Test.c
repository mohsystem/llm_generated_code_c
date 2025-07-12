#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

const char CHARACTERS[] = "0123456789";
const int OTP_LENGTH = 6;

char* generate_otp() {
    char* otp = malloc(OTP_LENGTH + 1);
    for (int i = 0; i < OTP_LENGTH; i++) {
        otp[i] = CHARACTERS[rand() % strlen(CHARACTERS)];
    }
    otp[OTP_LENGTH] = '\0';
    return otp;
}

char* login(const char* username, const char* password, const char* otp) {
    // هنا منطق غير عملي: يولد OTP جديد و يقارن مع otp المدخل
    if (strcmp(username, "user") == 0 && strcmp(password, "password") == 0) {
        char* generated_otp = generate_otp();
        if (strcmp(otp, generated_otp) == 0) {
            free(generated_otp);
            return "Login successful";
        } else {
            free(generated_otp);
            return "Invalid OTP";
        }
    } else {
        return "Invalid username or password";
    }
}

int main() {
    srand(time(0));
    int pass = 0, fail = 0;

    // لتحسين قابلية الاختبار، سنحاكي حالات كثيرة مع التحقق من النتائج الفعلية

    // Test 1: correct username/password, otp == generated_otp inside login => بسبب المنطق غير المتوقع:
    // لن يكون ممكن تمرير هذا لأنه يولد otp جديد داخل login، لذلك عادة لن ينجح
    // لذلك نتوقع fail مع هذا الكود.
    {
        char* otp = generate_otp();
        char* result = login("user", "password", otp);
        if (strcmp(result, "Login successful") == 0) {
            printf("Test 1: PASS\n");
            pass++;
        } else {
            printf("Test 1: FAIL\n");
            fail++;
        }
        free(otp);
    }

    // Test 2: correct username/password, wrong otp
    {
        char* result = login("user", "password", "000000");
        if (strcmp(result, "Invalid OTP") == 0) {
            printf("Test 2: PASS\n");
            pass++;
        } else {
            printf("Test 2: FAIL\n");
            fail++;
        }
    }

    // Test 3: wrong username
    {
        char* result = login("wronguser", "password", "123456");
        if (strcmp(result, "Invalid username or password") == 0) {
            printf("Test 3: PASS\n");
            pass++;
        } else {
            printf("Test 3: FAIL\n");
            fail++;
        }
    }

    // Test 4: wrong password
    {
        char* result = login("user", "wrongpassword", "123456");
        if (strcmp(result, "Invalid username or password") == 0) {
            printf("Test 4: PASS\n");
            pass++;
        } else {
            printf("Test 4: FAIL\n");
            fail++;
        }
    }

    // Test 5: empty username and password
    {
        char* result = login("", "", "123456");
        if (strcmp(result, "Invalid username or password") == 0) {
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
