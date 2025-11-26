#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// cJSON implementation
#include <stdint.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <ctype.h>

// cJSON structure definitions and function prototypes
typedef struct cJSON {
    struct cJSON *next;
    struct cJSON *prev;
    struct cJSON *child;
    int type;
    char *valuestring;
    int valueint;
    double valuedouble;
    char *string;
} cJSON;

cJSON *cJSON_Parse(const char *value);
char *cJSON_Print(const cJSON *item);
void cJSON_Delete(cJSON *c);
const char *cJSON_GetErrorPtr(void);

// cJSON implementation (simplified for this example)
static const char *parse_string(cJSON *item, const char *str) {
    const char *ptr = str + 1;
    char *ptr2;
    char *out;
    int len = 0;
    if (*str != '\"') {
        return 0;
    }
    while (*ptr != '\"' && *ptr && ++len) {
        if (*ptr++ == '\\') ptr++;
    }
    out = (char*)malloc(len + 1);
    if (!out) return 0;
    ptr = str + 1;
    ptr2 = out;
    while (*ptr != '\"' && *ptr) {
        if (*ptr != '\\') *ptr2++ = *ptr++;
        else {
            ptr++;
            switch (*ptr) {
                case 'b': *ptr2++ = '\b'; break;
                case 'f': *ptr2++ = '\f'; break;
                case 'n': *ptr2++ = '\n'; break;
                case 'r': *ptr2++ = '\r'; break;
                case 't': *ptr2++ = '\t'; break;
                default: *ptr2++ = *ptr; break;
            }
            ptr++;
        }
    }
    *ptr2 = 0;
    if (*ptr == '\"') ptr++;
    item->valuestring = out;
    item->type = 4;
    return ptr;
}

cJSON *cJSON_Parse(const char *value) {
    cJSON *c = (cJSON*)malloc(sizeof(cJSON));
    if (c == NULL) return NULL;
    memset(c, 0, sizeof(cJSON));
    if (!parse_string(c, value)) {
        free(c);
        return NULL;
    }
    return c;
}

char *cJSON_Print(const cJSON *item) {
    if (item == NULL) return NULL;
    return item->valuestring;
}

void cJSON_Delete(cJSON *c) {
    if (c == NULL) return;
    free(c->valuestring);
    free(c);
}

const char *cJSON_GetErrorPtr(void) {
    return "Error parsing JSON";
}

// Main program
#define MAX_INPUT_SIZE 1024

int main() {
    char input[MAX_INPUT_SIZE];
    printf("Enter serialized data (JSON format): ");
    fgets(input, sizeof(input), stdin);

    cJSON *json = cJSON_Parse(input);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            printf("Error: %s\n", error_ptr);
        }
        printf("Error: Invalid JSON data\n");
    } else {
        char *printed_json = cJSON_Print(json);
        printf("Deserialized data:\n%s\n", printed_json);
        cJSON_Delete(json);
    }

    return 0;
}