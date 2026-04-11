#ifndef ADDTRANSACTION_H
#define ADDTRANSACTION_H
#include <QWidget>
#include "BudgetManager.h"
namespace Ui {
class Addtransaction;
}

class Addtransaction : public QWidget
{
    Q_OBJECT
private slots:
    void onAddClicked();

public:
    explicit Addtransaction(QWidget *parent = nullptr);
    ~Addtransaction();

private:
    Ui::Addtransaction *ui;
     BudgetManager manager;
};

#endif
