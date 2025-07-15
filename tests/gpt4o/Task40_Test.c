#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_OUTPUT 8192

// --- كود CGI كما هو (دون تعديل) ---

void print_html_form() {
    printf("Content-Type: text/html\n\n");
    printf("<html><body>");
    printf("<form method='POST'>");
    printf("<input type='hidden' name='csrf_token' value='your_csrf_token_here'>");
    printf("Username: <input type='text' name='username'><br>");
    printf("Email: <input type='email' name='email'><br>");
    printf("<input type='submit' value='Update Settings'>");
    printf("</form>");
    printf("</body></html>");
}

int cgi_main() {
    char *method = getenv("REQUEST_METHOD");
    if (method && strcmp(method, "POST") == 0) {
        char input[MAX_INPUT];
        fgets(input, sizeof(input), stdin);
        if (strstr(input, "csrf_token=your_csrf_token_here") == NULL) {
            printf("Content-Type: text/html");
            printf("<html><body>CSRF token validation failed</body></html>");
            return 0;
        }
        printf("Content-Type: text/html");
        printf("<html><body>Settings updated successfully!</body></html>");
    } else {
        print_html_form();
    }
    return 0;
}

// --- دالة مساعدة لتشغيل cgi_main مع بيئة REQUEST_METHOD وإدخال POST data، والتقاط الإخراج ---

int run_cgi_with_method_and_data(const char* method, const char* post_data, char* output, size_t max_output) {
    int pipe_in[2];   // لتمرير البيانات إلى stdin للـ CGI
    int pipe_out[2];  // لقراءة إخراج CGI

    if (pipe(pipe_in) == -1) return 1;
    if (pipe(pipe_out) == -1) return 1;

    pid_t pid = fork();
    if (pid < 0) return 1;
    if (pid == 0) {
        // طفل
        close(pipe_in[1]);   // أغلق طرف الكتابة للـ stdin
        dup2(pipe_in[0], STDIN_FILENO);
        close(pipe_in[0]);

        close(pipe_out[0]);  // أغلق طرف القراءة للـ stdout
        dup2(pipe_out[1], STDOUT_FILENO);
        close(pipe_out[1]);

        setenv("REQUEST_METHOD", method, 1);

        // نفذ cgi_main مباشرة
        cgi_main();

        exit(0);
    } else {
        // أب
        close(pipe_in[0]);   // أغلق طرف القراءة للـ stdin
        if (post_data) {
            write(pipe_in[1], post_data, strlen(post_data));
        }
        close(pipe_in[1]);   // أنهي الإدخال

        close(pipe_out[1]);  // أغلق طرف الكتابة للـ stdout

        ssize_t n;
        size_t total_read = 0;
        while ((n = read(pipe_out[0], output + total_read, max_output - total_read -1)) > 0) {
            total_read += n;
            if (total_read >= max_output - 1) break;
        }
        output[total_read] = '\0';

        close(pipe_out[0]);

        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            return 0;
        } else {
            return 1;
        }
    }
}

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
        int ret = run_cgi_with_method_and_data(tests[i].method, tests[i].post_data, output, sizeof(output));
        printf("%s... ", tests[i].description);
        if (ret == 0 && strstr(output, tests[i].expected_fragment) != NULL) {
            printf("PASS\n");
        } else {
            printf("FAIL\nExpected fragment: %s\nGot output:\n%s\n", tests[i].expected_fragment, output);
        }
    }
}

int main() {
    run_tests();
    return 0;
}
