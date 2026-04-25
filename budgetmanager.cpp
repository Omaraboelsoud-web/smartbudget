#include "budgetmanager.h"

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

void BudgetManager::updateTransaction(int index, double amount, QString type, QString category, QString currency)
{
    if (index < 0 || index >= (int)transactions.size())
        return;

    transactions[index] = Transaction(amount, type, category, QDate::currentDate(), currency);
}

std::vector<Transaction> BudgetManager::getAllTransactions() const
{
    return transactions;
}

// ------------------- CORE FIX -------------------
// Always convert EVERYTHING to USD for calculations
double BudgetManager::getBalance() const
{
    double balance = 0;

    for (const Transaction& t : transactions)
    {
        QString type = t.getType().trimmed().toLower();
        double amountUSD = convert(t.getAmount(), t.getCurrency(), "USD");

        if (type == "income")
            balance += amountUSD;
        else if (type == "expense")
            balance -= amountUSD;
    }

    return balance;
}

double BudgetManager::getTotalIncome() const
{
    double total = 0;

    for (const Transaction& t : transactions)
    {
        if (t.getType().trimmed().toLower() == "income")
        {
            total += convert(t.getAmount(), t.getCurrency(), "USD");
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
            total += convert(t.getAmount(), t.getCurrency(), "USD");
        }
    }

    return total;
}

// ------------------- BUDGET -------------------

void BudgetManager::setBudget(const QString& category, double amount)
{
    categoryBudgets[category] = amount;
}

double BudgetManager::getBudget(const QString& category) const
{
    return categoryBudgets.value(category, 0);
}

bool BudgetManager::isOverBudget(const QString& category) const
{
    double budget = getBudget(category);
    double spent = 0;

    for (const Transaction& t : transactions)
    {
        if (t.getType().trimmed().toLower() == "expense" &&
            t.getCategory() == category)
        {
            spent += convert(t.getAmount(), t.getCurrency(), "USD");
        }
    }

    return spent > budget;
}

double BudgetManager::budgetDifference(const QString& category) const
{
    double budget = getBudget(category);
    double spent = 0;

    for (const Transaction& t : transactions)
    {
        if (t.getType().trimmed().toLower() == "expense" &&
            t.getCategory() == category)
        {
            spent += convert(t.getAmount(), t.getCurrency(), "USD");
        }
    }

    return budget - spent;
}


double BudgetManager::convert(double amount, QString from, QString to) const
{
    const double rate = 50.0;

    from = from.trimmed().toUpper();
    to = to.trimmed().toUpper();

    if (from == to)
        return amount;

    if (from == "USD" && to == "EGP")
        return amount * rate;

    if (from == "EGP" && to == "USD")
        return amount / rate;

    return amount;
}