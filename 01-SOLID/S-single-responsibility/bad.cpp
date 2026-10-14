// SRP violation: one class, many reasons to change.
//
// Report "knows" about:
//   1. the business data (order lines),
//   2. the pricing rules (tax, discount),
//   3. the output format (plain text),
//   4. the storage (where/how it is saved).
// A change in ANY of these forces us to edit (and re-test) the same class.

#include <iostream>
#include <string>
#include <vector>

struct OrderLine {
    std::string product;
    int quantity;
    double unitPrice;
};

class Order {
    std::vector<OrderLine> lines_;
public:
    void add(const std::string& product, int qty, double price) {
        lines_.push_back({product, qty, price});
    }

    // Responsibility #2: pricing rules (accounting team changes these)
    double total() const {
        double sum = 0;
        for (const auto& l : lines_) sum += l.quantity * l.unitPrice;
        if (sum > 100) sum *= 0.9;   // 10% discount
        return sum * 1.2;            // 20% VAT
    }

    // Responsibility #3: presentation (UI team changes this)
    void print() const {
        std::cout << "----- RECEIPT -----\n";
        for (const auto& l : lines_)
            std::cout << l.product << " x" << l.quantity << " = " << l.quantity * l.unitPrice << "\n";
        std::cout << "TOTAL: " << total() << "\n";
    }

    // Responsibility #4: persistence (DB team changes this)
    void saveToDatabase() const {
        std::cout << "INSERT INTO orders VALUES (" << total() << ");\n";
    }
};

int main() {
    Order order;
    order.add("Keyboard", 1, 70);
    order.add("Mouse", 2, 25);

    order.print();
    order.saveToDatabase();
    // Want an HTML receipt? Want to save to a file instead of a DB?
    // Want a different VAT? -> every time we edit Order.
}
