#include "addtransaction.h"
#include "ui_addtransaction.h"
#include <QDebug>
#include "Transaction.h"
#include <QDate>
#include "history.h"

Addtransaction::Addtransaction(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Addtransaction)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, &Addtransaction::onAddClicked);
    connect(ui->historyButton, &QPushButton::clicked,
            this, &Addtransaction::on_historyButton_clicked);
}

Addtransaction::~Addtransaction()
{
    delete ui;
}

void Addtransaction::onAddClicked() {

    double amount = ui->amountInput->text().toDouble();
    QString type = ui->comboBox->currentText();
    QString category = ui->categoryInput->text();

    Transaction t(amount, type, category, QDate::currentDate());
    manager.addTransaction(t);

    ui->incomeLabel->setText(QString::number(manager.getTotalIncome()));
    ui->expenseLabel->setText(QString::number(manager.getTotalExpenses()));
    ui->balanceLabel->setText(QString::number(manager.getBalance()));
    qDebug() << "Transaction added!";
}

void Addtransaction::on_historyButton_clicked()
{
    if (!historyWindow) {
        historyWindow = new History(this);
        historyWindow->setManager(&manager);
        connect(historyWindow, &QWidget::destroyed, this, &Addtransaction::onHistoryClosed);
    }
    historyWindow->setTransactions(manager.getAllTransactions());
    historyWindow->show();
    historyWindow->raise();
    historyWindow->activateWindow();
}
void Addtransaction::refreshLabels()
{
    ui->incomeLabel->setText(QString::number(manager.getTotalIncome()));
    ui->expenseLabel->setText(QString::number(manager.getTotalExpenses()));
    ui->balanceLabel->setText(QString::number(manager.getBalance()));
}
void Addtransaction::onHistoryClosed()
{
    historyWindow = nullptr;
    refreshLabels();
}