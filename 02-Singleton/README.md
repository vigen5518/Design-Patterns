# Singleton

**Intent:** make sure a class has **exactly one instance** and provide a global access point to it.

**Typical uses:** configuration, logger, connection pool, hardware access (one printer spooler, one GPU context).

## Files

| File | What it shows |
|------|---------------|
| `bad_copyable.cpp` | Private constructor, but copy is **not deleted**. `Config c = Config::getInstance();` (missing `&`) silently creates a second instance. |
| `bad_not_thread_safe.cpp` | Lazy `new` with a static pointer. Two threads both see `nullptr` and both create an instance (the race is made visible with a `sleep`). It also leaks: the destructor never runs. |
| `good_meyers.cpp` | **Meyers' Singleton**: function-local `static`, copy and move deleted, thread-safe, destroyed automatically. |

## The correct C++ Singleton checklist

```cpp
class Logger {
    Logger() = default;                          // 1. private constructor
    ~Logger() = default;
public:
    Logger(const Logger&) = delete;              // 2. no copy
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;                   // 3. no move
    Logger& operator=(Logger&&) = delete;

    static Logger& instance() {                  // 4. return a REFERENCE
        static Logger inst;                      // 5. function-local static
        return inst;
    }
};
```

**Why is the function-local static thread-safe?** Since C++11 the standard guarantees ("magic statics") that if several threads reach `static Logger inst;` at the same time, exactly one initializes it and the others wait.

**Why return a reference and not a pointer?** A reference can't be `nullptr`, and nobody is tempted to `delete` it.

## Eager vs. lazy

| | Eager (`static Logger inst;` as a class member) | Lazy (Meyers) |
|---|---|---|
| Created | before `main()` | on the first `instance()` call |
| Cost if never used | always paid | never paid |
| Init-order problems between globals in different `.cpp` files | **yes** ("static initialization order fiasco") | no |

## Criticism / when NOT to use it

- It is **global state** in disguise: hidden dependencies, and code is harder to reason about.
- It is **hard to unit-test**: you can't swap in a fake.
- The singleton's *creation* is thread-safe, but its **methods are not**. `good_meyers.cpp` still needs a `mutex` inside `log()`.
- It often violates SRP: the class manages its own lifetime *and* does its job.

A common alternative is to create one object in `main()` and **pass it in** (dependency injection, see SOLID D).
