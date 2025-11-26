#include <fastcgi/fcgi_stdio.h>
#include <onion/onion.h>
#include <onion/log.h>
#include <onion/request.h>
#include <onion/response.h>

// Handler function for the root URL ("/")
void handle_request(void *_, onion_request *req, onion_response *res) {
    // Set CORS header as in the Python example (allow origin http://example.com)
    onion_response_set_header(res, "Access-Control-Allow-Origin", "http://example.com");
    onion_response_set_header(res, "Content-Type", "application/json");
    onion_response_write0(res, "{\"message\": \"This is CORS-enabled data\"}");
}

int main() {
    // Create new onion server with thread pool option fixed
    onion *o = onion_new(O_POOL);
    onion_url *url = onion_root_url(o);

    // Add route for root URL "/"
    onion_url_add(url, "", handle_request);

    printf("Server started on port 8080\n");

    // Run server (blocking)
    int ret = onion_listen(o);

    onion_free(o);

    // For demonstration, print test case results based on success of onion_listen
    if (ret >= 0) {
        printf("Test case: PASS (Server started and running)\n");
    } else {
        printf("Test case: FAIL (Server failed to start)\n");
    }

    return ret;
}
