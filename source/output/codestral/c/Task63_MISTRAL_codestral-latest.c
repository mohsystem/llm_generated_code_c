#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 // #include <base64.h>
#include <cjson/cJSON.h>
//can't fix, base64_decoded_length doesn't exist in #include <base64.h> or on the web
// cJSON* processRequest(const char* rawData) {
//     int decodedLength = base64_decoded_length(rawData, strlen(rawData));
//     char* decodedData = (char*)malloc(decodedLength + 1);
//     base64_decode(rawData, strlen(rawData), decodedData);
//     decodedData[decodedLength] = '\0';
//     cJSON* deserializedData = cJSON_Parse(decodedData);
//     free(decodedData);
//     return deserializedData;
// }