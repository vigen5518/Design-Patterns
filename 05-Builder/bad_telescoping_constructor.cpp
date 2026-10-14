// BAD: "telescoping constructor" - many parameters, most of them optional.
//
// Reading the call site, can you tell what `false, true, 3, 30` mean?
// Swapping two ints or two bools compiles fine and is a silent bug.
// And the object can be built in an invalid state (no URL, negative timeout).

#include <iostream>
#include <map>
#include <string>

class HttpRequest {
public:
    std::string method;
    std::string url;
    std::map<std::string, std::string> headers;
    std::string body;
    int timeoutSec;
    int retries;
    bool followRedirects;
    bool verifySsl;

    HttpRequest(std::string method, std::string url, std::map<std::string, std::string> headers,
                std::string body, bool followRedirects, bool verifySsl, int timeoutSec, int retries)
        : method(std::move(method)), url(std::move(url)), headers(std::move(headers)),
          body(std::move(body)), timeoutSec(timeoutSec), retries(retries),
          followRedirects(followRedirects), verifySsl(verifySsl) {}

    void print() const {
        std::cout << method << " " << url << "  timeout=" << timeoutSec << " retries=" << retries
                  << " redirects=" << followRedirects << " ssl=" << verifySsl << "\n";
    }
};

int main() {
    // What is `false, true, 3, 30`? Which one is the timeout?
    HttpRequest r1("GET", "https://api.example.com/users", {}, "", false, true, 3, 30);
    r1.print();   // timeout=3 retries=30 -> we swapped them and nobody noticed

    HttpRequest r2("POST", "", {{"Accept", "json"}}, "{}", true, true, -5, 0);
    r2.print();   // empty URL and negative timeout - accepted without complaint
}
