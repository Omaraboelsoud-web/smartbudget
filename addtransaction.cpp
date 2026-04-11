#include "addtransaction.h"
#include "ui_addtransaction.h"
#include <QDebug>
#include "Transaction.h"
#include <QDate>

Addtransaction::Addtransaction(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Addtransaction)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, &Addtransaction::onAddClicked);
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
    qDebug() << "Income:" << manager.getTotalIncome();
    qDebug() << "Expenses:" << manager.getTotalExpenses();
    qDebug() << "Balance:" << manager.getBalance();
    qDebug() << "Transaction added!";
}