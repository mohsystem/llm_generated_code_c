#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <unistd.h>
#include <fcntl.h>

void execute_xpath(const char* id_value, const char* filename) {
    xmlDocPtr doc = xmlReadFile(filename, NULL, 0);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse %s\n", filename);
        return;
    }

    xmlXPathContextPtr context = xmlXPathNewContext(doc);
    if (context == NULL) {
        fprintf(stderr, "Error: unable to create new XPath context\n");
        xmlFreeDoc(doc);
        return;
    }

    char expression[256];
    snprintf(expression, sizeof(expression), "/library/tag[@id='%s']", id_value);

    xmlXPathObjectPtr result = xmlXPathEvalExpression((xmlChar *)expression, context);
    if (result == NULL) {
        fprintf(stderr, "Error: unable to evaluate xpath expression\n");
        xmlXPathFreeContext(context);
        xmlFreeDoc(doc);
        return;
    }

    if (xmlXPathNodeSetIsEmpty(result->nodesetval)) {
        printf("No result\n");
    } else {
        xmlNodeSetPtr nodeset = result->nodesetval;
        for (int i = 0; i < nodeset->nodeNr; i++) {
            xmlChar *content = xmlNodeListGetString(doc, nodeset->nodeTab[i]->xmlChildrenNode, 1);
            printf("%s\n", content);
            xmlFree(content);
        }
    }

    xmlXPathFreeObject(result);
    xmlXPathFreeContext(context);
    xmlFreeDoc(doc);
    xmlCleanupParser();
}

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

void capture_output(void (*func)(const char*, const char*), const char* id_value, const char* filename,
                    int fd_to_capture, char* buffer, size_t bufsize) {
    int pipefd[2];
    pipe(pipefd);

    int saved_fd = dup(fd_to_capture);
    dup2(pipefd[1], fd_to_capture);
    close(pipefd[1]);

    func(id_value, filename);

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

    printf("Running test cases...\n");

    // Test 1: missing args
    printf("Test 1 (missing args): ");
    if (argc != 3) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
        all_pass = 0;
    }

    // Prepare test XML file
    const char* test_xml_path = "test.xml";
    const char* test_xml_content =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<library>\n"
        "  <tag id=\"1\">First</tag>\n"
        "  <tag id=\"2\">Second</tag>\n"
        "  <tag id=\"3\">Third</tag>\n"
        "</library>\n";

    FILE* f = fopen(test_xml_path, "w");
    if (!f) {
        fprintf(stderr, "Cannot create test XML file\n");
        exit(1);
    }
    fputs(test_xml_content, f);
    fclose(f);

    // Test 2: non-existent file
    printf("Test 2 (non-existent XML file): ");
    capture_output(execute_xpath, "1", "no_such_file.xml", 2, output, BUF_SIZE);
    if (strstr(output, "Failed to parse") != NULL) {
        printf("PASS\n");
    } else {
        printf("FAIL\nOutput was:\n%s\n", output);
        all_pass = 0;
    }

    // Test 3: valid XML + existing id
    printf("Test 3 (valid XML + existing id): ");
    capture_output(execute_xpath, "1", test_xml_path, 1, output, BUF_SIZE);
    if (strcmp(output, "First\n") == 0) {
        printf("PASS\n");
    } else {
        printf("FAIL\nExpected:\nFirst\nGot:\n%s\n", output);
        all_pass = 0;
    }

    // Test 4: valid XML + non-existing id
    printf("Test 4 (valid XML + non-existing id): ");
    capture_output(execute_xpath, "999", test_xml_path, 1, output, BUF_SIZE);
    if (strcmp(output, "No result\n") == 0) {
        printf("PASS\n");
    } else {
        printf("FAIL\nExpected:\nNo result\nGot:\n%s\n", output);
        all_pass = 0;
    }

    // Clean up
    remove(test_xml_path);

    if (all_pass) {
        printf("All tests PASSED.\n");
    } else {
        printf("Some tests FAILED.\n");
    }
}

int main(int argc, char **argv) {
    if (argc == 3) {
        execute_xpath(argv[1], argv[2]);
        return 0;
    } else {
        printf("Usage: %s <xpath_value> <xml_file>\n", argv[0]);
        run_tests(argc, argv);
        return 1;
    }
}
