// LSP violation: a subclass that cannot honour the base class contract.
//
// The contract of Account::withdraw is "takes money out if there is enough".
// A FixedDeposit is "a kind of account" in real life, but it can NOT withdraw
// before maturity, so it throws. Code written for Account now breaks when it
// receives a FixedDeposit -> the subclass is NOT substitutable.

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

class Account {
protected:
    std::string owner_;
    double balance_;
public:
    Account(std::string owner, double balance) : owner_(std::move(owner)), balance_(balance) {}
    virtual ~Account() = default;

    void deposit(double amount) { balance_ += amount; }

    virtual void withdraw(double amount) {
        if (amount > balance_) throw std::runtime_error("insufficient funds");
        balance_ -= amount;
    }
    double balance() const { return balance_; }
    const std::string& owner() const { return owner_; }
};

class CheckingAccount : public Account {
public:
    using Account::Account;
};

class FixedDeposit : public Account {
public:
    using Account::Account;
    void withdraw(double) override {
        // Strengthens the precondition: the caller did nothing wrong, still fails.
        throw std::logic_error("fixed deposit: withdrawal not allowed before maturity");
    }
};

// Written against Account. It is correct for every *real* Account...
void chargeMonthlyFee(std::vector<std::unique_ptr<Account>>& accounts, double fee) {
    for (auto& acc : accounts) {
        acc->withdraw(fee);
        std::cout << acc->owner() << " charged, balance=" << acc->balance() << "\n";
    }
}

int main() {
    std::vector<std::unique_ptr<Account>> accounts;
    accounts.push_back(std::make_unique<CheckingAccount>("Anna", 500));
    accounts.push_back(std::make_unique<FixedDeposit>("Aram", 10000));

    try {
        chargeMonthlyFee(accounts, 2.0);
    } catch (const std::exception& e) {
        // ...but blows up as soon as a FixedDeposit is substituted.
        std::cout << "CRASH: " << e.what() << "\n";
    }
}
