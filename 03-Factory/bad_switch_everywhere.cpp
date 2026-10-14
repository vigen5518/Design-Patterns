// BAD: client code creates concrete classes itself.
//
// The application logic (planDelivery) contains `new Truck` / `new Ship`
// and a switch on the type. Every new transport (Plane, Drone...) forces
// us to edit this business function. Creation and usage are mixed together.

#include <iostream>
#include <memory>
#include <string>

class Transport {
public:
    virtual ~Transport() = default;
    virtual void deliver(const std::string& cargo) const = 0;
};

class Truck : public Transport {
public:
    void deliver(const std::string& c) const override { std::cout << "Truck drives " << c << " by road\n"; }
};

class Ship : public Transport {
public:
    void deliver(const std::string& c) const override { std::cout << "Ship carries " << c << " by sea\n"; }
};

void planDelivery(const std::string& mode, const std::string& cargo) {
    std::unique_ptr<Transport> t;
    if (mode == "road")      t = std::make_unique<Truck>();
    else if (mode == "sea")  t = std::make_unique<Ship>();
    // else if (mode == "air") ... edit here every time
    else { std::cout << "unknown mode " << mode << "\n"; return; }

    std::cout << "Packing " << cargo << "\n";
    t->deliver(cargo);
}

int main() {
    planDelivery("road", "apples");
    planDelivery("sea", "cars");
    planDelivery("air", "letters");   // not supported until we edit planDelivery
}
