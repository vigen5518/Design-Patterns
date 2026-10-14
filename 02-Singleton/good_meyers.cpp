// GOOD: Meyers' Singleton (Scott Meyers, "Effective C++").
//
//  - private constructor           -> nobody else can create one
//  - copy & move deleted           -> nobody can duplicate it (fixes bad_copyable.cpp)
//  - function-local static         -> created on first use (lazy),
//                                     thread-safe since C++11 ("magic statics"),
//                                     destroyed automatically at program exit.

#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

class Logger {
    std::mutex mutex_;
    int lineNo_ = 0;

    Logger() { std::cout << "Logger constructed (exactly once)\n"; }
    ~Logger() { std::cout << "Logger destroyed, " << lineNo_ << " lines written\n"; }

public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

    static Logger& instance() {
        static Logger inst;   // C++11 guarantees: initialized once, even with many threads
        return inst;
    }

    // The single instance is shared, so its own state must be protected too.
    void log(const std::string& msg) {
        std::lock_guard<std::mutex> lock(mutex_);
        std::cout << ++lineNo_ << ": " << msg << "\n";
    }
};

int main() {
    std::vector<std::thread> workers;
    for (int i = 0; i < 4; ++i)
        workers.emplace_back([i] { Logger::instance().log("hello from thread " + std::to_string(i)); });
    for (auto& t : workers) t.join();

    std::cout << "same object? "
              << (&Logger::instance() == &Logger::instance() ? "yes" : "no") << "\n";

    // Logger copy = Logger::instance();   // does not compile: copy constructor is deleted
}
