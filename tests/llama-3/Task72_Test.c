#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <unistd.h> // pipe, dup2, read, close

void print_hex(const unsigned char* data, int len) {
    for (int i = 0; i < len; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}

void program_main(int argc, char* argv[]) {
    const char *plainText = "Hello, World!";
    unsigned char iv[AES_BLOCK_SIZE];  // static IV
    for (int i = 0; i < AES_BLOCK_SIZE; i++) {
        iv[i] = 0;
    }
    unsigned char key[] = "your_secret_key_here";

    AES_KEY aes_key;
    AES_set_encrypt_key(key, 128, &aes_key);

    int len = strlen(plainText);
    int blockSize = AES_BLOCK_SIZE;
    int padding = blockSize - (len % blockSize);
    int padded_len = len + padding;

    unsigned char *encryptedData = malloc(padded_len);
    if (!encryptedData) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // PKCS7 padding
    unsigned char *padded_input = malloc(padded_len);
    if (!padded_input) {
        free(encryptedData);
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    memcpy(padded_input, plainText, len);
    for (int i = 0; i < padding; i++) {
        padded_input[len + i] = (unsigned char)padding;
    }

    AES_cbc_encrypt(padded_input, encryptedData, padded_len, &aes_key, iv, AES_ENCRYPT);

    print_hex(encryptedData, padded_len);

    free(encryptedData);
    free(padded_input);
}

// Capture stdout and stderr for testing
int capture_output(void (*func)(int,char**), int argc, char* argv[],
                   char* out_buf, size_t out_size,
                   char* err_buf, size_t err_size) {
    int out_pipe[2], err_pipe[2];
    if (pipe(out_pipe) == -1) return -1;
    if (pipe(err_pipe) == -1) return -1;

    fflush(stdout);
    fflush(stderr);

    int stdout_copy = dup(fileno(stdout));
    int stderr_copy = dup(fileno(stderr));
    if (stdout_copy == -1 || stderr_copy == -1) return -1;

    dup2(out_pipe[1], fileno(stdout));
    dup2(err_pipe[1], fileno(stderr));
    close(out_pipe[1]);
    close(err_pipe[1]);

    func(argc, argv);

    fflush(stdout);
    fflush(stderr);

    dup2(stdout_copy, fileno(stdout));
    dup2(stderr_copy, fileno(stderr));
    close(stdout_copy);
    close(stderr_copy);

    ssize_t out_len = read(out_pipe[0], out_buf, out_size - 1);
    ssize_t err_len = read(err_pipe[0], err_buf, err_size - 1);
    close(out_pipe[0]);
    close(err_pipe[0]);

    if (out_len < 0 || err_len < 0) return -1;

    out_buf[out_len] = '\0';
    err_buf[err_len] = '\0';

    return 0;
}

void print_result(const char* test_name, int passed) {
    printf("%s: %s\n", test_name, passed ? "PASS" : "FAIL");
}

// Test: verify output length and format (hex)
void test_encryption_output() {
    char out[1024];
    char err[1024];
    char* argv[] = {"program"};
    int argc = 1;

    if (capture_output(program_main, argc, argv, out, sizeof(out), err, sizeof(err)) != 0) {
        print_result("test_encryption_output", 0);
        return;
    }

    size_t out_len = strlen(out);
    // For "Hello, World!" length 13 padded to 16 bytes encrypted → 16 bytes ciphertext = 32 hex chars + newline
    int valid = 0;
    if (out_len >= 32 && out_len <= 33) {
        valid = 1;
        for (size_t i = 0; i < out_len - 1; i++) {
            if (!((out[i] >= '0' && out[i] <= '9') || (out[i] >= 'a' && out[i] <= 'f'))) {
                valid = 0;
                break;
            }
        }
        if (out[out_len - 1] != '\n') valid = 0;
    }
    print_result("test_encryption_output", valid);
}

int main() {
    test_encryption_output();
    return 0;
}
