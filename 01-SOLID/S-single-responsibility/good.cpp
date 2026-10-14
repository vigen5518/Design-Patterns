// SRP fixed: every class has exactly one reason to change.
//
//   Order             - holds the data             (changes when the domain changes)
//   PriceCalculator   - pricing rules              (changes when accounting rules change)
//   ReceiptPrinter    - text output                (changes when the receipt layout changes)
//   OrderRepository   - persistence                (changes when storage changes)

#include <iostream>
#include <string>
#include <vector>

struct OrderLine {
    std::string product;
    int quantity;
    double unitPrice;

    double subtotal() const { return quantity * unitPrice; }
};

class Order {
    std::vector<OrderLine> lines_;
public:
    void add(const std::string& product, int qty, double price) {
        lines_.push_back({product, qty, price});
    }
    const std::vector<OrderLine>& lines() const { return lines_; }
};

class PriceCalculator {
    double vat_;
    double discountThreshold_;
    double discount_;
public:
    PriceCalculator(double vat, double discountThreshold, double discount)
        : vat_(vat), discountThreshold_(discountThreshold), discount_(discount) {}

    double total(const Order& order) const {
        double sum = 0;
        for (const auto& l : order.lines()) sum += l.subtotal();
        if (sum > discountThreshold_) sum *= (1.0 - discount_);
        return sum * (1.0 + vat_);
    }
};

class ReceiptPrinter {
public:
    void print(const Order& order, double total) const {
        std::cout << "----- RECEIPT -----\n";
        for (const auto& l : order.lines())
            std::cout << l.product << " x" << l.quantity << " = " << l.subtotal() << "\n";
        std::cout << "TOTAL: " << total << "\n";
    }
};

class OrderRepository {
public:
    void save(double total) const {
        std::cout << "INSERT INTO orders VALUES (" << total << ");\n";
    }
};

int main() {
    Order order;
    order.add("Keyboard", 1, 70);
    order.add("Mouse", 2, 25);

    PriceCalculator calculator(/*vat*/ 0.2, /*threshold*/ 100, /*discount*/ 0.1);
    const double total = calculator.total(order);

    ReceiptPrinter{}.print(order, total);
    OrderRepository{}.save(total);
    // New VAT rate? Only PriceCalculator changes.
    // HTML receipt?  Write an HtmlReceiptPrinter, nothing else is touched.
}
