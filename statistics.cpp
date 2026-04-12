#include "statistics.h"
#include "ui_statistics.h"

#include <QMap>
#include <QDate>

#include "budgetmanager.h"
#include "transaction.h"
#include "addtransaction.h"

statistics::statistics(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::statistics)
{
    ui->setupUi(this);

    connect(ui->back1Button, &QPushButton::clicked,
            this, &statistics::on_back1Button_clicked);
}

void statistics::setManager(BudgetManager* m)
{
    manager = m;
    updateStats();
}

void statistics::updateStats()
{
    if (!manager) return;

    double monthIncome = 0;
    double monthExpenses = 0;

    QDate now = QDate::currentDate();

    QMap<QString, double> categoryTotals;

    for (const Transaction& t : manager->getAllTransactions())
    {
        QString type = t.getType().trimmed().toLower();
        double amount = t.getAmount();

        // Monthly summary
        if (t.getDate().month() == now.month() &&
            t.getDate().year() == now.year())
        {
            if (type == "income")
                monthIncome += amount;
            else if (type == "expense")
                monthExpenses += amount;
        }

        // Category totals
        categoryTotals[t.getCategory()] += amount;
    }

    int totalTransactions = manager->getAllTransactions().size();
    double monthBalance = monthIncome - monthExpenses;

    // Highest category
    QString highestCategory = "None";
    double highestValue = 0;

    for (auto it = categoryTotals.begin(); it != categoryTotals.end(); ++it)
    {
        if (it.value() > highestValue)
        {
            highestValue = it.value();
            highestCategory = it.key();
        }
    }

    // UI updates
    ui->monthIncome->setText(QString::number(monthIncome));
    ui->monthExpenses->setText(QString::number(monthExpenses));
    ui->monthSaving->setText(QString::number(monthBalance));
    ui->totalTransactions->setText(QString::number(totalTransactions));
    ui->Highestspending->setText(highestCategory);

    ui->Education->setText(QString::number(categoryTotals["Education"]));
    ui->Entertainment->setText(QString::number(categoryTotals["Entertainment"]));
    ui->Food->setText(QString::number(categoryTotals["Food"]));
    ui->Groceries->setText(QString::number(categoryTotals["Groceries"]));
    ui->Health->setText(QString::number(categoryTotals["Health"]));
    ui->Other->setText(QString::number(categoryTotals["Other"]));
    ui->Rent->setText(QString::number(categoryTotals["Rent"]));
    ui->Transportation->setText(QString::number(categoryTotals["Transport"]));
    ui->Utilities->setText(QString::number(categoryTotals["Utilities"]));
}

void statistics::setMainWindow(Addtransaction* w)
{
    mainWindow = w;
}

void statistics::on_back1Button_clicked()
{
    this->close();

    if (mainWindow)
        mainWindow->show();
}

statistics::~statistics()
{
    delete ui;
}
