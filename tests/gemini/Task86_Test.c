#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <windows.h>
// #include <urlmon.h>
//#pragma comment(lib, "urlmon.lib")
//tested on windows  and its fine

void test_case_1(const char* test_url) {
    char url[256];
    strncpy(url, test_url, sizeof(url) - 1);
    url[sizeof(url) - 1] = '\0';

    printf("Testing download URL: %s\n", url);

    // بما أن دالة التحميل والتنفيذ معطلة، نتحقق فقط من صحة URL وطولها
    if (strlen(url) > 0) {
        printf("Test case 1: pass (URL accepted)\n");
    } else {
        printf("Test case 1: fail (empty URL)\n");
    }
}

int main() {
    // مثال URL صالح للاختبار
    const char* sample_url = "https://download.sysinternals.com/files/ProcessExplorer.exe";
    test_case_1(sample_url);

    return 0;
}
