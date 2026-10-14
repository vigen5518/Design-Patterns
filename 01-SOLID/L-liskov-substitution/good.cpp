// LSP fixed: the hierarchy only promises what EVERY subclass can deliver.
//
// Account           - what all accounts can do (deposit, balance)
// WithdrawableAccount - adds withdraw(); only accounts that really allow it inherit it
//
// Any function that takes WithdrawableAccount& can now trust withdraw() works.
// FixedDeposit is still an Account, so code that only needs Account works with it too.

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
    double balance() const { return balance_; }
    const std::string& owner() const { return owner_; }
};

class WithdrawableAccount : public Account {
public:
    using Account::Account;
    void withdraw(double amount) {
        if (amount > balance_) throw std::runtime_error("insufficient funds");
        balance_ -= amount;
    }
};

class CheckingAccount : public WithdrawableAccount {
public:
    using WithdrawableAccount::WithdrawableAccount;
};

class SavingsAccount : public WithdrawableAccount {
public:
    using WithdrawableAccount::WithdrawableAccount;
};

class FixedDeposit : public Account {
    int monthsToMaturity_;
public:
    FixedDeposit(std::string owner, double balance, int months)
        : Account(std::move(owner), balance), monthsToMaturity_(months) {}
    int monthsToMaturity() const { return monthsToMaturity_; }
};

// Requires withdraw() -> asks for exactly that abstraction. No surprises.
void chargeMonthlyFee(std::vector<WithdrawableAccount*>& accounts, double fee) {
    for (auto* acc : accounts) {
        acc->withdraw(fee);
        std::cout << acc->owner() << " charged, balance=" << acc->balance() << "\n";
    }
}

// Needs only Account -> every account type works, FixedDeposit included.
void addInterest(std::vector<Account*>& accounts, double rate) {
    for (auto* acc : accounts) {
        acc->deposit(acc->balance() * rate);
        std::cout << acc->owner() << " +interest, balance=" << acc->balance() << "\n";
    }
}

int main() {
    CheckingAccount anna("Anna", 500);
    SavingsAccount  gor("Gor", 1200);
    FixedDeposit    aram("Aram", 10000, 12);

    std::vector<WithdrawableAccount*> withdrawable{&anna, &gor};
    chargeMonthlyFee(withdrawable, 2.0);

    std::vector<Account*> all{&anna, &gor, &aram};
    addInterest(all, 0.01);
}
