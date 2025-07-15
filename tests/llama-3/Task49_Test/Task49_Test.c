#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_OUTPUT 8192

// Your original console program unchanged, wrapped for test
int add_user_to_file(const char* name, const char* email, char* output, size_t max_output) {
    if (!name || !email) {
        snprintf(output, max_output, "Name and email required");
        return 1;
    }

    FILE *file = fopen("users.txt", "a");
    if (!file) {
        snprintf(output, max_output, "Failed to open file");
        return 1;
    }

    fprintf(file, "%s,%s\n", name, email);
    fclose(file);

    snprintf(output, max_output, "User created successfully!");
    return 0;
}

// Since the program is not an API endpoint, all API test cases are "not implemented"
void run_tests() {
    struct {
        const char* name;
        const char* email;
        const char* expected_output;
        int expect_success;
        const char* description;
    } tests[] = {
        {"John Doe", "john.doe2@example.com", "User created successfully!", 1, "Test 1: Valid user"},
        {"Jane Doe", "jane.doe@example.com", "User created successfully!", 1, "Test 2: Another valid user"},
        {"John Doe", "john.doe2@example.com", "User created successfully!", 1, "Test 3: Duplicate email"},
        {"", "no.name@example.com", "User created successfully!", 1, "Test 4: Empty name"},
        {"No Email", "", "User created successfully!", 1, "Test 5: Empty email"},
        {NULL, NULL, "Name and email required", 0, "Test 6: NULL values"},
    };

    for (int i=0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        printf("%s... ", tests[i].description);
        // This program does not implement any API endpoint
        printf("FAIL not implemented\n");
    }
}

int main() {
    run_tests();
    return 0;
}
