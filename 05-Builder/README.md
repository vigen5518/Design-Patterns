# Builder

**Intent:** separate the **construction** of a complex object from its **representation**, so you can build it step by step.

## Files

| File | What it shows |
|------|---------------|
| `bad_telescoping_constructor.cpp` | A constructor with 8 parameters. `HttpRequest(..., false, true, 3, 30)`: which is the timeout? Swapped arguments and invalid objects compile fine. |
| `good_fluent_builder.cpp` | **Fluent builder**: named, chainable setters with defaults, and `build()` validates the result. |
| `good_classic_builder.cpp` | **Classic GoF builder**: a `Director` fixes the order of steps, and concrete builders produce different results (a gaming PC, an office PC, or a price quote). |

## Fluent builder

```cpp
auto req = HttpRequest::Builder("https://api.example.com/orders")
               .method("POST")
               .header("Content-Type", "application/json")
               .body(R"({"item":"book"})")
               .timeout(5)
               .build();          // validates, then returns the finished object
```

Key points:
- Each setter returns `Builder&` (`return *this;`), which is what makes chaining work.
- Required parameters go in the builder's **constructor** (the URL); optional ones get setters.
- `HttpRequest`'s constructor is **private**, so the builder is the only way to make one. An invalid request can't exist.

## Classic builder + Director

```
Director ── calls steps in order ──► ComputerBuilder (interface)
                                       buildCase(), buildCpu(), buildRam(), ...
                                          ▲            ▲             ▲
                                  GamingPcBuilder OfficePcBuilder PriceQuoteBuilder
                                   → Computer      → Computer       → double
```

The **same recipe** (`Director::buildFullPc`) gives completely different results depending on which builder you pass in.

## Builder vs. Factory

| Factory | Builder |
|---------|---------|
| creates the object in **one call** | builds it **step by step** |
| picks **which class** to create | configures **one complex object** |
