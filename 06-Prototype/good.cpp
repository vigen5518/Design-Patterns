// GOOD: Prototype - objects know how to copy THEMSELVES.
//
//  - Shape declares `virtual clone()`; each subclass implements it with its own
//    copy constructor, so the copy is always of the correct concrete type.
//  - Members are value types / unique_ptr with a proper deep copy -> no shared state.
//  - PrototypeRegistry: pre-configured "templates" that we clone instead of
//    building from scratch (like "duplicate" or "stamp" tools in an editor).

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

struct Style {
    std::string color;
    int borderWidth;
};

class Shape {
protected:
    int x_ = 0, y_ = 0;
    std::unique_ptr<Style> style_;   // owned -> must be DEEP copied

public:
    explicit Shape(Style style) : style_(std::make_unique<Style>(std::move(style))) {}
    Shape(const Shape& other)        // deep copy: new Style object, same values
        : x_(other.x_), y_(other.y_), style_(std::make_unique<Style>(*other.style_)) {}
    Shape& operator=(const Shape&) = delete;
    virtual ~Shape() = default;

    virtual std::unique_ptr<Shape> clone() const = 0;
    virtual void draw() const = 0;

    void moveTo(int x, int y) { x_ = x; y_ = y; }
    void setColor(const std::string& c) { style_->color = c; }
};

class Circle : public Shape {
    int radius_;
public:
    Circle(int radius, Style style) : Shape(std::move(style)), radius_(radius) {}

    std::unique_ptr<Shape> clone() const override { return std::make_unique<Circle>(*this); }
    void draw() const override {
        std::cout << "Circle r=" << radius_ << " at (" << x_ << "," << y_ << ") " << style_->color << "\n";
    }
};

class Rectangle : public Shape {
    int w_, h_;
public:
    Rectangle(int w, int h, Style style) : Shape(std::move(style)), w_(w), h_(h) {}

    std::unique_ptr<Shape> clone() const override { return std::make_unique<Rectangle>(*this); }
    void draw() const override {
        std::cout << "Rect " << w_ << "x" << h_ << " at (" << x_ << "," << y_ << ") " << style_->color << "\n";
    }
};

// Added later: just implement clone(). No central switch to update.
class Triangle : public Shape {
    int side_;
public:
    Triangle(int side, Style style) : Shape(std::move(style)), side_(side) {}

    std::unique_ptr<Shape> clone() const override { return std::make_unique<Triangle>(*this); }
    void draw() const override {
        std::cout << "Triangle side=" << side_ << " at (" << x_ << "," << y_ << ") " << style_->color << "\n";
    }
};

class PrototypeRegistry {
    std::map<std::string, std::unique_ptr<Shape>> prototypes_;
public:
    void add(const std::string& key, std::unique_ptr<Shape> proto) { prototypes_[key] = std::move(proto); }
    std::unique_ptr<Shape> create(const std::string& key) const { return prototypes_.at(key)->clone(); }
};

int main() {
    // 1. Clone through a base pointer: correct type, independent state.
    std::unique_ptr<Shape> original = std::make_unique<Circle>(5, Style{"red", 1});
    auto copy = original->clone();
    copy->moveTo(100, 0);
    copy->setColor("blue");

    std::cout << "original: "; original->draw();   // still red at (0,0)
    std::cout << "copy:     "; copy->draw();       // blue at (100,0)

    // 2. Registry of ready-made templates.
    PrototypeRegistry registry;
    registry.add("big-red-circle", std::make_unique<Circle>(50, Style{"red", 3}));
    registry.add("button",         std::make_unique<Rectangle>(120, 40, Style{"gray", 1}));
    registry.add("warning",        std::make_unique<Triangle>(30, Style{"yellow", 2}));

    std::vector<std::unique_ptr<Shape>> canvas;
    for (const char* key : {"button", "button", "warning", "big-red-circle"})
        canvas.push_back(registry.create(key));

    int x = 0;
    for (auto& s : canvas) { s->moveTo(x, 10); x += 130; }

    std::cout << "\ncanvas:\n";
    for (const auto& s : canvas) s->draw();
}
