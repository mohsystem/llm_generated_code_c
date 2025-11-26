#include <stdio.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <unistd.h> // pipe, dup2, read, close

// PKCS7 padding function
int pkcs7_pad(const unsigned char *input, int input_len, unsigned char *output, int block_size) {
    int pad_len = block_size - (input_len % block_size);
    memcpy(output, input, input_len);
    for (int i = 0; i < pad_len; i++) {
        output[input_len + i] = (unsigned char)pad_len;
    }
    return input_len + pad_len;
}

void encrypt_data(const unsigned char *data, int data_len, const unsigned char *key, unsigned char *encrypted, int *encrypted_len) {
    // Create a static initialization vector (IV)
    unsigned char iv[AES_BLOCK_SIZE];
    memset(iv, 0, AES_BLOCK_SIZE);

    // Initialize AES key structure
    AES_KEY aes_key;
    AES_set_encrypt_key(key, 256, &aes_key);

    // Pad the input data with PKCS7
    int padded_size = ((data_len / AES_BLOCK_SIZE) + 1) * AES_BLOCK_SIZE;
    unsigned char padded_data[padded_size];
    int final_len = pkcs7_pad(data, data_len, padded_data, AES_BLOCK_SIZE);

    // Encrypt the padded data using AES CBC mode
    AES_cbc_encrypt(padded_data, encrypted, final_len, &aes_key, iv, AES_ENCRYPT);

    *encrypted_len = final_len;
}

void program_main(int argc, char* argv[]) {
    // For testing, fixed key and data are used similar to Python example
    unsigned char key[32];
    if (!RAND_bytes(key, sizeof(key))) {
        printf("Failed to generate key\n");
        return;
    }

    const unsigned char *data = (const unsigned char *)"This is a secret message.";
    int data_len = (int)strlen((const char *)data);

    int padded_size = ((data_len / AES_BLOCK_SIZE) + 1) * AES_BLOCK_SIZE;
    unsigned char encrypted[padded_size];
    int encrypted_len = 0;

    encrypt_data(data, data_len, key, encrypted, &encrypted_len);

    // Print encrypted data as hex
    for (int i = 0; i < encrypted_len; i++) {
        printf("%02x ", encrypted[i]);
    }
    printf("\n");
}

// Capture stdout for testing
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

// Since encryption output is random due to random key, tests verify output length & pattern

void test_output_length_and_format() {
    char out[1024];
    char err[1024];
    char* argv[] = {"program"};
    int argc = 1;

    if (capture_output(program_main, argc, argv, out, sizeof(out), err, sizeof(err)) != 0) {
        print_result("test_output_length_and_format", 0);
        return;
    }

    // Check output length: output should have multiples of 16 bytes printed as hex with space, e.g. "xx xx xx ... "
    // For data "This is a secret message." length 24, padded to 32 bytes → 32*3-1 = 95 chars approx (2 hex + space per byte minus last space)
    size_t out_len = strlen(out);
    if (out_len >= 90 && out_len <= 100) { // allow some margin
        // check output consists only of hex digits, spaces, and newline at end
        int valid = 1;
        for (size_t i = 0; i < out_len - 1; i++) {
            if (!((out[i] >= '0' && out[i] <= '9') ||
                  (out[i] >= 'a' && out[i] <= 'f') ||
                  out[i] == ' ')) {
                valid = 0;
                break;
            }
        }
        if (out[out_len - 1] != '\n') valid = 0;

        print_result("test_output_length_and_format", valid);
    } else {
        print_result("test_output_length_and_format", 0);
    }
}

int main() {
    test_output_length_and_format();
    return 0;
}
