# Design Patterns in C++

SOLID principles and the core creational and structural design patterns, written in modern C++ (C++17).

Every topic includes:
- a **bad** version that compiles and runs, so you can watch the problem happen,
- one or more **good** versions that fix it,
- a **README** covering the problem, the solution, when to use it, and common mistakes.

Each `.cpp` file is self-contained and has its own `main()`.

## Contents

| # | Topic | Example domain | Files |
|---|-------|----------------|-------|
| 1 | [SOLID](01-SOLID/) | shop orders, shipping, bank accounts, printers, notifications | 5 × (`bad.cpp`, `good.cpp`) |
| 2 | [Singleton](02-Singleton/) | config, logger | copyable bug, thread race, Meyers' Singleton |
| 3 | [Factory](03-Factory/) | `Point` (Cartesian/Polar), logistics | static factory methods, inner factory, Factory Method |
| 4 | [Abstract Factory](04-AbstractFactory/) | light/dark UI themes | mixed families vs. one factory per family |
| 5 | [Builder](05-Builder/) | HTTP request, computer assembly | telescoping constructor, fluent builder, classic builder + director |
| 6 | [Prototype](06-Prototype/) | shapes in a drawing editor | `dynamic_cast` chain and shallow copy vs. `clone()` and a registry |
| 7 | [Composite](07-Composite/) | file system tree | two lists and duplicated loops vs. one uniform interface |

## Build & run

**Linux / macOS / MinGW (g++ or clang++):**

```bash
make            # builds every example into ./bin
make run        # builds and runs all of them
./bin/05-Builder_good_fluent_builder
```

**Any platform with CMake (including Visual Studio):**

```bash
cmake -S . -B build
cmake --build build
```

**A single file, by hand:**

```bash
g++ -std=c++17 -Wall -Wextra -pthread 02-Singleton/good_meyers.cpp -o meyers && ./meyers
```

## Pattern categories

| Category | Question it answers | Patterns here |
|----------|---------------------|---------------|
| **Creational** | *How* are objects created? | Singleton, Factory, Abstract Factory, Builder, Prototype |
| **Structural** | How are objects *composed* into bigger structures? | Composite |

## Which pattern when?

| You need... | Use |
|-------------|-----|
| exactly one shared instance (config, logger) | **Singleton** |
| readable creation with different meanings for the same arguments | **Factory (static methods)** |
| subclasses to decide which object to create | **Factory Method** |
| a whole family of objects that must match | **Abstract Factory** |
| an object with many optional parameters, or one built in steps | **Builder** |
| a copy of an object whose concrete type you don't know | **Prototype** |
| tree structures where leaves and groups are treated the same way | **Composite** |
