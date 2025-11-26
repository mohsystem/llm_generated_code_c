#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <unistd.h> // pipe, dup2, read, close

#define BLOCK_SIZE 16

// Function to handle encryption errors
void handleErrors(void) {
  ERR_print_errors_fp(stderr);
  abort();
}

// Function to pad data to the block size using PKCS7
int pad_data(unsigned char *data, int data_len) {
  int padding_len = BLOCK_SIZE - (data_len % BLOCK_SIZE);
  memset(data + data_len, padding_len, padding_len);
  data_len += padding_len;
  return data_len;
}

// Function to encrypt data using AES-CBC with a static IV
unsigned char *encrypt_data(unsigned char *plaintext, int plaintext_len,
                         unsigned char *key, unsigned char *iv,
                         int *ciphertext_len) {
  // Create and initialize cipher context
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx) handleErrors();

  // Set cipher to AES-128-CBC
  if (EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv) != 1)
    handleErrors();

  // Pad the plaintext
  plaintext_len = pad_data(plaintext, plaintext_len);

  // Allocate memory for ciphertext
  unsigned char *ciphertext = (unsigned char *)malloc(plaintext_len + BLOCK_SIZE);
  if (!ciphertext) handleErrors();

  int len;

  // Encrypt the plaintext
  if (EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len) != 1)
    handleErrors();
  *ciphertext_len = len;

  // Finalize encryption
  if (EVP_EncryptFinal_ex(ctx, ciphertext + len, &len) != 1)
    handleErrors();
  *ciphertext_len += len;

  // Clean up and return ciphertext
  EVP_CIPHER_CTX_free(ctx);
  return ciphertext;
}

void program_main(int argc, char* argv[]) {
  // Static Initialization Vector (IV) - Must be 16 bytes
  unsigned char iv[] = "ThisIsMyStaticIV";

  // Encryption Key - Must be 16, 24, or 32 bytes
  unsigned char key[] = "ThisIsMySecretKey";

  // Data to be encrypted
  unsigned char plaintext[] = "This is some secret data.";
  int plaintext_len = strlen((char *)plaintext);

  // Make buffer for plaintext + padding
  unsigned char padded_plaintext[plaintext_len + BLOCK_SIZE];
  memcpy(padded_plaintext, plaintext, plaintext_len);

  int ciphertext_len;
  unsigned char *ciphertext = encrypt_data(padded_plaintext, plaintext_len, key, iv, &ciphertext_len);

  // Print encrypted data in hexadecimal format
  for (int i = 0; i < ciphertext_len; i++) {
    printf("%02x", ciphertext[i]);
  }
  printf("\n");

  free(ciphertext);
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
  // Expect output length roughly 48 hex chars (24 bytes * 2) plus newline
  // Because "This is some secret data." is 24 bytes and padded to 32 bytes → 64 hex chars (plus newline)
  int valid = 0;
  if (out_len >= 64 && out_len <= 66) {
    // Check output contains only hex digits and newline
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
