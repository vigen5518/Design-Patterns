// OCP fixed: open for extension, closed for modification.
//
// Each shipping method is its own class behind a common interface.
// Adding DroneShipping = adding ONE new class. Checkout is never edited.

#include <iostream>
#include <memory>
#include <string>
#include <vector>

class ShippingMethod {
public:
    virtual ~ShippingMethod() = default;
    virtual std::string name() const = 0;
    virtual double cost(double weightKg) const = 0;
    virtual int days() const = 0;
};

class StandardShipping : public ShippingMethod {
public:
    std::string name() const override { return "Standard"; }
    double cost(double w) const override { return 5.0 + w * 1.0; }
    int days() const override { return 5; }
};

class ExpressShipping : public ShippingMethod {
public:
    std::string name() const override { return "Express"; }
    double cost(double w) const override { return 15.0 + w * 2.5; }
    int days() const override { return 1; }
};

class StorePickup : public ShippingMethod {
public:
    std::string name() const override { return "Pickup"; }
    double cost(double) const override { return 0.0; }
    int days() const override { return 0; }
};

// NEW requirement, added without touching any line above or below.
class DroneShipping : public ShippingMethod {
public:
    std::string name() const override { return "Drone"; }
    double cost(double w) const override { return 25.0 + w * 4.0; }
    int days() const override { return 0; }
};

// Closed for modification: works with any current or future ShippingMethod.
void printOptions(const std::vector<std::unique_ptr<ShippingMethod>>& methods, double weightKg) {
    for (const auto& m : methods)
        std::cout << m->name() << ": cost=" << m->cost(weightKg) << "  days=" << m->days() << "\n";
}

int main() {
    std::vector<std::unique_ptr<ShippingMethod>> methods;
    methods.push_back(std::make_unique<StandardShipping>());
    methods.push_back(std::make_unique<ExpressShipping>());
    methods.push_back(std::make_unique<StorePickup>());
    methods.push_back(std::make_unique<DroneShipping>());

    printOptions(methods, 3.0);
}
