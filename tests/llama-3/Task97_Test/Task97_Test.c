#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <unistd.h>
#include <fcntl.h>

int run_xpath(const char* id_value, const char* xml_file) {
    xmlDocPtr doc = xmlReadFile(xml_file, NULL, 0);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse XML file\n");
        return 1;
    }

    xmlXPathContextPtr context = xmlXPathNewContext(doc);
    if (context == NULL) {
        fprintf(stderr, "Failed to create XPath context\n");
        xmlFreeDoc(doc);
        return 1;
    }

    char xpathExpr[256];
    snprintf(xpathExpr, sizeof(xpathExpr), "/tag[@id='%s']", id_value);

    xmlXPathObjectPtr result = xmlXPathEvalExpression((xmlChar *)xpathExpr, context);
    if (result == NULL) {
        fprintf(stderr, "Failed to evaluate XPath expression\n");
        xmlXPathFreeContext(context);
        xmlFreeDoc(doc);
        return 1;
    }

    xmlNodeSetPtr nodeset = result->nodesetval;
    int count = (nodeset) ? nodeset->nodeNr : 0;
    for (int i = 0; i < count; i++) {
        xmlNodePtr node = nodeset->nodeTab[i];
        if (node->content)
            printf("%s\n", node->content);
    }

    xmlXPathFreeObject(result);
    xmlXPathFreeContext(context);
    xmlFreeDoc(doc);
    return 0;
}

char* read_file(const char* path) {
    FILE* f = fopen(path, "r");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* buf = malloc(len + 1);
    if (!buf) {
        fclose(f);
        return NULL;
    }
    fread(buf, 1, len, f);
    buf[len] = '\0';
    fclose(f);
    return buf;
}

void capture_output(int (*func)(const char*, const char*), const char* id_value, const char* xml_file,
                    int fd_capture, char* buffer, size_t bufsize) {
    int pipefd[2];
    pipe(pipefd);

    int saved_fd = dup(fd_capture);
    dup2(pipefd[1], fd_capture);
    close(pipefd[1]);

    func(id_value, xml_file);

    fflush(stdout);
    fflush(stderr);

    dup2(saved_fd, fd_capture);
    close(saved_fd);

    ssize_t n = read(pipefd[0], buffer, bufsize - 1);
    if (n >= 0) buffer[n] = '\0'; else buffer[0] = '\0';
    close(pipefd[0]);
}

void run_tests(int argc, char** argv) {
    const size_t BUF_SIZE = 8192;
    char output[BUF_SIZE];
    int all_pass = 1;

    fprintf(stderr, "Running tests...\n");

    // Test 1: missing args
    fprintf(stderr, "Test 1 (missing args): ");
    if (argc != 3) {
        fprintf(stderr, "PASS\n");
    } else {
        fprintf(stderr, "FAIL\n");
        all_pass = 0;
    }

    // Prepare test XML file
    const char* test_xml_path = "test.xml";
    const char* test_xml_content =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<tag id=\"1\">First</tag>\n"
        "<tag id=\"2\">Second</tag>\n";

    FILE* f = fopen(test_xml_path, "w");
    if (!f) {
        fprintf(stderr, "Cannot create test XML file\n");
        exit(1);
    }
    fputs(test_xml_content, f);
    fclose(f);

    // Test 2: Non-existent file
    fprintf(stderr, "Test 2 (non-existent XML file): ");
    capture_output(run_xpath, "1", "no_such_file.xml", 2, output, BUF_SIZE);
    if (strstr(output, "Failed to parse XML file") != NULL) {
        fprintf(stderr, "PASS\n");
    } else {
        fprintf(stderr, "FAIL\nOutput was:\n%s\n", output);
        all_pass = 0;
    }

    // Test 3: valid XML + existing id
    fprintf(stderr, "Test 3 (valid XML + existing id): ");
    capture_output(run_xpath, "1", test_xml_path, 1, output, BUF_SIZE);
    if (strcmp(output, "First\n") == 0) {
        fprintf(stderr, "PASS\n");
    } else {
        fprintf(stderr, "FAIL\nExpected: First\nGot:\n%s\n", output);
        all_pass = 0;
    }

    // Test 4: valid XML + non-existing id
    fprintf(stderr, "Test 4 (valid XML + non-existing id): ");
    capture_output(run_xpath, "999", test_xml_path, 1, output, BUF_SIZE);
    if (strlen(output) == 0) {
        fprintf(stderr, "PASS\n");
    } else {
        fprintf(stderr, "FAIL\nExpected no output\nGot:\n%s\n", output);
        all_pass = 0;
    }

    remove(test_xml_path);

    if (all_pass) {
        fprintf(stderr, "All tests PASSED.\n");
    } else {
        fprintf(stderr, "Some tests FAILED.\n");
    }
}

int main(int argc, char** argv) {
    if (argc == 3) {
        return run_xpath(argv[1], argv[2]);
    } else {
        fprintf(stderr, "Usage: %s <xpath_value> <xml_file_name>\n", argv[0]);
        run_tests(argc, argv);
        return 1;
    }
}
