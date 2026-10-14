// GOOD #1: Fluent Builder.
//
//  - each setter has a NAME, so the call site documents itself,
//  - every option has a sensible default, set only what you need,
//  - methods return *this, so calls can be chained,
//  - build() validates once and returns a finished HttpRequest.
//    An invalid request can never exist.

#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

class HttpRequest {
public:
    class Builder;   // the only way to create an HttpRequest

    void print() const {
        std::cout << method_ << " " << url_ << "\n";
        for (const auto& [k, v] : headers_) std::cout << "  " << k << ": " << v << "\n";
        if (!body_.empty()) std::cout << "  body: " << body_ << "\n";
        std::cout << "  timeout=" << timeoutSec_ << "s retries=" << retries_
                  << " followRedirects=" << std::boolalpha << followRedirects_ << "\n";
    }

private:
    HttpRequest() = default;

    std::string method_ = "GET";
    std::string url_;
    std::map<std::string, std::string> headers_;
    std::string body_;
    int timeoutSec_ = 30;
    int retries_ = 0;
    bool followRedirects_ = true;
};

class HttpRequest::Builder {
    HttpRequest req_;
public:
    explicit Builder(std::string url) { req_.url_ = std::move(url); }

    Builder& method(std::string m)                         { req_.method_ = std::move(m); return *this; }
    Builder& header(const std::string& k, std::string v)   { req_.headers_[k] = std::move(v); return *this; }
    Builder& body(std::string b)                           { req_.body_ = std::move(b); return *this; }
    Builder& timeout(int seconds)                          { req_.timeoutSec_ = seconds; return *this; }
    Builder& retries(int n)                                { req_.retries_ = n; return *this; }
    Builder& noRedirects()                                 { req_.followRedirects_ = false; return *this; }

    HttpRequest build() const {
        if (req_.url_.empty())       throw std::invalid_argument("url is required");
        if (req_.timeoutSec_ <= 0)   throw std::invalid_argument("timeout must be positive");
        if (req_.method_ == "GET" && !req_.body_.empty())
            throw std::invalid_argument("GET request cannot have a body");
        return req_;
    }
};

int main() {
    auto get = HttpRequest::Builder("https://api.example.com/users")
                   .timeout(3)
                   .retries(2)
                   .build();
    get.print();

    auto post = HttpRequest::Builder("https://api.example.com/orders")
                    .method("POST")
                    .header("Content-Type", "application/json")
                    .header("Authorization", "Bearer abc123")
                    .body(R"({"item": "book", "qty": 1})")
                    .noRedirects()
                    .build();
    post.print();

    try {
        HttpRequest::Builder("").timeout(-5).build();
    } catch (const std::exception& e) {
        std::cout << "Rejected invalid request: " << e.what() << "\n";
    }
}
