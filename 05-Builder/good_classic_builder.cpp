// GOOD #2: Classic (GoF) Builder with a Director.
//
// Use it when the SAME construction steps must produce DIFFERENT representations.
//
//   Director          - knows the ORDER of steps ("a computer = case, cpu, ram, storage, gpu")
//   Builder           - abstract interface with one method per step
//   Concrete builders - decide WHAT each step puts in the product
//
// Here one Director recipe builds a gaming PC, an office PC, or a price quote
// (a completely different product type!) just by swapping the builder.

#include <iostream>
#include <string>
#include <vector>

// ---------- Product #1 ----------
struct Computer {
    std::vector<std::string> parts;
    void show() const {
        std::cout << "Computer:\n";
        for (const auto& p : parts) std::cout << "  - " << p << "\n";
    }
};

// ---------- Builder interface ----------
class ComputerBuilder {
public:
    virtual ~ComputerBuilder() = default;
    virtual void buildCase() = 0;
    virtual void buildCpu() = 0;
    virtual void buildRam() = 0;
    virtual void buildStorage() = 0;
    virtual void buildGpu() = 0;
};

class GamingPcBuilder : public ComputerBuilder {
    Computer pc_;
public:
    void buildCase() override    { pc_.parts.push_back("RGB tower case"); }
    void buildCpu() override     { pc_.parts.push_back("16-core CPU"); }
    void buildRam() override     { pc_.parts.push_back("64 GB DDR5"); }
    void buildStorage() override { pc_.parts.push_back("2 TB NVMe SSD"); }
    void buildGpu() override     { pc_.parts.push_back("High-end GPU"); }
    Computer result() { return std::move(pc_); }
};

class OfficePcBuilder : public ComputerBuilder {
    Computer pc_;
public:
    void buildCase() override    { pc_.parts.push_back("Mini case"); }
    void buildCpu() override     { pc_.parts.push_back("4-core CPU"); }
    void buildRam() override     { pc_.parts.push_back("16 GB DDR4"); }
    void buildStorage() override { pc_.parts.push_back("512 GB SSD"); }
    void buildGpu() override     { /* integrated graphics: nothing to add */ }
    Computer result() { return std::move(pc_); }
};

// ---------- Product #2: totally different type, same steps ----------
class PriceQuoteBuilder : public ComputerBuilder {
    double total_ = 0;
public:
    void buildCase() override    { total_ += 80; }
    void buildCpu() override     { total_ += 450; }
    void buildRam() override     { total_ += 200; }
    void buildStorage() override { total_ += 150; }
    void buildGpu() override     { total_ += 900; }
    double result() const { return total_; }
};

// ---------- Director ----------
class Director {
public:
    void buildFullPc(ComputerBuilder& b) const {
        b.buildCase();
        b.buildCpu();
        b.buildRam();
        b.buildStorage();
        b.buildGpu();
    }
    void buildServerWithoutGpu(ComputerBuilder& b) const {
        b.buildCase();
        b.buildCpu();
        b.buildRam();
        b.buildStorage();
    }
};

int main() {
    Director director;

    GamingPcBuilder gaming;
    director.buildFullPc(gaming);
    gaming.result().show();

    OfficePcBuilder office;
    director.buildFullPc(office);
    office.result().show();

    PriceQuoteBuilder quote;
    director.buildFullPc(quote);
    std::cout << "Full PC quote: $" << quote.result() << "\n";

    PriceQuoteBuilder serverQuote;
    director.buildServerWithoutGpu(serverQuote);
    std::cout << "Server quote:  $" << serverQuote.result() << "\n";
}
