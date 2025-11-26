// Strictly follow the same test case as Python CORS example.
// Complete working libonion C server example with CORS headers.

#include <onion/onion.h>
#include <onion/request.h>
#include <onion/response.h>

int cors_handler(onion_request *req, onion_response *res) {
    // Set CORS headers as in the Python example
    onion_response_set_header(res, "Access-Control-Allow-Origin", "*");
    onion_response_set_header(res, "Access-Control-Allow-Headers", "origin, content-type, accept, authorization");
    onion_response_set_header(res, "Access-Control-Allow-Credentials", "true");
    onion_response_set_header(res, "Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, HEAD");
    onion_response_set_header(res, "Access-Control-Max-Age", "1209600");

    onion_response_set_code(res, 200);
    onion_response_write0(res, "{\"message\": \"This is CORS-enabled data\"}");
    return OCS_PROCESSED;
}

int main() {
    onion *server = onion_new(O_POOL);
    onion_url *urls = onion_root_url(server);

    onion_url_add(urls, "/api/data", cors_handler);

    // Start server on port 8080, blocking call
    onion_listen(server);

    onion_free(server);
    return 0;
}
