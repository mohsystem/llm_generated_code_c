#include <stdio.h>
#include <string.h>

// محاكاة النتائج كما في التست كيسز البايثون:
typedef struct {
    int status_code;
    const char *response_text;
    int simulate_error; // 0=none, 1=generic error
} MockResponse;

MockResponse mock_request(const char *url) {
    if (strcmp(url, "https://www.google.com") == 0) {
        return (MockResponse){200, "Success", 0};
    } else if (strcmp(url, "https://www.bing.com") == 0) {
        return (MockResponse){200, "Success", 0};
    } else if (strcmp(url, "https://www.nonexistentwebsite.com") == 0) {
        return (MockResponse){0, NULL, 1}; // error
    } else if (strcmp(url, "https://www.example.com") == 0) {
        // سيتم اختبار عدة حالات على example.com حسب التست كيسز
        // هنا نفترض حالة 302 Redirect
        return (MockResponse){302, "Redirected", 0};
    } else if (strcmp(url, "https://www.largecontent.com") == 0) {
        static char large_content[10001];
        for(int i=0; i<10000; i++) large_content[i] = 'A';
        large_content[10000] = '\0';
        return (MockResponse){200, large_content, 0};
    } else if (strcmp(url, "https://www.timeout.com") == 0) {
        return (MockResponse){0, NULL, 1}; // error: timeout
    } else if (strcmp(url, "https://www.noresponse.com") == 0) {
        return (MockResponse){200, NULL, 0}; // null content
    } else if (strcmp(url, "https://www.partialcontent.com") == 0) {
        return (MockResponse){206, "Partial content", 0};
    }
    // افتراض error لأي url غير معروف
    return (MockResponse){0, NULL, 1};
}

const char* foo(const char* url) {
    MockResponse r = mock_request(url);
    if (r.simulate_error) {
        return "ERROR";
    }
    return r.response_text;
}

void run_tests() {
    struct {
        const char* url;
        const char* expected;
        const char* description;
    } tests[] = {
        {"https://www.google.com", "Success", "valid_url_google"},
        {"https://www.bing.com", "Success", "valid_url_bing"},
        {"https://www.nonexistentwebsite.com", "ERROR", "invalid_url"},
        {"https://www.example.com", "Redirected", "redirect"},
        {"https://www.largecontent.com", NULL, "large_response"}, // will compare length
        {"https://www.timeout.com", "ERROR", "timeout"},
        {"https://www.noresponse.com", NULL, "no_response"},
        {"https://www.partialcontent.com", "Partial content", "partial_content"},
    };

    for (int i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        const char* res = foo(tests[i].url);
        int pass = 0;
        if (tests[i].expected == NULL) {
            // cases with large response or NULL expected response
            if (res == NULL) {
                pass = 1;
            } else if (strcmp(tests[i].description, "large_response") == 0) {
                // تحقق من طول النص الكبير (10000 'A')
                if (strlen(res) == 10000) pass = 1;
            }
        } else {
            if (res != NULL && strcmp(res, tests[i].expected) == 0) pass = 1;
        }

        printf("Test %d (%s): %s\n", i+1, tests[i].description, pass ? "PASS" : "FAIL");
    }
}

int main() {
    run_tests();

    // لو حبيت تفاعل مع المستخدم حقيقي (curl حقيقي) تقدر تضيف هنا
    return 0;
}
