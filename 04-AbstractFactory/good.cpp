// GOOD: Abstract Factory - an interface for creating FAMILIES of related objects.
//
//                 ThemeFactory (abstract)
//                 + createButton()
//                 + createCheckbox()
//                  /                \
//       LightThemeFactory       DarkThemeFactory
//       -> LightButton          -> DarkButton
//       -> LightCheckbox        -> DarkCheckbox
//
// The theme is chosen ONCE (at startup). Client code receives a factory and
// can only ever get matching widgets -> mixing families is impossible.
// Adding a theme = one new factory + its widgets. Client code is untouched.

#include <iostream>
#include <memory>
#include <string>

// ----- Abstract products -----
class Button {
public:
    virtual ~Button() = default;
    virtual void render(const std::string& label) const = 0;
};

class Checkbox {
public:
    virtual ~Checkbox() = default;
    virtual void render(const std::string& label) const = 0;
};

// ----- Light family -----
class LightButton : public Button {
public:
    void render(const std::string& l) const override { std::cout << "[ " << l << " ]   (white bg, black text)\n"; }
};
class LightCheckbox : public Checkbox {
public:
    void render(const std::string& l) const override { std::cout << "( ) " << l << "   (light)\n"; }
};

// ----- Dark family -----
class DarkButton : public Button {
public:
    void render(const std::string& l) const override { std::cout << "[ " << l << " ]   (black bg, white text)\n"; }
};
class DarkCheckbox : public Checkbox {
public:
    void render(const std::string& l) const override { std::cout << "(*) " << l << "   (dark)\n"; }
};

// ----- Abstract factory -----
class ThemeFactory {
public:
    virtual ~ThemeFactory() = default;
    virtual std::unique_ptr<Button> createButton() const = 0;
    virtual std::unique_ptr<Checkbox> createCheckbox() const = 0;
};

class LightThemeFactory : public ThemeFactory {
public:
    std::unique_ptr<Button> createButton() const override { return std::make_unique<LightButton>(); }
    std::unique_ptr<Checkbox> createCheckbox() const override { return std::make_unique<LightCheckbox>(); }
};

class DarkThemeFactory : public ThemeFactory {
public:
    std::unique_ptr<Button> createButton() const override { return std::make_unique<DarkButton>(); }
    std::unique_ptr<Checkbox> createCheckbox() const override { return std::make_unique<DarkCheckbox>(); }
};

// ----- Client: knows only the abstract interfaces -----
class LoginForm {
    std::unique_ptr<Button> ok_;
    std::unique_ptr<Checkbox> remember_;
public:
    explicit LoginForm(const ThemeFactory& theme)
        : ok_(theme.createButton()), remember_(theme.createCheckbox()) {}

    void render() const {
        remember_->render("Remember me");
        ok_->render("Log in");
    }
};

// The ONLY place that knows concrete factories.
std::unique_ptr<ThemeFactory> makeTheme(const std::string& name) {
    if (name == "dark") return std::make_unique<DarkThemeFactory>();
    return std::make_unique<LightThemeFactory>();
}

int main() {
    for (const std::string theme : {"light", "dark"}) {
        std::cout << "--- " << theme << " theme ---\n";
        auto factory = makeTheme(theme);
        LoginForm form(*factory);
        form.render();
    }
}
