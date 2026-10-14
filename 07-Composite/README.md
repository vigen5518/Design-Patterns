# Composite

**Intent:** compose objects into **tree structures**, and let clients treat **single objects and groups the same way**.

## The problem (`bad.cpp`)

```cpp
struct Folder {
    std::vector<File>   files;        // leaves
    std::vector<Folder> subfolders;   // containers
};
```

- Every operation (`totalSize`, `print`, ...) needs **two loops**: one for files, one for folders.
- A new node type (a shortcut, a symlink) means editing every operation.
- A `File` and a `Folder` share no interface, so `totalSize(file)` doesn't compile and the client has to special-case files.

## The solution (`good.cpp`)

```
         FileSystemNode            <- component (common interface)
         + size() + fileCount() + print()
          ▲          ▲          ▲
        File      Shortcut    Folder            <- leaves / composite
                               children: vector<unique_ptr<FileSystemNode>>
```

- `Folder::size()` is a single loop: `sum += child->size()`. It doesn't care what the child is.
- `report(const FileSystemNode&)` works on a whole tree **and** on a lone file.
- `Shortcut` was added without touching `Folder` or the client.

## Roles

| Role | Here | Responsibility |
|------|------|----------------|
| Component | `FileSystemNode` | the common interface |
| Leaf | `File`, `Shortcut` | does the real work, has no children |
| Composite | `Folder` | stores children and **delegates** to them |

## Other real-world uses

UI widget trees (a panel contains buttons and panels), org charts (a department contains employees and departments), graphics groups, menus and submenus, arithmetic expression trees.

## Design choice: where does `add()` go?

- **In the composite only** (as here): type-safe, since you can't call `add()` on a file.
- **In the component**: clients treat everything uniformly, but `File::add()` has to throw. That is a small LSP violation, which is why the type-safe version is used here.
