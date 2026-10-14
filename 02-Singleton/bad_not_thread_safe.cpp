// BAD #2: classic "lazy pointer" Singleton (pre-C++11 style).
//
// Problems:
//  1. Data race: two threads can both see instance_ == nullptr and both call `new`.
//     -> two instances + a memory leak. (Simulated below with a sleep to make it visible.)
//  2. Never deleted: destructor never runs (files not flushed, connections not closed).
//  3. Copy/move not deleted (same bug as bad_copyable.cpp).

#include <chrono>
#include <iostream>
#include <thread>

class Logger {
    static Logger* instance_;
    Logger() { std::cout << "Logger constructed at " << this << "\n"; }
public:
    static Logger* getInstance() {
        if (instance_ == nullptr) {                                   // thread A checks...
            std::this_thread::sleep_for(std::chrono::milliseconds(50)); // ...thread B checks too
            instance_ = new Logger();                                 // both create!
        }
        return instance_;
    }
};

Logger* Logger::instance_ = nullptr;

int main() {
    Logger* a = nullptr;
    Logger* b = nullptr;

    std::thread t1([&] { a = Logger::getInstance(); });
    std::thread t2([&] { b = Logger::getInstance(); });
    t1.join();
    t2.join();

    std::cout << "same object? " << (a == b ? "yes" : "NO - race created two loggers!") << "\n";
}
