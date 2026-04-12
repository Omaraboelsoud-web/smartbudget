#ifndef STATISTICS_H
#define STATISTICS_H
#include "budgetmanager.h"
#include <QWidget>
#include "addtransaction.h"

class Addtransaction;
namespace Ui {
class statistics;
}

class statistics : public QWidget
{
    Q_OBJECT

public:
    explicit statistics(QWidget *parent = nullptr);
    void setManager(BudgetManager* m);
    ~statistics();
    void setMainWindow(Addtransaction* w);

private:
    Ui::statistics *ui;
    BudgetManager* manager = nullptr;
    void updateStats();
    Addtransaction* mainWindow = nullptr;

private slots:
    void on_back1Button_clicked();
};


#endif // STATISTICS_H
