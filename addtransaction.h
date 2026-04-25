#ifndef ADDTRANSACTION_H
#define ADDTRANSACTION_H
#include <QWidget>
#include "budgetmanager.h"
#include "history.h"

namespace Ui {
class History;
class Addtransaction;
}

class Addtransaction : public QWidget
{
    Q_OBJECT
private slots:
    void onAddClicked();
    void on_historyButton_clicked();
    void onHistoryClosed();
    void on_summaryButton_clicked();
    void onDisplayCurrencyChanged();
public:
    explicit Addtransaction(QWidget *parent = nullptr);
    ~Addtransaction();

private:
    Ui::Addtransaction *ui;
    BudgetManager manager;
    History* historyWindow = nullptr;
    void refreshLabels();
    QString displayCurrency = "Display-USD";

};

#endif