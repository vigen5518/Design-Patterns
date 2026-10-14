// DIP fixed: both high-level and low-level modules depend on an ABSTRACTION.
//
//      OrderService ---> INotifier <--- EmailNotifier / SmsNotifier / FakeNotifier
//
// The dependency is injected through the constructor ("dependency injection").
// Swapping email for SMS, or using a fake in tests, needs zero changes to OrderService.

#include <iostream>
#include <string>
#include <vector>

// Abstraction, owned by the high-level side
class INotifier {
public:
    virtual ~INotifier() = default;
    virtual void notify(const std::string& to, const std::string& text) = 0;
};

// High-level module: knows only INotifier
class OrderService {
    INotifier& notifier_;
public:
    explicit OrderService(INotifier& notifier) : notifier_(notifier) {}

    void placeOrder(const std::string& customer, int orderId) {
        std::cout << "Order #" << orderId << " placed\n";
        notifier_.notify(customer, "Your order #" + std::to_string(orderId) + " is confirmed");
    }
};

// Low-level modules: implement the abstraction
class EmailNotifier : public INotifier {
public:
    void notify(const std::string& to, const std::string& text) override {
        std::cout << "[SMTP] to " << to << ": " << text << "\n";
    }
};

class SmsNotifier : public INotifier {
public:
    void notify(const std::string& to, const std::string& text) override {
        std::cout << "[SMS] to " << to << ": " << text << "\n";
    }
};

// Test double: records messages instead of sending them
class FakeNotifier : public INotifier {
public:
    std::vector<std::string> sent;
    void notify(const std::string& to, const std::string& text) override {
        sent.push_back(to + " | " + text);
    }
};

int main() {
    EmailNotifier email;
    OrderService(email).placeOrder("anna@example.com", 42);

    SmsNotifier sms;
    OrderService(sms).placeOrder("+374 99 123456", 43);

    // "Unit test" of OrderService: no network, fully checkable.
    FakeNotifier fake;
    OrderService(fake).placeOrder("test@example.com", 44);
    std::cout << "Test: " << (fake.sent.size() == 1 ? "PASSED" : "FAILED")
              << " (captured: " << fake.sent.front() << ")\n";
}
