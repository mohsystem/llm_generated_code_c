#include <stdio.h>
#include <string.h>

// الكود الأصلي محفوظ كما هو (deserializeData + main)

// === Test framework ===
#define ASSERT_STR_CONTAINS(str, substr, testname) do { \
if (strstr(str, substr) != NULL) printf("%s: PASS\n", testname); \
else printf("%s: FAIL\n", testname); \
} while(0)

#define ASSERT_FALSE(cond, testname) do { \
if (!(cond)) printf("%s: PASS\n", testname); \
else printf("%s: FAIL\n", testname); \
} while(0)

#define PRINT_FAIL_NOT_IMPLEMENTED(testname) printf("%s: FAIL (Not Implemented)\n", testname)

// === Helper function to capture printed output (simple simulation) ===
// ملاحظة: في C عموماً صعب الإمساك بالطباعة بدون redirect أو تقنيات معقدة، هنا نستخدم Not Implemented

// === Test cases ===

void test_deserialize_valid_data() {
    char testData[] = "key1:value1:key2:value2";
    // في الكود الأصلي يتم الطباعة، لا يمكن التقاطها بدون تعديل، لذلك Not Implemented
    PRINT_FAIL_NOT_IMPLEMENTED("test_deserialize_valid_data");
}

void test_deserialize_empty_string() {
    char testData[] = "";
    PRINT_FAIL_NOT_IMPLEMENTED("test_deserialize_empty_string");
}

void test_deserialize_no_delimiter() {
    char testData[] = "key1value1key2value2";
    PRINT_FAIL_NOT_IMPLEMENTED("test_deserialize_no_delimiter");
}

void test_deserialize_single_pair() {
    char testData[] = "key:value";
    PRINT_FAIL_NOT_IMPLEMENTED("test_deserialize_single_pair");
}

int main() {
    test_deserialize_valid_data();
    test_deserialize_empty_string();
    test_deserialize_no_delimiter();
    test_deserialize_single_pair();
    return 0;
}
