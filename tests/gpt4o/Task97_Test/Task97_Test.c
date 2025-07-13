#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <unistd.h>
#include <fcntl.h>

void printNode(xmlNode *node) {
    xmlBufferPtr buffer = xmlBufferCreate();
    xmlNodeDump(buffer, node->doc, node, 0, 1);
    printf("%s\n", (const char*)buffer->content);
    xmlBufferFree(buffer);
}

int execute_xpath(const char* xpathExpr, const char* xmlFile) {
    xmlDocPtr doc = xmlParseFile(xmlFile);
    if (doc == NULL) {
        fprintf(stderr, "Error: unable to parse file %s\n", xmlFile);
        return -1;
    }

    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (xpathCtx == NULL) {
        fprintf(stderr, "Error: unable to create new XPath context\n");
        xmlFreeDoc(doc);
        return -1;
    }

    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*) xpathExpr, xpathCtx);
    if (xpathObj == NULL) {
        fprintf(stderr, "Error: unable to evaluate xpath expression %s\n", xpathExpr);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return -1;
    }

    xmlNodeSetPtr nodes = xpathObj->nodesetval;
    if (xmlXPathNodeSetIsEmpty(nodes)){
        printf("No result found for the XPath query.\n");
    } else {
        for (int i = 0; i < nodes->nodeNr; ++i) {
            printNode(nodes->nodeTab[i]);
        }
    }

    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);
    return 0;
}

// قراءة ملف نص كامل في string
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

// التقاط المخرجات (stdout/stderr) عند استدعاء دالة
void capture_output(int (*func)(const char*, const char*), const char* xpathExpr, const char* xmlFile,
                    int fd_to_capture, char* buffer, size_t bufsize) {
    int pipefd[2];
    pipe(pipefd);

    int saved_fd = dup(fd_to_capture);
    dup2(pipefd[1], fd_to_capture);
    close(pipefd[1]);

    func(xpathExpr, xmlFile);

    fflush(stdout);
    fflush(stderr);

    dup2(saved_fd, fd_to_capture);
    close(saved_fd);

    ssize_t n = read(pipefd[0], buffer, bufsize - 1);
    if (n >= 0) buffer[n] = '\0'; else buffer[0] = '\0';
    close(pipefd[0]);
}

void run_tests(int argc, char **argv) {
    const size_t BUF_SIZE = 16384;
    char output[BUF_SIZE];
    int all_pass = 1;

    fprintf(stderr, "Running test cases...\n");

    // Test 1: missing args
    fprintf(stderr, "Test 1 (missing args): ");
    if (argc != 3) {
        fprintf(stderr, "PASS\n");
    } else {
        fprintf(stderr, "FAIL\n");
        all_pass = 0;
    }

    // إعداد ملف xml للاختبار
    const char* test_xml_path = "test.xml";
    const char* test_xml_content =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<library>\n"
        "  <book id=\"1\">Test Book 1</book>\n"
        "  <book id=\"2\">Test Book 2</book>\n"
        "  <book id=\"3\">Test Book 3</book>\n"
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
    capture_output(execute_xpath, "/library/book", "no_such_file.xml", 2, output, BUF_SIZE);
    if (strstr(output, "Error: unable to parse file") != NULL) {
        fprintf(stderr, "PASS\n");
    } else {
        fprintf(stderr, "FAIL\nOutput was:\n%s\n", output);
        all_pass = 0;
    }

    // Test 3: valid xml + xpath (should print nodes)
    fprintf(stderr, "Test 3 (valid XML + xpath): ");
    capture_output(execute_xpath, "/library/book[@id='1']", test_xml_path, 1, output, BUF_SIZE);
    // نتوقع طباعة عنصر book id=1 كامل، مثلاً:
    // <book id="1">Test Book 1</book>
    if (strstr(output, "<book") && strstr(output, "Test Book 1")) {
        fprintf(stderr, "PASS\n");
    } else {
        fprintf(stderr, "FAIL\nOutput was:\n%s\n", output);
        all_pass = 0;
    }

    // Test 4: valid xml + xpath no result
    fprintf(stderr, "Test 4 (valid XML + xpath no results): ");
    capture_output(execute_xpath, "/library/book[@id='999']", test_xml_path, 1, output, BUF_SIZE);
    if (strstr(output, "No result found for the XPath query.") != NULL) {
        fprintf(stderr, "PASS\n");
    } else {
        fprintf(stderr, "FAIL\nOutput was:\n%s\n", output);
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
    if (argc == 3) {
        return execute_xpath(argv[1], argv[2]);
    } else {
        fprintf(stderr, "Usage: %s <xpath> <xmlfile>\n", argv[0]);
        run_tests(argc, argv);
        return 1;
    }
}
