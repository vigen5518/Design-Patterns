// ISP fixed: many small, focused interfaces.
//
// A class implements only what it really can do.
// A client asks only for what it really needs (IPrinter&, not IMachine&).
// "SimplePrinter cannot scan" is now a COMPILE error instead of a runtime crash.

#include <iostream>
#include <string>

class IPrinter {
public:
    virtual ~IPrinter() = default;
    virtual void print(const std::string& doc) = 0;
};

class IScanner {
public:
    virtual ~IScanner() = default;
    virtual void scan(const std::string& doc) = 0;
};

class IFax {
public:
    virtual ~IFax() = default;
    virtual void fax(const std::string& doc, const std::string& number) = 0;
};

class SimplePrinter : public IPrinter {
public:
    void print(const std::string& d) override { std::cout << "Printer prints " << d << "\n"; }
};

class PhotoScanner : public IScanner {
public:
    void scan(const std::string& d) override { std::cout << "Scanner scans " << d << "\n"; }
};

// Big device = combination of small interfaces.
class MultiFunctionPrinter : public IPrinter, public IScanner, public IFax {
public:
    void print(const std::string& d) override { std::cout << "MFP prints " << d << "\n"; }
    void scan(const std::string& d) override { std::cout << "MFP scans " << d << "\n"; }
    void fax(const std::string& d, const std::string& n) override {
        std::cout << "MFP faxes " << d << " to " << n << "\n";
    }
};

// Clients depend only on what they use.
void printInvoice(IPrinter& p) { p.print("invoice.pdf"); }
void digitize(IScanner& s)     { s.scan("contract.jpg"); }

int main() {
    SimplePrinter printer;
    PhotoScanner scanner;
    MultiFunctionPrinter mfp;

    printInvoice(printer);
    printInvoice(mfp);
    digitize(scanner);
    digitize(mfp);
    // digitize(printer);   // does not compile: SimplePrinter is not an IScanner. Good!
}
