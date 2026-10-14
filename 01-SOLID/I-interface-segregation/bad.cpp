// ISP violation: one "fat" interface forces clients to depend on methods they don't use.
//
// A simple printer must implement scan() and fax() it does not have.
// It ends up throwing or silently doing nothing -> callers can't trust the interface,
// and every change to fax() recompiles code that only ever prints.

#include <iostream>
#include <stdexcept>
#include <string>

class IMachine {
public:
    virtual ~IMachine() = default;
    virtual void print(const std::string& doc) = 0;
    virtual void scan(const std::string& doc) = 0;
    virtual void fax(const std::string& doc, const std::string& number) = 0;
};

class MultiFunctionPrinter : public IMachine {
public:
    void print(const std::string& d) override { std::cout << "MFP prints " << d << "\n"; }
    void scan(const std::string& d) override { std::cout << "MFP scans " << d << "\n"; }
    void fax(const std::string& d, const std::string& n) override {
        std::cout << "MFP faxes " << d << " to " << n << "\n";
    }
};

class SimplePrinter : public IMachine {
public:
    void print(const std::string& d) override { std::cout << "Printer prints " << d << "\n"; }

    // Forced to implement things it physically cannot do:
    void scan(const std::string&) override { throw std::logic_error("SimplePrinter cannot scan"); }
    void fax(const std::string&, const std::string&) override { /* silently does nothing - even worse */ }
};

int main() {
    SimplePrinter printer;
    IMachine& machine = printer;

    machine.print("report.pdf");
    machine.fax("report.pdf", "+374 10 000000");   // nothing happens, no error
    try {
        machine.scan("passport.jpg");              // compiles fine, fails at runtime
    } catch (const std::exception& e) {
        std::cout << "ERROR: " << e.what() << "\n";
    }
}
