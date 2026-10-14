// BAD: the "Point problem" - why constructors are not always enough.
//
// We want to create a point from Cartesian (x, y) OR Polar (r, theta) coordinates.
// The natural attempt does not even compile, because both have the same signature:
//
//     Point(double x, double y);
//     Point(double r, double theta);   // error: cannot be overloaded
//
// The usual "workaround" below compiles, but it is ugly and error-prone:
//  - parameter names a/b mean nothing,
//  - caller must remember which enum goes with which meaning,
//  - adding a new system (e.g. Grid) means editing the constructor (violates OCP).

#include <cmath>
#include <iomanip>
#include <iostream>

enum class CoordSystem { Cartesian, Polar };

class Point {
public:
    double x, y;

    Point(double a, double b, CoordSystem system = CoordSystem::Cartesian) {
        if (system == CoordSystem::Cartesian) {
            x = a;
            y = b;
        } else {
            x = a * std::cos(b);
            y = a * std::sin(b);
        }
    }
};

int main() {
    std::cout << std::fixed << std::setprecision(2);
    const double pi = std::acos(-1.0);

    Point p1(3, 4);                                 // ok, Cartesian
    Point p2(5, pi / 2, CoordSystem::Polar);        // ok, Polar
    Point p3(5, pi / 2);                            // BUG: meant Polar, forgot the flag -> (5, 1.57)

    std::cout << "p1 = (" << p1.x << ", " << p1.y << ")\n";
    std::cout << "p2 = (" << p2.x << ", " << p2.y << ")\n";
    std::cout << "p3 = (" << p3.x << ", " << p3.y << ")  <- silently wrong\n";
}
