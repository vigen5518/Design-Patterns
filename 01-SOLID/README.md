# SOLID Principles

Five principles for object-oriented code that is easy to change. Robert C. Martin collected them under one name.

| Letter | Principle | One-line rule | Example |
|--------|-----------|---------------|---------|
| **S** | Single Responsibility | A class should have **one reason to change**. | [Order / receipt](S-single-responsibility/) |
| **O** | Open/Closed | Open for **extension**, closed for **modification**. | [Shipping methods](O-open-closed/) |
| **L** | Liskov Substitution | A subclass must be usable **anywhere** its base class is. | [Bank accounts](L-liskov-substitution/) |
| **I** | Interface Segregation | Don't force clients to depend on methods they **don't use**. | [Printer / scanner / fax](I-interface-segregation/) |
| **D** | Dependency Inversion | Depend on **abstractions**, not on concrete classes. | [Order notifications](D-dependency-inversion/) |

Each folder has a `bad.cpp` that breaks the principle and a `good.cpp` that follows it. Both compile and run.

---

## S: Single Responsibility Principle

**Problem (`bad.cpp`):** `Order` stores the data, calculates the tax and discount, prints the receipt, *and* saves itself to the database. When the accounting, UI, or database team needs a change, they all edit the same class.

**Fix (`good.cpp`):** Split it into `Order` (data), `PriceCalculator` (rules), `ReceiptPrinter` (output), and `OrderRepository` (storage).

> "Responsibility" means **a reason to change**, not "one method". A class can have many methods and still have one responsibility.

## O: Open/Closed Principle

**Problem:** Adding a shipping method means editing an `enum` and **every** `switch` that uses it. If you miss one, you get a runtime bug.

**Fix:** Make `ShippingMethod` an interface with one class per method. `DroneShipping` is added as a new class, and no existing line changes.

> Red flag: a `switch`/`if-else` on a *type code* that appears in more than one place.

## L: Liskov Substitution Principle

**Problem:** `FixedDeposit` inherits `Account::withdraw()` but throws. `chargeMonthlyFee(vector<Account>)` is correct for every *real* account, yet it crashes once a `FixedDeposit` shows up.

**Fix:** Move `withdraw()` into `WithdrawableAccount`. A function that needs withdrawals asks for `WithdrawableAccount`, and a function that only needs a balance asks for `Account`.

> LSP violations usually look like this: a subclass **throws "not supported"**, **does nothing**, or needs a `dynamic_cast` check. Inheritance must model *behaviour*, not just real-world "is-a".
> The textbook example is Square/Rectangle: a square "is a" rectangle in math, but `setWidth()` on a `Square` breaks the expectations callers have of `Rectangle`.

## I: Interface Segregation Principle

**Problem:** The fat interface `IMachine` has `print`, `scan`, and `fax`. A `SimplePrinter` has to implement `scan()` (it throws) and `fax()` (it silently does nothing).

**Fix:** Split it into `IPrinter`, `IScanner`, and `IFax`. A multifunction printer implements all three, and a simple printer implements only `IPrinter`. Calling `scan` on a simple printer is now a **compile-time error**.

> ISP is LSP's partner: fat interfaces *force* LSP violations.

## D: Dependency Inversion Principle

**Problem:** The business class `OrderService` creates `EmailSender` itself. You can't switch to SMS, and you can't test it without sending real e-mail.

**Fix:** `OrderService` depends on the `INotifier` interface, and the concrete notifier is **injected** through the constructor. `FakeNotifier` makes it unit-testable.

```
bad:   OrderService ───────────► EmailSender
good:  OrderService ──► INotifier ◄── EmailNotifier / SmsNotifier / FakeNotifier
```

> DIP is a *principle*. **Dependency Injection** (passing dependencies in through the constructor) is the most common *technique* for applying it.
