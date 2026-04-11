#include "BudgetManager.h"

BudgetManager::BudgetManager() {

}

void BudgetManager::addTransaction(const Transaction& t) {
    transactions.push_back(t);
}

void BudgetManager::removeTransaction(int index) {
    if (index >= 0 && index < transactions.size()) {
        transactions.erase(transactions.begin() + index);
    }
}

double BudgetManager::getBalance() const
{
    double balance = 0;

    for (const Transaction& t : transactions)
    {
        QString type = t.getType().trimmed().toLower();

        if (type == "income")
        {
            balance += t.getAmount();
        }
        else if (type == "expense")
        {
            balance -= t.getAmount();
        }
    }

    return balance;
}

double BudgetManager::getTotalIncome() const {
    double total = 0;

    for (const Transaction& t : transactions) {
        if (t.getType() == "income") {
            total += t.getAmount();
        }
    }

    return total;
}

double BudgetManager::getTotalExpenses() const
{
    double total = 0;

    for (const Transaction& t : transactions)
    {
        if (t.getType().trimmed().toLower() == "expense")
        {
            total += t.getAmount();
        }
    }

    return total;
}

std::vector<Transaction> BudgetManager::getAllTransactions() const {
    return transactions;
}