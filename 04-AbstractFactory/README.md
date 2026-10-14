# Abstract Factory

**Intent:** provide an interface for creating **families of related objects** without naming their concrete classes.

## Example: UI themes

| | Button | Checkbox |
|---|---|---|
| **Light** family | `LightButton` | `LightCheckbox` |
| **Dark** family | `DarkButton` | `DarkCheckbox` |

### `bad.cpp`

Every place that creates a widget checks `if (dark)` again. One inverted check produces a **dark button next to a light checkbox**, and nothing catches it. A third theme means editing every `if` in the program.

### `good.cpp`

```
           ThemeFactory
           + createButton()
           + createCheckbox()
            ▲            ▲
 LightThemeFactory   DarkThemeFactory
```

- The theme is chosen **once**, in `makeTheme()`, the only place that names concrete classes.
- `LoginForm` receives a `ThemeFactory&` and knows only `Button` and `Checkbox`.
- A factory can only produce matching products, so **mixing families is impossible**.
- A new theme (HighContrast) means one factory class plus its widgets. `LoginForm` doesn't change.

## When to use

- The system must work with several **families** of products (OS look-and-feel, database drivers, game difficulty levels).
- Products in a family **must be used together**.

## Trade-off

Adding a new theme is easy, but adding a new **product kind** (e.g. `Slider`) is hard: every factory needs a new `createSlider()`.
