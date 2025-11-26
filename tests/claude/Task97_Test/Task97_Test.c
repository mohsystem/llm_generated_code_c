#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

// تنفيذيّة تنفيذ XPath مع طباعة المخرجات كالموجودة
void execute_xpath(const char* filename, const char* xpathExpr) {
    xmlDocPtr doc;
    xmlXPathContextPtr xpathCtx;
    xmlXPathObjectPtr xpathObj;

    doc = xmlParseFile(filename);
    if (doc == NULL) {
        fprintf(stderr, "Error: unable to parse file \"%s\"\n", filename);
        return;
    }

    xpathCtx = xmlXPathNewContext(doc);
    if(xpathCtx == NULL) {
        fprintf(stderr,"Error: unable to create new XPath context\n");
        xmlFreeDoc(doc);
        return;
    }

    xpathObj = xmlXPathEvalExpression((xmlChar*)xpathExpr, xpathCtx);
    if(xpathObj == NULL) {
        fprintf(stderr,"Error: unable to evaluate xpath expression \"%s\"\n", xpathExpr);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return;
    }

    xmlNodeSetPtr nodes = xpathObj->nodesetval;
    if (nodes) {
        for (int i = 0; i < nodes->nodeNr; i++) {
            xmlNodePtr cur = nodes->nodeTab[i];
            if (cur->type == XML_ELEMENT_NODE) {
                xmlChar* content = xmlNodeGetContent(cur);
                printf("%s\n", content);
                xmlFree(content);
            }
        }
    }

    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);
    xmlCleanupParser();
}

// دالة لقراءة ملف نصّي بالكامل
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

// دالة لالتقاط stdout أو stderr أثناء تنفيذ دالة execute_xpath
// fd_to_capture = 1 (stdout) أو 2 (stderr)
void capture_output(void (*func)(const char*, const char*), const char* file, const char* xpath,
                    int fd_to_capture, char* buffer, size_t bufsize) {
    int pipefd[2];
    pipe(pipefd);

    int saved_fd = dup(fd_to_capture);
    dup2(pipefd[1], fd_to_capture);
    close(pipefd[1]);

    func(file, xpath);

    fflush(stdout);
    fflush(stderr);

    dup2(saved_fd, fd_to_capture);
    close(saved_fd);

    ssize_t n = read(pipefd[0], buffer, bufsize - 1);
    if (n >= 0) buffer[n] = '\0'; else buffer[0] = '\0';
    close(pipefd[0]);
}

void run_all_tests(int argc, char **argv) {
    const size_t BUF_SIZE = 8192;
    char output[BUF_SIZE];
    int all_pass = 1;

    printf("Running all test cases...\n\n");

    // Test 1: missing args (argc != 3)
    printf("Test case 1 (missing args): ");
    if (argc != 3) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
        all_pass = 0;
    }

    // Test 2: Non-existent XML file
    printf("Test case 2 (non-existent XML file): ");
    capture_output(execute_xpath, "no_such_file.xml", "/library/book", 2, output, BUF_SIZE);
    if (strstr(output, "unable to parse file") != NULL) {
        printf("PASS\n");
    } else {
        printf("FAIL\nOutput was:\n%s\n", output);
        all_pass = 0;
    }

    // Test 3: Valid XML + XPath from your file
    printf("Test case 3 (valid XML + xpath from file): ");
    const char* xml_path = "/home/kali/CLionProjects/llm_generated_code_c/tests/claude/Task97_Test/xmlExample.xml";
    const char* xpath_path = "/home/kali/CLionProjects/llm_generated_code_c/tests/claude/Task97_Test/xpathExample.txt";

    char* xpath_expr = read_file_to_string(xpath_path);
    if (!xpath_expr) {
        printf("FAIL\nCould not read XPath file\n");
        all_pass = 0;
    } else {
        // Trim trailing newline
        size_t len = strlen(xpath_expr);
        if (len > 0 && (xpath_expr[len - 1] == '\n' || xpath_expr[len - 1] == '\r'))
            xpath_expr[len - 1] = '\0';

        capture_output(execute_xpath, xml_path, xpath_expr, 1, output, BUF_SIZE);

        // Expected output for xpath /library/book[@id='1']/author in your xmlExample.xml is:
        const char* expected_output = "Joshua Bloch\n";

        if (strcmp(output, expected_output) == 0) {
            printf("PASS\n");
        } else {
            printf("FAIL\nExpected:\n%sGot:\n%s\n", expected_output, output);
            all_pass = 0;
        }
        free(xpath_expr);
    }

    // Test 4: Invalid XPath expression
    printf("Test case 4 (invalid xpath): ");
    capture_output(execute_xpath, xml_path, "/library/book[@id='1'", 2, output, BUF_SIZE);
    if (strstr(output, "unable to evaluate xpath expression") != NULL) {
        printf("PASS\n");
    } else {
        printf("FAIL\nExpected XPath evaluation error.\nGot:\n%s\n", output);
        all_pass = 0;
    }

    if (all_pass) {
        printf("\nAll test cases PASSED.\n");
    } else {
        printf("\nSome test cases FAILED.\n");
    }
}

int main(int argc, char **argv) {
    if (argc == 3) {
        execute_xpath(argv[1], argv[2]);
        return 0;
    } else {
        printf("Usage: %s <xml_file> <xpath>\n", argv[0]);
        printf("Running internal test cases...\n\n");
        run_all_tests(argc, argv);
        return 1;
    }
}
