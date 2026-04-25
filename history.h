#ifndef HISTORY_H
#define HISTORY_H
#include <vector>
#include "transaction.h"
#include <QWidget>
#include "budgetmanager.h"
class Addtransaction;
namespace Ui {
class History;
}

class History : public QWidget
{
    Q_OBJECT
private slots:
    void on_deleteButton_clicked();
    void on_backButton_clicked();
    void on_editButton_clicked();
public:
    explicit History(QWidget *parent = nullptr);
    void setManager(BudgetManager* m);
    void setMainWindow(Addtransaction* w);
    ~History();
public:
    void setTransactions(const std::vector<Transaction>& transactions);
private:
    Ui::History *ui;
    BudgetManager* manager;
    void refreshTable();
    Addtransaction* mainWindow = nullptr;
};

#endif