#include "budgetmanager.h"
#include <QDate>

BudgetManager::BudgetManager() {}

void BudgetManager::addTransaction(const Transaction& t)
{
    transactions.push_back(t);
}

void BudgetManager::removeTransaction(int index)
{
    if (index >= 0 && index < (int)transactions.size())
        transactions.erase(transactions.begin() + index);
}

void BudgetManager::updateTransaction(int index, double amount,
                                      QString type, QString category,
                                      QString currency)
{
    if (index < 0 || index >= (int)transactions.size()) return;
    transactions[index] = Transaction(amount, type, category,
                                      QDate::currentDate(), currency);
}

std::vector<Transaction> BudgetManager::getAllTransactions() const
{
    return transactions;
}

// ---------------- CORE ----------------

double BudgetManager::getBalance() const
{
    double balance = 0;
    for (const Transaction& t : transactions)
    {
        QString type = t.getType().trimmed().toLower();
        double  amt  = convertToDisplay(t.getAmount(), t.getCurrency());
        if      (type == "income")  balance += amt;
        else if (type == "expense") balance -= amt;
    }
    return balance;
}

double BudgetManager::getTotalIncome() const
{
    double total = 0;
    for (const Transaction& t : transactions)
        if (t.getType().trimmed().toLower() == "income")
            total += convertToDisplay(t.getAmount(), t.getCurrency());
    return total;
}

double BudgetManager::getTotalExpenses() const
{
    double total = 0;
    for (const Transaction& t : transactions)
        if (t.getType().trimmed().toLower() == "expense")
            total += convertToDisplay(t.getAmount(), t.getCurrency());
    return total;
}

// ---------------- BUDGET ----------------

void BudgetManager::setBudget(const QString& category, double amount)
{
    categoryBudgets[category] = amount;
}

double BudgetManager::getBudget(const QString& category) const
{
    return categoryBudgets.value(category, 0.0);
}

bool BudgetManager::isOverBudget(const QString& category) const
{
    return budgetDifference(category) < 0;
}

double BudgetManager::budgetDifference(const QString& category) const
{
    double budget = getBudget(category);
    double spent  = 0;
    for (const Transaction& t : transactions)
        if (t.getType().trimmed().toLower() == "expense" &&
            t.getCategory() == category)
            spent += convertToDisplay(t.getAmount(), t.getCurrency());
    return budget - spent;
}

// ---------------- CURRENCY ----------------

double BudgetManager::convert(double amount, QString from, QString to) const
{
    const double rate = 50.0;   // 1 USD = 50 EGP
    from = from.trimmed().toUpper();
    to   = to.trimmed().toUpper();

    if (from == to)              return amount;
    if (from == "USD" && to == "EGP") return amount * rate;
    if (from == "EGP" && to == "USD") return amount / rate;
    return amount;
}

double BudgetManager::convertToDisplay(double amount,
                                       const QString& fromCurrency) const
{
    return convert(amount, fromCurrency, displayCurrency);
}

void BudgetManager::setDisplayCurrency(const QString& currency)
{
    displayCurrency = currency.trimmed().toUpper();
}

QString BudgetManager::getDisplayCurrency() const
{
    return displayCurrency;
}
