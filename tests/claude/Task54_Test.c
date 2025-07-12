#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define OTP_LENGTH 6

char* generateOTP() {
    static char otp[OTP_LENGTH + 1];
    const char digits[] = "0123456789";

    srand(time(NULL));
    for (int i = 0; i < OTP_LENGTH; i++) {
        otp[i] = digits[rand() % 10];
    }
    otp[OTP_LENGTH] = '\0';

    return otp;
}

void sendOTP(const char* otp) {
    printf("OTP sent: %s\n", otp);
}

int verifyOTP(const char* userOTP, const char* generatedOTP) {
    return strcmp(userOTP, generatedOTP) == 0;
}

int login_with_credentials(const char* username, const char* password, const char* inputOTP, char* generatedOTP_out) {
    if (strcmp(username, "user") == 0 && strcmp(password, "password") == 0) {
        char* otp = generateOTP();
        strcpy(generatedOTP_out, otp);
        sendOTP(otp);
        if (verifyOTP(inputOTP, otp)) {
            printf("Login successful!\n");
            return 1; // success
        } else {
            printf("Invalid OTP. Login failed.\n");
            return 0; // fail OTP
        }
    } else {
        printf("Invalid username or password.\n");
        return 0; // fail username/password
    }
}

int main() {
    int pass = 0, fail = 0;
    char generatedOTP[OTP_LENGTH + 1];

    // Test 1: Correct username/password and correct OTP
    // Simulate: username="user", password="password", inputOTP = generated OTP (simulate)
    char* otp = generateOTP();
    strcpy(generatedOTP, otp);
    sendOTP(generatedOTP);
    if (login_with_credentials("user", "password", generatedOTP, generatedOTP)) {
        printf("Test 1: PASS\n");
        pass++;
    } else {
        printf("Test 1: FAIL\n");
        fail++;
    }

    // Test 2: Correct username/password, wrong OTP
    strcpy(generatedOTP, generateOTP());
    sendOTP(generatedOTP);
    if (!login_with_credentials("user", "password", "000000", generatedOTP)) {
        printf("Test 2: PASS\n");
        pass++;
    } else {
        printf("Test 2: FAIL\n");
        fail++;
    }

    // Test 3: Wrong username, any password
    if (!login_with_credentials("wronguser", "password", "123456", generatedOTP)) {
        printf("Test 3: PASS\n");
        pass++;
    } else {
        printf("Test 3: FAIL\n");
        fail++;
    }

    // Test 4: Correct username, wrong password
    if (!login_with_credentials("user", "wrongpassword", "123456", generatedOTP)) {
        printf("Test 4: PASS\n");
        pass++;
    } else {
        printf("Test 4: FAIL\n");
        fail++;
    }

    // Test 5: Empty username and password
    if (!login_with_credentials("", "", "123456", generatedOTP)) {
        printf("Test 5: PASS\n");
        pass++;
    } else {
        printf("Test 5: FAIL\n");
        fail++;
    }

    // ملخص النتائج
    printf("\nTotal Passed: %d\nTotal Failed: %d\n", pass, fail);

    return 0;
}
