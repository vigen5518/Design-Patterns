// BAD: copying objects when you only have a pointer to the base class.
//
// We want "duplicate the selected shape" in a drawing editor.
// We hold Shape*, but copying requires knowing the CONCRETE type.
//
// Attempt 1 (slicing):      Shape copy = *shape;   -> not allowed for an abstract Shape,
//                           and for a concrete base it would cut off the derived part.
// Attempt 2 (type switch):  dynamic_cast chain     -> shown below. Every new shape type
//                           means editing duplicate(). Forget one -> nullptr at runtime.
//
// Bonus bug: a shallow copy of a raw-pointer member (Style*) - both "copies" share it.

#include <iostream>
#include <memory>
#include <string>

struct Style {
    std::string color;
};

class Shape {
public:
    int x = 0, y = 0;
    Style* style = nullptr;   // raw pointer: default copy shares it (shallow copy)
    virtual ~Shape() = default;
    virtual void draw() const = 0;
};

class Circle : public Shape {
public:
    int radius = 0;
    void draw() const override {
        std::cout << "Circle r=" << radius << " at (" << x << "," << y << ") color=" << style->color << "\n";
    }
};

class Rectangle : public Shape {
public:
    int w = 0, h = 0;
    void draw() const override {
        std::cout << "Rect " << w << "x" << h << " at (" << x << "," << y << ") color=" << style->color << "\n";
    }
};

class Triangle : public Shape {   // added later, nobody updated duplicate()
public:
    void draw() const override { std::cout << "Triangle\n"; }
};

std::unique_ptr<Shape> duplicate(const Shape* s) {
    if (auto c = dynamic_cast<const Circle*>(s))    return std::make_unique<Circle>(*c);
    if (auto r = dynamic_cast<const Rectangle*>(s)) return std::make_unique<Rectangle>(*r);
    return nullptr;   // Triangle falls through here
}

int main() {
    Style red{"red"};
    Circle c;
    c.radius = 5;
    c.style = &red;

    auto copy = duplicate(&c);
    copy->x = 100;
    copy->style->color = "blue";   // changes the ORIGINAL too - they share one Style

    c.draw();       // color=blue  <- we never touched the original!
    copy->draw();

    Triangle t;
    std::cout << "duplicate(triangle) = "
              << (duplicate(&t) ? "ok" : "nullptr - forgot to update the dynamic_cast chain") << "\n";
}
