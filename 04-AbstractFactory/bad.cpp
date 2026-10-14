// BAD: creating a "family" of related objects with if/else scattered everywhere.
//
// A UI has Buttons and Checkboxes, in a Light or Dark theme.
// Problems:
//  - the `if (dark)` check is repeated at every creation point,
//  - nothing prevents mixing families (dark button + light checkbox) - see the bug below,
//  - adding a "HighContrast" theme means hunting down every `if` in the program.

#include <iostream>
#include <memory>

class Button {
public:
    virtual ~Button() = default;
    virtual void render() const = 0;
};
class LightButton : public Button {
public:
    void render() const override { std::cout << "[ Light button ]\n"; }
};
class DarkButton : public Button {
public:
    void render() const override { std::cout << "[ Dark button  ]\n"; }
};

class Checkbox {
public:
    virtual ~Checkbox() = default;
    virtual void render() const = 0;
};
class LightCheckbox : public Checkbox {
public:
    void render() const override { std::cout << "( ) Light checkbox\n"; }
};
class DarkCheckbox : public Checkbox {
public:
    void render() const override { std::cout << "(*) Dark checkbox\n"; }
};

void buildLoginForm(bool dark) {
    std::unique_ptr<Button> ok;
    if (dark) ok = std::make_unique<DarkButton>();
    else      ok = std::make_unique<LightButton>();

    std::unique_ptr<Checkbox> remember;
    if (!dark) remember = std::make_unique<DarkCheckbox>();   // BUG: inverted check, families mixed!
    else       remember = std::make_unique<LightCheckbox>();

    ok->render();
    remember->render();
}

int main() {
    std::cout << "Dark login form:\n";
    buildLoginForm(true);   // prints a Dark button with a Light checkbox
}
