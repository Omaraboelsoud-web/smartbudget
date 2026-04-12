#ifndef BUDGETMANAGER_H
#define BUDGETMANAGER_H
#include <QMap>
#include <vector>
#include "Transaction.h"


class BudgetManager {
private:
    std::vector<Transaction> transactions;
    QMap<QString, double> categoryBudgets;
public:
    BudgetManager();
    void addTransaction(const Transaction& t);
    void removeTransaction(int index);
    void updateTransaction(int index, double amount, QString type, QString category);
    double getBalance() const;
    double getTotalIncome() const;
    double getTotalExpenses() const;
    void setBudget(const QString& category, double amount);
    double getBudget(const QString& category) const;

    std::vector<Transaction> getAllTransactions() const;
};

#endif