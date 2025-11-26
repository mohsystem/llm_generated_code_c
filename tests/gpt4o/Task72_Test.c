#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <unistd.h> // pipe, dup2, read, close

void handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

void encrypt_data(const char *data, const char *key, unsigned char *encrypted_data, int *encrypted_data_len) {
    unsigned char iv[AES_BLOCK_SIZE] = {0};  // Static IV
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int outlen, finallen;

    if (!EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, (unsigned char *)key, iv))
        handleErrors();

    if (!EVP_EncryptUpdate(ctx, encrypted_data, &outlen, (unsigned char *)data, strlen(data)))
        handleErrors();

    if (!EVP_EncryptFinal_ex(ctx, encrypted_data + outlen, &finallen))
        handleErrors();

    *encrypted_data_len = outlen + finallen;
    EVP_CIPHER_CTX_free(ctx);
}

void program_main(int argc, char* argv[]) {
    const char *key = "sixteen byte key";
    const char *data = "Hello, World!";
    unsigned char encrypted_data[128];
    int encrypted_data_len;

    encrypt_data(data, key, encrypted_data, &encrypted_data_len);

    for(int i = 0; i < encrypted_data_len; i++)
        printf("%02x", encrypted_data[i]);
    printf("\n");
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
