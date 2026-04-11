#ifndef BUDGETMANAGER_H
#define BUDGETMANAGER_H

#include <vector>
#include "Transaction.h"

class BudgetManager {
private:
    std::vector<Transaction> transactions;

public:
    BudgetManager();
    void addTransaction(const Transaction& t);
    void removeTransaction(int index);
    void updateTransaction(int index, double amount, QString type, QString category);
    double getBalance() const;
    double getTotalIncome() const;
    double getTotalExpenses() const;


    std::vector<Transaction> getAllTransactions() const;
};

#endif