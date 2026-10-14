// GOOD #2: inner factory (nested class).
//
// When there are many creation methods, they can be grouped in a separate
// Factory class. Nesting it inside Point gives it access to the private
// constructor without `friend`, and keeps the API discoverable: Point::Factory::...

#include <cmath>
#include <iomanip>
#include <iostream>

class Point {
    double x_, y_;
    Point(double x, double y) : x_(x), y_(y) {}

public:
    double x() const { return x_; }
    double y() const { return y_; }

    // A nested class is a member, so it can call Point's private constructor.
    class Factory {
    public:
        Factory() = delete;   // pure "namespace" of creators, never instantiated

        static Point cartesian(double x, double y) { return Point(x, y); }
        static Point polar(double r, double theta) {
            return Point(r * std::cos(theta), r * std::sin(theta));
        }
        // Grid cell -> its center point (cell size 10)
        static Point gridCell(int col, int row) { return Point(col * 10 + 5, row * 10 + 5); }
    };
};

int main() {
    std::cout << std::fixed << std::setprecision(2);
    const double pi = std::acos(-1.0);

    auto a = Point::Factory::cartesian(3, 4);
    auto b = Point::Factory::polar(2, pi);
    auto c = Point::Factory::gridCell(1, 2);

    std::cout << "a = (" << a.x() << ", " << a.y() << ")\n";
    std::cout << "b = (" << b.x() << ", " << b.y() << ")\n";
    std::cout << "c = (" << c.x() << ", " << c.y() << ")\n";
}
