#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>

const char* get_root_element(const char* xml_string) {
    xmlDocPtr doc = xmlReadMemory(xml_string, strlen(xml_string), "noname.xml", NULL, 0);
    if (doc == NULL) {
        return NULL; // إشارة إلى خطأ في التحليل
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);
    if (root == NULL) {
        xmlFreeDoc(doc);
        return NULL; // مستند فارغ
    }

    const char* root_name = (const char*)root->name;
    xmlFreeDoc(doc);
    return root_name;
}

int main() {
    struct {
        const char* xml_str;
        const char* expected_root; // NULL يعني خطأ أو استثناء متوقع
        const char* test_name;
    } tests[] = {
        {"<root><child1>Content</child1><child2 attr=\"value\"/></root>", "root", "test_basic_xml"},
        {"<single/>", "single", "test_single_element_xml"},
        {"<parent><child><subchild/></child></parent>", "parent", "test_nested_elements"},
        {"<empty></empty>", "empty", "test_empty_document"},
        {"<selfclosing attr=\"value\"/>", "selfclosing", "test_self_closing_tag"},
        {"<root attr=\"value\"><child/></root>", "root", "test_xml_with_attributes"},
        {"<root1/><root2/>", NULL, "test_multiple_root_elements"},
        {NULL, NULL, "test_large_xml"}, // سيتم بناء النص في الأسفل
        {"<root><child></root>", NULL, "test_malformed_xml"},
        {"<ns:root xmlns:ns=\"http://example.com\"><ns:child/></ns:root>", "{http://example.com}root", "test_xml_with_namespaces"}
    };

    // بناء نص large_xml
    char large_xml[50000];
    strcpy(large_xml, "<root>");
    for (int i = 0; i < 1000; i++) {
        strcat(large_xml, "<child>");
    }
    for (int i = 0; i < 1000; i++) {
        strcat(large_xml, "</child>");
    }
    strcat(large_xml, "</root>");
    tests[7].xml_str = large_xml;
    tests[7].expected_root = "root";

    int pass = 0, fail = 0;
    int total_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < total_tests; i++) {
        const char* root = get_root_element(tests[i].xml_str);
        if (tests[i].expected_root == NULL) {
            // نتوقع خطأ/استثناء
            if (root == NULL) {
                printf("Test %d (%s): PASS\n", i + 1, tests[i].test_name);
                pass++;
            } else {
                printf("Test %d (%s): FAIL (expected error but got root: %s)\n", i + 1, tests[i].test_name, root);
                fail++;
            }
        } else {
            // تحقق التطابق النصي
            if (root != NULL && strcmp(root, tests[i].expected_root) == 0) {
                printf("Test %d (%s): PASS\n", i + 1, tests[i].test_name);
                pass++;
            } else {
                printf("Test %d (%s): FAIL (expected root: %s, got: %s)\n", i + 1, tests[i].test_name,
                       tests[i].expected_root, root ? root : "NULL");
                fail++;
            }
        }
    }

    printf("\nTotal Passed: %d\nTotal Failed: %d\n", pass, fail);
    return 0;
}
