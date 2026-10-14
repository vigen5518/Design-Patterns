// OCP violation: to add a new shipping method we must MODIFY existing code.
//
// Every new carrier means: edit the enum, edit the switch, recompile and re-test
// code that was already working. Forgetting one switch somewhere = silent bug.

#include <iostream>
#include <stdexcept>

enum class Shipping { Standard, Express, Pickup };   // add "Drone" here...

class ShippingCostCalculator {
public:
    double cost(Shipping type, double weightKg) const {
        switch (type) {                               // ...and here...
        case Shipping::Standard: return 5.0 + weightKg * 1.0;
        case Shipping::Express:  return 15.0 + weightKg * 2.5;
        case Shipping::Pickup:   return 0.0;
        }
        throw std::invalid_argument("unknown shipping type");
    }
};

class DeliveryTimeEstimator {
public:
    int days(Shipping type) const {
        switch (type) {                               // ...and here too.
        case Shipping::Standard: return 5;
        case Shipping::Express:  return 1;
        case Shipping::Pickup:   return 0;
        }
        throw std::invalid_argument("unknown shipping type");
    }
};

int main() {
    ShippingCostCalculator calc;
    DeliveryTimeEstimator eta;

    for (Shipping s : {Shipping::Standard, Shipping::Express, Shipping::Pickup})
        std::cout << "cost=" << calc.cost(s, 3.0) << "  days=" << eta.days(s) << "\n";
}
