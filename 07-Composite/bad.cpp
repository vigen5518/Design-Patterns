// BAD: a tree where the client must treat leaves and containers differently.
//
// A Folder keeps files and sub-folders in TWO separate lists, and every
// operation (size, print, count...) needs two loops + its own recursion.
// Adding a third kind of node (e.g. Shortcut) means editing every operation.
// Client code also has to know "is it a file or a folder?" before calling anything.

#include <iostream>
#include <string>
#include <vector>

struct File {
    std::string name;
    long sizeKb;
};

struct Folder {
    std::string name;
    std::vector<File> files;
    std::vector<Folder> subfolders;
};

long totalSize(const Folder& f) {
    long sum = 0;
    for (const auto& file : f.files) sum += file.sizeKb;          // loop #1 for leaves
    for (const auto& sub : f.subfolders) sum += totalSize(sub);   // loop #2 for containers
    return sum;
}

void print(const Folder& f, int indent = 0) {
    std::cout << std::string(indent, ' ') << f.name << "/\n";
    for (const auto& sub : f.subfolders) print(sub, indent + 2);  // same split again
    for (const auto& file : f.files)
        std::cout << std::string(indent + 2, ' ') << file.name << " (" << file.sizeKb << " KB)\n";
}

int main() {
    Folder src{"src", {{"main.cpp", 4}, {"utils.cpp", 6}}, {}};
    Folder docs{"docs", {{"README.md", 2}}, {}};
    Folder root{"project", {{"Makefile", 1}}, {src, docs}};

    print(root);
    std::cout << "Total: " << totalSize(root) << " KB\n";

    // A single file and a folder have NO common interface:
    File lonely{"notes.txt", 3};
    // totalSize(lonely);   // does not compile - client must special-case files
    std::cout << "notes.txt: " << lonely.sizeKb << " KB (handled separately)\n";
}
