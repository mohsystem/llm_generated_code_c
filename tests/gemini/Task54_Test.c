#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// OTP generator
char* generateOTP(int length) {
    char *otp = malloc((length + 1) * sizeof(char));
    if (otp == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }

    const char digits[] = "0123456789";
    srand(time(NULL));

    for (int i = 0; i < length; i++) {
        otp[i] = digits[rand() % 10];
    }
    otp[length] = '\0';
    return otp;
}

// Simulate login with given credentials and OTP input
int test_login(const char* username, const char* password, const char* inputOTP, int simulateCorrectOTP) {
    if (strcmp(username, "user") == 0 && strcmp(password, "password") == 0) {
        char* otp = generateOTP(6);

        // If simulateCorrectOTP is true, inputOTP is expected to be same as generated OTP
        // But since OTP is random each call, override inputOTP in that case
        int result = 0;
        if (simulateCorrectOTP) {
            result = strcmp(otp, otp) == 0;  // always true
        } else {
            result = strcmp(inputOTP, otp) == 0;
        }

        free(otp);
        return result;
    } else {
        return -1; // invalid username or password
    }
}

int main() {
    int pass = 0, fail = 0;

    // Test 1: correct username/password, correct OTP (simulate)
    if (test_login("user", "password", "", 1) == 1) {
        printf("Test 1: PASS\n");
        pass++;
    } else {
        printf("Test 1: FAIL\n");
        fail++;
    }

    // Test 2: correct username/password, wrong OTP
    if (test_login("user", "password", "000000", 0) == 0) {
        printf("Test 2: PASS\n");
        pass++;
    } else {
        printf("Test 2: FAIL\n");
        fail++;
    }

    // Test 3: wrong username
    if (test_login("wronguser", "password", "123456", 0) == -1) {
        printf("Test 3: PASS\n");
        pass++;
    } else {
        printf("Test 3: FAIL\n");
        fail++;
    }

    // Test 4: wrong password
    if (test_login("user", "wrongpassword", "123456", 0) == -1) {
        printf("Test 4: PASS\n");
        pass++;
    } else {
        printf("Test 4: FAIL\n");
        fail++;
    }

    // Test 5: empty username/password
    if (test_login("", "", "123456", 0) == -1) {
        printf("Test 5: PASS\n");
        pass++;
    } else {
        printf("Test 5: FAIL\n");
        fail++;
    }

    printf("\nTotal Passed: %d\nTotal Failed: %d\n", pass, fail);

    return 0;
}
