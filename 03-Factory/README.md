# Factory

"Factory" covers several related ideas. All of them **separate creating an object from using it**.

## Files

| File | Kind |
|------|------|
| `bad_point_constructors.cpp` | The **Point problem**: constructors can't be named, so we get a confusing enum flag. |
| `good_static_factory_methods.cpp` | **Static factory methods**: `Point::fromCartesian`, `Point::fromPolar`. |
| `good_inner_factory.cpp` | **Inner factory**: the creators are grouped in the nested class `Point::Factory`. |
| `bad_switch_everywhere.cpp` | Business logic does `new Truck` / `new Ship` itself, inside a type switch. |
| `good_factory_method.cpp` | **Factory Method (GoF)**: a virtual `createTransport()` that subclasses override. |

## 1. The Point problem

```cpp
class Point {
    Point(double x, double y);       // Cartesian
    Point(double r, double theta);   // Polar   <- error: same signature!
};
```

Constructors all share the class's name, so two constructors with the same parameter types can't coexist. The "fix" with an extra `enum` parameter works, but it is easy to misuse: forget the flag and you get the wrong point with no warning (see the `p3` output in the bad example).

## 2. Static factory methods

```cpp
auto a = Point::fromCartesian(3, 4);
auto b = Point::fromPolar(5, pi / 2);
```

- The name says what the arguments mean.
- The constructor becomes `private`, so the factories are the only way in.
- You can add more creators (`origin()`, `fromGrid()`) freely.

## 3. Inner factory

The same idea, but the creators live in a nested `Point::Factory` class. A nested class is a member, so it can call the private constructor **without `friend`**. This is useful when there are many creation methods, or when the factory needs its own state.

## 4. Factory Method (GoF)

```
Logistics                         Transport
+ planDelivery()  ── uses ──►     + deliver()
+ createTransport() = 0               ▲
       ▲                       Truck  Ship  Plane
RoadLogistics → Truck
SeaLogistics  → Ship
AirLogistics  → Plane
```

The base class holds the **algorithm** (`planDelivery`) and leaves **which object to create** to a virtual method that subclasses override. Adding air delivery means adding a class; the algorithm never changes (Open/Closed).

## Simple Factory vs. Factory Method vs. Abstract Factory

| | Creates | Extension point |
|---|---|---|
| Simple / static factory | one product, picked by a parameter | edit the factory function |
| **Factory Method** | one product | override a virtual method in a subclass |
| **Abstract Factory** | a *family* of related products | add a new factory class |
