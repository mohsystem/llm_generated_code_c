#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    char *input_file = argv[1];
    char temp_file[] = "temp_file.txt";

    FILE *fp_in, *fp_out;

    // Open input file
    fp_in = fopen(input_file, "rb");
    if (fp_in == NULL) {
        fprintf(stderr, "Error opening input file: %s\n", strerror(errno));
        return 1;
    }

    // Create temporary file
    fp_out = fopen(temp_file, "wb");
    if (fp_out == NULL) {
        fprintf(stderr, "Error creating temporary file: %s\n", strerror(errno));
        fclose(fp_in);
        return 1;
    }

    // Copy data from input file to temporary file
    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fp_in)) > 0) {
        if (fwrite(buffer, 1, bytes_read, fp_out) != bytes_read) {
            fprintf(stderr, "Error writing to temporary file\n");
            fclose(fp_in);
            fclose(fp_out);
            remove(temp_file); // Delete partially written temp file
            return 1;
        }
    }

    // Check for errors during reading
    if (ferror(fp_in)) {
        fprintf(stderr, "Error reading from input file\n");
        fclose(fp_in);
        fclose(fp_out);
        remove(temp_file); // Delete partially written temp file
        return 1;
    }

    // Close files
    fclose(fp_in);
    fclose(fp_out);

    printf("File copied successfully to %s\n", temp_file);

    return 0;
}
