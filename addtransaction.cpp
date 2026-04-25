#include "addtransaction.h"
#include "ui_addtransaction.h"
#include <QDebug>
#include <QDate>
#include "history.h"
#include "statistics.h"
#include "transaction.h"

Addtransaction::Addtransaction(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Addtransaction)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked,
            this, &Addtransaction::onAddClicked);

    connect(ui->historyButton, &QPushButton::clicked,
            this, &Addtransaction::on_historyButton_clicked);

    connect(ui->Summary, &QPushButton::clicked,
            this, &Addtransaction::on_summaryButton_clicked);
    connect(ui->comboDisplayCurrency, &QComboBox::currentTextChanged,
            this, &Addtransaction::onDisplayCurrencyChanged);
}

Addtransaction::~Addtransaction()
{
    delete ui;
}

void Addtransaction::onAddClicked()
{
    double amount = ui->amountInput->text().toDouble();
    QString type = ui->comboBox->currentText();
    QString category = ui->categoryBox->currentText();
    QString currency = ui->comboCurrency->currentText();

    Transaction t(amount, type, category, QDate::currentDate(), currency);
    manager.addTransaction(t);

    refreshLabels();
}

void Addtransaction::on_historyButton_clicked()
{
    if (!historyWindow)
    {
        historyWindow = new History();
        historyWindow->setManager(&manager);
        historyWindow->setMainWindow(this);
    }

    historyWindow->setTransactions(manager.getAllTransactions());
    historyWindow->show();
    this->hide();
}

void Addtransaction::on_summaryButton_clicked()
{
    statistics* s = new statistics();
    s->setManager(&manager);
    s->setMainWindow(this);
    s->show();
    this->hide();
}

void Addtransaction::onHistoryClosed()
{
    historyWindow = nullptr;
    refreshLabels();
}
void Addtransaction::onDisplayCurrencyChanged()
{
    displayCurrency = ui->comboDisplayCurrency->currentText();
    refreshLabels();
}
void Addtransaction::refreshLabels()
{
    double income = manager.getTotalIncome();
    double expense = manager.getTotalExpenses();
    double balance = manager.getBalance();


    if (displayCurrency == "Display-EGP") {
        income *= 50;
        expense *= 50;
        balance *= 50;
    }

    ui->incomeLabel->setText(QString::number(income) + " " + displayCurrency);
    ui->expenseLabel->setText(QString::number(expense) + " " + displayCurrency);
    ui->balanceLabel->setText(QString::number(balance) + " " + displayCurrency);
}