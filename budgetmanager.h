#ifndef BUDGETMANAGER_H
#define BUDGETMANAGER_H

#include <vector>
#include <QString>
#include <QMap>
#include "transaction.h"

class BudgetManager {
private:
    std::vector<Transaction> transactions;
    QMap<QString, double> categoryBudgets;

public:
    BudgetManager();

    void addTransaction(const Transaction& t);
    void removeTransaction(int index);
    void updateTransaction(int index, double amount, QString type, QString category, QString currency);
    std::vector<Transaction> getAllTransactions() const;
    double getBalance() const;
    double getTotalIncome() const;
    double getTotalExpenses() const;
    double convert(double amount, QString from, QString to) const;
    void setBudget(const QString& category, double amount);
    double getBudget(const QString& category) const;
    bool isOverBudget(const QString& category) const;
    double budgetDifference(const QString& category) const;
};

#endif