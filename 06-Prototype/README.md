# Prototype

**Intent:** create new objects by **copying an existing object** (the prototype), without depending on its concrete class.

## The problem (`bad.cpp`)

The editor's "Duplicate" command holds a `Shape*`. To copy it, you need the concrete type:

```cpp
if (auto c = dynamic_cast<const Circle*>(s))    return make_unique<Circle>(*c);
if (auto r = dynamic_cast<const Rectangle*>(s)) return make_unique<Rectangle>(*r);
return nullptr;   // a new Triangle class falls through here
```

There is a second bug as well: `Shape` holds a raw `Style*`. The default copy is **shallow**, so the copy and the original share one `Style`. Recolouring the copy recolours the original.

## The solution (`good.cpp`)

```cpp
class Shape {
public:
    virtual std::unique_ptr<Shape> clone() const = 0;
};
class Circle : public Shape {
public:
    std::unique_ptr<Shape> clone() const override {
        return std::make_unique<Circle>(*this);   // uses Circle's copy constructor
    }
};
```

- Each class copies **itself**, so the copy always has the right concrete type.
- `Shape`'s copy constructor does a **deep copy** of the owned `Style`.
- **PrototypeRegistry** stores pre-configured templates (`"button"`, `"warning"`, ...) and stamps out copies with `registry.create("button")`.

## Shallow vs. deep copy

| Shallow | Deep |
|---------|------|
| copies the **pointer** | copies the **pointed-to object** |
| copy and original share data | fully independent |
| the default for raw pointers | you write it (copy constructor), or use value members |

> Rule of thumb: if a class owns a resource (`unique_ptr`, raw `new`), write a copy constructor that deep-copies it. Otherwise `= delete` copying.

## When to use

- Objects are expensive to create from scratch, but cheap to copy.
- You need copies of objects you only know through a base-class pointer.
- You want a set of preset configurations to clone from.
