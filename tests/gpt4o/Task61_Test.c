#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

const char* get_root_element(const char* xmlString) {
    xmlDoc *document = xmlReadMemory(xmlString, strlen(xmlString), "noname.xml", NULL, 0);
    if (document == NULL) {
        return NULL; // خطأ في التحليل
    }

    xmlNode *root = xmlDocGetRootElement(document);
    if (root == NULL) {
        xmlFreeDoc(document);
        return NULL; // مستند فارغ
    }

    const char* rootName = (const char*)root->name;
    xmlFreeDoc(document);
    return rootName;
}

int main() {
    struct {
        const char* xml;
        const char* expected;
        const char* test_name;
    } tests[] = {
        {"<root><child1>Content</child1><child2 attr=\"value\"/></root>", "root", "test_basic_xml"},
        {"<single/>", "single", "test_single_element_xml"},
        {"<parent><child><subchild/></child></parent>", "parent", "test_nested_elements"},
        {"<empty></empty>", "empty", "test_empty_document"},
        {"<selfclosing attr=\"value\"/>", "selfclosing", "test_self_closing_tag"},
        {"<root attr=\"value\"><child/></root>", "root", "test_xml_with_attributes"},
        {"<root1/><root2/>", NULL, "test_multiple_root_elements"},
        {NULL, NULL, "test_large_xml"}, // يتم بناءه أدناه
        {"<root><child></root>", NULL, "test_malformed_xml"},
        {"<ns:root xmlns:ns=\"http://example.com\"><ns:child/></ns:root>", "{http://example.com}root", "test_xml_with_namespaces"}
    };

    // بناء نص XML كبير
    static char large_xml[60000];
    strcpy(large_xml, "<root>");
    for (int i = 0; i < 1000; i++) {
        strcat(large_xml, "<child>");
    }
    for (int i = 0; i < 1000; i++) {
        strcat(large_xml, "</child>");
    }
    strcat(large_xml, "</root>");
    tests[7].xml = large_xml;
    tests[7].expected = "root";

    int pass = 0, fail = 0;
    int total_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < total_tests; i++) {
        const char* rootName = get_root_element(tests[i].xml);
        if (tests[i].expected == NULL) {
            if (rootName == NULL) {
                printf("Test %d (%s): PASS\n", i + 1, tests[i].test_name);
                pass++;
            } else {
                printf("Test %d (%s): FAIL (Expected parse error but got: %s)\n", i + 1, tests[i].test_name, rootName);
                fail++;
            }
        } else {
            if (rootName != NULL && strcmp(rootName, tests[i].expected) == 0) {
                printf("Test %d (%s): PASS\n", i + 1, tests[i].test_name);
                pass++;
            } else {
                printf("Test %d (%s): FAIL (Expected: %s, Got: %s)\n", i + 1, tests[i].test_name,
                       tests[i].expected, rootName ? rootName : "NULL");
                fail++;
            }
        }
    }

    printf("\nTotal Passed: %d\nTotal Failed: %d\n", pass, fail);
    return 0;
}
