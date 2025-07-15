#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

// دالة استخراج اسم العنصر الجذر
char* get_root_element(const char* xmlString) {
    xmlDocPtr doc = xmlParseMemory(xmlString, strlen(xmlString));
    if (doc == NULL) {
        return NULL; // خطأ في التحليل
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);
    if (root == NULL) {
        xmlFreeDoc(doc);
        return NULL; // لا يوجد عنصر جذر
    }

    char* rootName = (char*)xmlStrdup(root->name);
    xmlFreeDoc(doc);
    return rootName;
}

// دالة مساعدة للمقارنة وطباعة النتيجة
void run_test(const char* test_name, const char* xml_input, const char* expected_root, int expect_fail) {
    char* root_element = get_root_element(xml_input);
    int pass = 0;

    if (expect_fail) {
        // نتوقع خطأ (NULL)
        pass = (root_element == NULL);
    } else {
        // نتوقع اسم جذر مطابق
        if (root_element != NULL && strcmp(root_element, expected_root) == 0) {
            pass = 1;
        }
    }

    printf("Test %s: %s\n", test_name, pass ? "PASS" : "FAIL");

    if (root_element != NULL) {
        xmlFree(root_element);
    }
}

int main() {
    // التست كيسز حسب المطلوب

    run_test("basic_xml",
             "<root><child1>Content</child1><child2 attr=\"value\"/></root>",
             "root", 0);

    run_test("single_element_xml",
             "<single/>",
             "single", 0);

    run_test("nested_elements",
             "<parent><child><subchild/></child></parent>",
             "parent", 0);

    run_test("empty_document",
             "<empty></empty>",
             "empty", 0);

    run_test("self_closing_tag",
             "<selfclosing attr=\"value\"/>",
             "selfclosing", 0);

    run_test("xml_with_attributes",
             "<root attr=\"value\"><child/></root>",
             "root", 0);

    run_test("multiple_root_elements",
             "<root1/><root2/>",
             NULL, 1); // نتوقع خطأ

    // بناء نص XML كبير برمجياً
    {
        char large_xml[60000] = "<root>";
        for (int i=0; i<1000; i++) {
            strcat(large_xml, "<child>");
        }
        for (int i=0; i<1000; i++) {
            strcat(large_xml, "</child>");
        }
        strcat(large_xml, "</root>");
        run_test("large_xml_fixed", large_xml, "root", 0);
    }

    run_test("malformed_xml",
             "<root><child></root>",
             NULL, 1); // نتوقع خطأ

    // ملاحظة: libxml2 ترجع اسم العنصر مع أو بدون namespace حسب التحليل، نستخدم الاسم فقط "root" هنا.
    run_test("xml_with_namespaces",
             "<ns:root xmlns:ns=\"http://example.com\"><ns:child/></ns:root>",
             "root", 0);

    return 0;
}
