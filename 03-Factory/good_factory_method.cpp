// GOOD #3: Factory Method (GoF).
//
// The base class Logistics contains the business algorithm (planDelivery)
// but delegates "which Transport to create" to a virtual factory method.
// Subclasses decide the concrete product. Adding AirLogistics = adding
// two small classes; planDelivery is never edited (Open/Closed Principle).
//
//      Logistics ---------creates---------> Transport
//         ^                                     ^
//   RoadLogistics -> Truck                   Truck
//   SeaLogistics  -> Ship                    Ship
//   AirLogistics  -> Plane                   Plane

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Product
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

class Plane : public Transport {
public:
    void deliver(const std::string& c) const override { std::cout << "Plane flies " << c << " by air\n"; }
};

// Creator
class Logistics {
public:
    virtual ~Logistics() = default;

    // The factory method
    virtual std::unique_ptr<Transport> createTransport() const = 0;

    // Business logic that USES the product, without knowing its concrete type
    void planDelivery(const std::string& cargo) const {
        std::cout << "Packing " << cargo << "\n";
        auto transport = createTransport();
        transport->deliver(cargo);
    }
};

class RoadLogistics : public Logistics {
public:
    std::unique_ptr<Transport> createTransport() const override { return std::make_unique<Truck>(); }
};

class SeaLogistics : public Logistics {
public:
    std::unique_ptr<Transport> createTransport() const override { return std::make_unique<Ship>(); }
};

// New requirement - nothing above changed.
class AirLogistics : public Logistics {
public:
    std::unique_ptr<Transport> createTransport() const override { return std::make_unique<Plane>(); }
};

int main() {
    std::vector<std::unique_ptr<Logistics>> companies;
    companies.push_back(std::make_unique<RoadLogistics>());
    companies.push_back(std::make_unique<SeaLogistics>());
    companies.push_back(std::make_unique<AirLogistics>());

    for (const auto& c : companies) c->planDelivery("boxes");
}
