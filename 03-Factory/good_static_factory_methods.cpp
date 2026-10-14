// GOOD #1: static factory methods.
//
// The constructor becomes private; creation goes through NAMED functions.
// The name tells the reader what the two numbers mean - no flag to forget.

#include <cmath>
#include <iomanip>
#include <iostream>

class Point {
    double x_, y_;
    Point(double x, double y) : x_(x), y_(y) {}   // private: only factories may call it

public:
    static Point fromCartesian(double x, double y) { return Point(x, y); }

    static Point fromPolar(double r, double theta) {
        return Point(r * std::cos(theta), r * std::sin(theta));
    }

    static Point origin() { return Point(0, 0); }

    double x() const { return x_; }
    double y() const { return y_; }
};

std::ostream& operator<<(std::ostream& os, const Point& p) {
    return os << "(" << p.x() << ", " << p.y() << ")";
}

int main() {
    std::cout << std::fixed << std::setprecision(2);
    const double pi = std::acos(-1.0);

    auto a = Point::fromCartesian(3, 4);
    auto b = Point::fromPolar(5, pi / 2);
    auto c = Point::origin();

    std::cout << "a = " << a << "\nb = " << b << "\nc = " << c << "\n";
    // Point p(1, 2);   // does not compile: constructor is private
}
