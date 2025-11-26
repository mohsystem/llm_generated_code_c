#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <unistd.h>
#include <fcntl.h>

void print_node_info(xmlNodePtr node) {
    if (node) {
        printf("Node Name: %s\n", node->name);
        xmlAttrPtr attr = node->properties;
        while (attr) {
            printf("  Attribute: %s, Value: %s\n", attr->name, attr->children->content);
            attr = attr->next;
        }
    }
}

int execute_xpath_with_id(const char* xpath_base, const char* xml_file, const char* id_value) {
    xmlDocPtr doc = xmlParseFile(xml_file);
    if (doc == NULL) {
        fprintf(stderr, "Error parsing XML file.\n");
        return 1;
    }

    xmlXPathContextPtr context = xmlXPathNewContext(doc);
    if (context == NULL) {
        fprintf(stderr, "Error creating XPath context.\n");
        xmlFreeDoc(doc);
        return 1;
    }

    char full_xpath[256];
    snprintf(full_xpath, sizeof(full_xpath), "%s[@id='%s']", xpath_base, id_value);

    xmlXPathObjectPtr result = xmlXPathEvalExpression((const xmlChar*)full_xpath, context);
    if (result == NULL) {
        fprintf(stderr, "Error evaluating XPath expression.\n");
        xmlXPathFreeContext(context);
        xmlFreeDoc(doc);
        return 1;
    }

    if (result->nodesetval) {
        int node_count = xmlXPathNodeSetGetLength(result->nodesetval);
        if (node_count == 0) {
            // لا نطبع شيء إذا لا نتائج (يمكن طباعة رسالة لو تريد)
        }
        for (int i = 0; i < node_count; ++i) {
            print_node_info(result->nodesetval->nodeTab[i]);
        }
    }

    xmlXPathFreeObject(result);
    xmlXPathFreeContext(context);
    xmlFreeDoc(doc);
    return 0;
}

// مساعدة قراءة ملف نصي كامل في string
char* read_file_to_string(const char* filepath) {
    FILE* f = fopen(filepath, "r");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* buffer = malloc(len + 1);
    if (!buffer) {
        fclose(f);
        return NULL;
    }
    fread(buffer, 1, len, f);
    buffer[len] = '\0';
    fclose(f);
    return buffer;
}

// التقاط المخرجات (stdout/stderr) عند استدعاء الدالة
void capture_output(int (*func)(const char*, const char*, const char*),
                    const char* xpath_base, const char* xml_file, const char* id_value,
                    int fd_to_capture, char* buffer, size_t bufsize) {
    int pipefd[2];
    pipe(pipefd);

    int saved_fd = dup(fd_to_capture);
    dup2(pipefd[1], fd_to_capture);
    close(pipefd[1]);

    func(xpath_base, xml_file, id_value);

    fflush(stdout);
    fflush(stderr);

    dup2(saved_fd, fd_to_capture);
    close(saved_fd);

    ssize_t n = read(pipefd[0], buffer, bufsize - 1);
    if (n >= 0) buffer[n] = '\0'; else buffer[0] = '\0';
    close(pipefd[0]);
}

void run_tests(int argc, char **argv) {
    const size_t BUF_SIZE = 8192;
    char output[BUF_SIZE];
    int all_pass = 1;

    fprintf(stderr, "Running tests...\n");

    // Test 1: missing args
    fprintf(stderr, "Test 1 (missing args): ");
    if (argc != 4) {
        fprintf(stderr, "PASS\n");
    } else {
        fprintf(stderr, "FAIL\n");
        all_pass = 0;
    }

    // تجهيز ملف xml للاختبارات
    const char* test_xml_path = "test.xml";
    const char* test_xml_content =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<library>\n"
        "  <book id=\"1\" author=\"Author One\" year=\"2000\"/>\n"
        "  <book id=\"2\" author=\"Author Two\" year=\"2010\"/>\n"
        "  <book id=\"3\" author=\"Author Three\" year=\"2020\"/>\n"
        "</library>\n";

    FILE* f = fopen(test_xml_path, "w");
    if (!f) {
        fprintf(stderr, "Cannot create test XML file\n");
        exit(1);
    }
    fputs(test_xml_content, f);
    fclose(f);

    // Test 2: non-existent file
    fprintf(stderr, "Test 2 (non-existent XML file): ");
    capture_output(execute_xpath_with_id, "/library/book", "no_such_file.xml", "1", 2, output, BUF_SIZE);
    if (strstr(output, "Error parsing XML file.") != NULL) {
        fprintf(stderr, "PASS\n");
    } else {
        fprintf(stderr, "FAIL\nOutput was:\n%s\n", output);
        all_pass = 0;
    }

    // Test 3: valid XML + existing id
    fprintf(stderr, "Test 3 (valid XML + existing id): ");
    capture_output(execute_xpath_with_id, "/library/book", test_xml_path, "1", 1, output, BUF_SIZE);
    // نتوقع طباعة اسم العقدة وصفاتها لـ book id=1:
    // Node Name: book
    //   Attribute: id, Value: 1
    //   Attribute: author, Value: Author One
    //   Attribute: year, Value: 2000
    if (strstr(output, "Node Name: book") && strstr(output, "Attribute: id") &&
        strstr(output, "Attribute: author") && strstr(output, "Attribute: year")) {
        fprintf(stderr, "PASS\n");
    } else {
        fprintf(stderr, "FAIL\nOutput was:\n%s\n", output);
        all_pass = 0;
    }

    // Test 4: valid XML + non-existing id
    fprintf(stderr, "Test 4 (valid XML + non-existing id): ");
    capture_output(execute_xpath_with_id, "/library/book", test_xml_path, "999", 1, output, BUF_SIZE);
    if (strlen(output) == 0) {
        fprintf(stderr, "PASS\n");
    } else {
        fprintf(stderr, "FAIL\nExpected no output.\nGot:\n%s\n", output);
        all_pass = 0;
    }

    remove(test_xml_path);

    if (all_pass) {
        fprintf(stderr, "All tests PASSED.\n");
    } else {
        fprintf(stderr, "Some tests FAILED.\n");
    }
}

int main(int argc, char **argv) {
    if (argc == 4) {
        return execute_xpath_with_id(argv[1], argv[2], argv[3]);
    } else {
        fprintf(stderr, "Usage: %s <xpath_base> <xml_file> <id_value>\n", argv[0]);
        run_tests(argc, argv);
        return 1;
    }
}
