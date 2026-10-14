// GOOD: Composite - treat single objects and groups of objects UNIFORMLY.
//
//              FileSystemNode (component)
//              + size()  + print()  + count()
//                 /                   \
//            File (leaf)        Folder (composite)
//                               holds vector<FileSystemNode>
//                               and delegates to its children
//
// The client calls node.size() and does not care if it is a file or a folder.
// Adding a new node type (Shortcut) = one new class; Folder and the client stay unchanged.

#include <iostream>
#include <memory>
#include <string>
#include <vector>

class FileSystemNode {
protected:
    std::string name_;
public:
    explicit FileSystemNode(std::string name) : name_(std::move(name)) {}
    virtual ~FileSystemNode() = default;

    virtual long size() const = 0;
    virtual int fileCount() const = 0;
    virtual void print(int indent = 0) const = 0;
};

// Leaf
class File : public FileSystemNode {
    long sizeKb_;
public:
    File(std::string name, long sizeKb) : FileSystemNode(std::move(name)), sizeKb_(sizeKb) {}

    long size() const override { return sizeKb_; }
    int fileCount() const override { return 1; }
    void print(int indent) const override {
        std::cout << std::string(indent, ' ') << name_ << " (" << sizeKb_ << " KB)\n";
    }
};

// Another leaf, added later - nothing else had to change.
class Shortcut : public FileSystemNode {
    std::string target_;
public:
    Shortcut(std::string name, std::string target)
        : FileSystemNode(std::move(name)), target_(std::move(target)) {}

    long size() const override { return 0; }
    int fileCount() const override { return 0; }
    void print(int indent) const override {
        std::cout << std::string(indent, ' ') << name_ << " -> " << target_ << "\n";
    }
};

// Composite
class Folder : public FileSystemNode {
    std::vector<std::unique_ptr<FileSystemNode>> children_;
public:
    explicit Folder(std::string name) : FileSystemNode(std::move(name)) {}

    Folder& add(std::unique_ptr<FileSystemNode> child) {
        children_.push_back(std::move(child));
        return *this;
    }

    long size() const override {
        long sum = 0;
        for (const auto& c : children_) sum += c->size();   // one loop, no type checks
        return sum;
    }
    int fileCount() const override {
        int n = 0;
        for (const auto& c : children_) n += c->fileCount();
        return n;
    }
    void print(int indent) const override {
        std::cout << std::string(indent, ' ') << name_ << "/  [" << size() << " KB]\n";
        for (const auto& c : children_) c->print(indent + 2);
    }
};

// Client code: works with ANY node.
void report(const FileSystemNode& node) {
    node.print(0);
    std::cout << "=> " << node.fileCount() << " file(s), " << node.size() << " KB\n\n";
}

int main() {
    auto src = std::make_unique<Folder>("src");
    src->add(std::make_unique<File>("main.cpp", 4))
        .add(std::make_unique<File>("utils.cpp", 6));

    auto docs = std::make_unique<Folder>("docs");
    docs->add(std::make_unique<File>("README.md", 2))
        .add(std::make_unique<Shortcut>("spec.lnk", "//server/spec.pdf"));

    Folder root("project");
    root.add(std::make_unique<File>("Makefile", 1))
        .add(std::move(src))
        .add(std::move(docs));

    report(root);                         // a whole tree
    report(File("notes.txt", 3));         // a single file - same function, same call
}
