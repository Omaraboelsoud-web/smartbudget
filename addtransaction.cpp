#include "addtransaction.h"
#include "ui_addtransaction.h"
#include <QDebug>
#include "Transaction.h"
#include <QDate>
#include "history.h"
#include "statistics.h"

Addtransaction::Addtransaction(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Addtransaction)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, &Addtransaction::onAddClicked);
    connect(ui->historyButton, &QPushButton::clicked, this, &Addtransaction::on_historyButton_clicked);
    connect(ui->Summary, &QPushButton::clicked, this, &Addtransaction::on_summaryButton_clicked);
}

Addtransaction::~Addtransaction()
{
    delete ui;
}

void Addtransaction::onAddClicked() {

    double amount = ui->amountInput->text().toDouble();
    QString type = ui->comboBox->currentText();
    QString category = ui->categoryBox->currentText();

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
        historyWindow = new History();

        historyWindow->setManager(&manager);
        historyWindow->setMainWindow(this);
    }

    historyWindow->setTransactions(manager.getAllTransactions());
    historyWindow->show();
    this->hide();
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
void Addtransaction::on_summaryButton_clicked()
{
    statistics *s = new statistics();

    s->setManager(&manager);
    s->setMainWindow(this);

    s->show();
    this->hide();
}