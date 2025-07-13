#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

int gcd(int a, int b) {
    if (b == 0) {
        return a;
    } else {
        return gcd(b, a % b);
    }
}

int multiplicativeInverse(int e, int phi) {
    int x = 0;
    int y = 1;
    int a = phi;
    int b = e;

    while (b != 0) {
        int q = a / b;
        int temp = b;
        b = a % b;
        a = temp;
        temp = x;
        x = y - q * x;
        y = temp;
    }

    if (y < 0) {
        y += phi;
    }

    return y;
}

void generateKeyPair(int p, int q, int* publicKey, int* privateKey) {
    int n = p * q;
    int phi = (p - 1) * (q - 1);

    srand(time(NULL));
    int e = rand() % phi + 1;
    while (gcd(e, phi) != 1) {
        e = rand() % phi + 1;
    }

    int d = multiplicativeInverse(e, phi);
    *publicKey = e;
    *privateKey = d;
}

void encrypt(int publicKey, int n, char* plaintext, int* ciphertext, int length) {
    for (int i = 0; i < length; i++) {
        ciphertext[i] = (int)pow(plaintext[i], publicKey) % n;
    }
}

void decrypt(int privateKey, int n, int* ciphertext, char* plaintext, int length) {
    for (int i = 0; i < length; i++) {
        plaintext[i] = (int)pow(ciphertext[i], privateKey) % n;
    }
    plaintext[length] = '\0';
}

int main() {
    int p = 61;
    int q = 53;
    int publicKey, privateKey;
    generateKeyPair(p, q, &publicKey, &privateKey);
    printf("Public Key: %d, %d\n", publicKey, p * q);
    printf("Private Key: %d, %d\n", privateKey, p * q);

    // Test cases
    const char* test_cases[] = {
        "",
        "Hello",
        "RSA Test 123!"
    };

    for (int t = 0; t < 3; t++) {
        const char* message = test_cases[t];
        int length = strlen(message);
        int* encryptedMessage = malloc(length * sizeof(int));
        char* decryptedMessage = malloc((length + 1) * sizeof(char));

        encrypt(publicKey, p * q, (char*)message, encryptedMessage, length);

        // Print encrypted message (optional)
        printf("Test case %d Encrypted: ", t + 1);
        for (int i = 0; i < length; i++) {
            printf("%d ", encryptedMessage[i]);
        }
        printf("\n");

        decrypt(privateKey, p * q, encryptedMessage, decryptedMessage, length);

        printf("Test case %d Decrypted: %s\n", t + 1, decryptedMessage);

        if (strcmp(message, decryptedMessage) == 0) {
            printf("Test case %d: PASS\n\n", t + 1);
        } else {
            printf("Test case %d: FAIL\n\n", t + 1);
        }

        free(encryptedMessage);
        free(decryptedMessage);
    }

    return 0;
}
