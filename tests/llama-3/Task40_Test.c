#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define MAX_OUTPUT 8192

// --- Your CGI code unchanged ---
int cgi_main() {
    char csrfToken[] = "your_csrf_token_here";
    char method[10] = {0};

    if (getenv("REQUEST_METHOD") != NULL) {
        strcpy(method, getenv("REQUEST_METHOD"));
    }

    if (strcmp(method, "POST") == 0) {
        // Your code only checks CSRF token presence in env variable
        char csrfTokenForm[100] = {0};
        if (getenv("csrf_token") != NULL) {
            strcpy(csrfTokenForm, getenv("csrf_token"));
        }
        if (strcmp(csrfToken, csrfTokenForm) != 0) {
            printf("Content-Type: text/html");
            printf("<html><body>CSRF token validation failed</body></html>");
            return 0;
        }
        // Your code always prints success, but does not validate username/email content
        printf("Content-Type: text/html");
        printf("<html><body>Settings updated successfully!</body></html>");
    } else {
        // GET returns form with CSRF token hidden input
        printf("Content-Type: text/html\n\n");
        printf("<html><body>");
        printf("<form method='POST'>");
        printf("<input type='hidden' name='csrf_token' value='%s'>", csrfToken);
        printf("Username: <input type='text' name='username'><br>");
        printf("Email: <input type='email' name='email'><br>");
        printf("<input type='submit' value='Update Settings'>");
        printf("</form>");
        printf("</body></html>");
    }
    return 0;
}

// --- Helper to run cgi_main with environment set and capture output ---
int run_cgi_with_env(const char* method, const char* csrf_env, char* output, size_t max_output) {
    int pipe_out[2];
    if (pipe(pipe_out) == -1) return 1;

    pid_t pid = fork();
    if (pid < 0) return 1;

    if (pid == 0) {
        // child
        if (method)
            setenv("REQUEST_METHOD", method, 1);
        else
            unsetenv("REQUEST_METHOD");

        if (csrf_env)
            setenv("csrf_token", csrf_env, 1);
        else
            unsetenv("csrf_token");

        close(pipe_out[0]);
        dup2(pipe_out[1], STDOUT_FILENO);
        close(pipe_out[1]);

        cgi_main();

        exit(0);
    } else {
        // parent
        close(pipe_out[1]);

        ssize_t n, total = 0;
        while ((n = read(pipe_out[0], output + total, max_output - total - 1)) > 0) {
            total += n;
            if (total >= max_output - 1) break;
        }
        output[total] = '\0';

        close(pipe_out[0]);

        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
            return 0;
        else
            return 1;
    }
}

// --- Full test cases exactly as your original list ---

void run_tests() {
    struct {
        const char* method;
        const char* post_data;
        const char* expected_fragment;
        const char* description;
    } tests[] = {
        {"GET", NULL, "<form method='POST'>", "Test 1: GET returns form"},
        {"POST", "csrf_token=your_csrf_token_here&username=alice&email=a@example.com", "Settings updated successfully!", "Test 2: POST with valid CSRF"},
        {"POST", "username=alice&email=a@example.com", "CSRF token validation failed", "Test 3: POST missing CSRF"},
        {"POST", "csrf_token=wrong_token&username=alice&email=a@example.com", "CSRF token validation failed", "Test 4: POST invalid CSRF"},
        {"POST", "csrf_token=your_csrf_token_here", "Settings updated successfully!", "Test 5: POST with valid CSRF only, no username/email"},
        {"POST", "csrf_token=your_csrf_token_here&username=&email=", "Settings updated successfully!", "Test 6: POST with empty username and email but valid CSRF"},
        {"POST", "", "CSRF token validation failed", "Test 7: POST with empty body"},
        {"POST", "random=data&another=thing", "CSRF token validation failed", "Test 8: POST with unrelated data no CSRF"},
        {"GET", "csrf_token=your_csrf_token_here", "<form method='POST'>", "Test 9: GET with irrelevant data still returns form"},
    };

    char output[MAX_OUTPUT];
    int n = sizeof(tests) / sizeof(tests[0]);

    for (int i=0; i<n; i++) {
        // Since your CGI reads csrf_token only from env var,
        // we simulate setting/unsetting it only if it exists in post_data

        // Detect csrf_token presence and value in post_data string:
        const char* csrf_env = NULL;

        if (tests[i].post_data) {
            // simple extraction: look for "csrf_token=" in post_data
            const char* p = strstr(tests[i].post_data, "csrf_token=");
            if (p) {
                p += strlen("csrf_token=");
                // read until & or end
                static char tokenval[256];
                int j = 0;
                while (*p && *p != '&' && j < 255) {
                    tokenval[j++] = *p++;
                }
                tokenval[j] = 0;
                csrf_env = tokenval;
            }
        }

        int ret = run_cgi_with_env(tests[i].method, csrf_env, output, sizeof(output));

        printf("%s... ", tests[i].description);

        // Now check if output contains expected fragment:
        int contains = strstr(output, tests[i].expected_fragment) != NULL;

        // For tests 3,7,8 your code implements CSRF check by env var correctly,
        // For tests 2,5,6 your code always returns success if CSRF env matches,
        // For test 1,9 code returns form with correct html,
        // However your code DOES NOT parse username/email nor handle empty username/email cases.
        // So for test 6 (empty username/email) and test 7 (empty body) we must mark fail not implemented.
        // For test 8 (random data no CSRF), code correctly fails CSRF.

        int not_implemented_fail = 0;
        if (i == 5) { // test 6: empty username/email check — not implemented (code does not parse)
            not_implemented_fail = 1;
        }
        if (i == 6) { // test 7: empty body (empty POST) — code uses env var only, so fails CSRF check, this is implemented
            not_implemented_fail = 0;
        }

        if (contains) {
            if (not_implemented_fail) {
                printf("FAIL not implemented\n");
            } else {
                printf("PASS\n");
            }
        } else {
            printf("FAIL\nExpected fragment: %s\nGot output:\n%s\n", tests[i].expected_fragment, output);
        }
    }
}

int main() {
    run_tests();
    return 0;
}
