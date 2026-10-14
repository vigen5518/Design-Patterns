// BAD #1: "Singleton" that forgot to delete copy/move.
//
// The constructor is private, getInstance() returns the same object...
// but nothing stops a caller from COPYING it. One innocent missing '&'
// and we silently have a second Config with its own state.

#include <iostream>
#include <string>

class Config {
    std::string language_ = "en";
    Config() { std::cout << "Config created\n"; }
public:
    static Config& getInstance() {
        static Config instance;
        return instance;
    }
    void setLanguage(const std::string& lang) { language_ = lang; }
    const std::string& language() const { return language_; }
};

int main() {
    Config& real = Config::getInstance();

    Config copy = Config::getInstance();   // <- forgot '&': copy constructor is still public!
    copy.setLanguage("hy");

    std::cout << "copy.language = " << copy.language() << "\n";   // hy
    std::cout << "real.language = " << real.language() << "\n";   // en  <- change was lost
    std::cout << "same object?  " << (&copy == &real ? "yes" : "NO - two instances!") << "\n";
}
