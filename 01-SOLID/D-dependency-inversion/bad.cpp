// DIP violation: high-level policy depends directly on a low-level detail.
//
// OrderService (business logic) creates and owns a concrete EmailSender.
//  - Want SMS instead of email? Edit OrderService.
//  - Want to unit-test OrderService without sending real e-mails? Impossible.
// The arrow points the wrong way: business logic -> infrastructure.

#include <iostream>
#include <string>

// Low-level module
class EmailSender {
public:
    void sendEmail(const std::string& to, const std::string& text) {
        std::cout << "[SMTP] to " << to << ": " << text << "\n";
    }
};

// High-level module, hard-wired to the low-level one
class OrderService {
    EmailSender email_;   // concrete type, created inside
public:
    void placeOrder(const std::string& customer, int orderId) {
        std::cout << "Order #" << orderId << " placed\n";
        email_.sendEmail(customer, "Your order #" + std::to_string(orderId) + " is confirmed");
    }
};

int main() {
    OrderService service;
    service.placeOrder("anna@example.com", 42);
}
