#include "statistics.h"
#include "ui_statistics.h"
#include "budgetmanager.h"
#include "transaction.h"
#include "addtransaction.h"
#include <QDate>
#include <QMap>
#include <QLabel>
#include <QLineEdit>

statistics::statistics(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::statistics)
{
    ui->setupUi(this);

    connect(ui->back1Button, &QPushButton::clicked,
            this, &statistics::on_back1Button_clicked);

    // FIXED: removed manager->applyRecurringTransactions() from here.
    // manager is nullptr at construction time — moved to setManager().

    const auto budgetInputs = {
        ui->budgetUtilities,    ui->budgetGroceries,
        ui->budgetRent,         ui->budgetFood,
        ui->budgetEntertainment,ui->budgetEducation,
        ui->budgetHealth,       ui->budgetTransportation,
        ui->budgetOther
    };
    for (QLineEdit* input : budgetInputs)
        connect(input, &QLineEdit::textChanged, this, &statistics::updateStats);
}

statistics::~statistics()
{
    delete ui;
}

void statistics::setManager(BudgetManager* m)
{
    manager = m;
    if (manager)
        manager->applyRecurringTransactions(QDate::currentDate());
    updateStats();
}

void statistics::setMainWindow(Addtransaction* w)
{
    mainWindow = w;
}

void statistics::updateCategoryRow(const QString& category,
                                   const QMap<QString, double>& totals,
                                   QLabel*    spentLabel,
                                   QLineEdit* budgetInput,
                                   QLabel*    remainingLabel,
                                   const QString& currency)
{
    double spent = totals.value(category, 0.0);
    spentLabel->setText(QString::number(spent, 'f', 2) + " " + currency);

    bool   ok     = false;
    double budget = budgetInput->text().toDouble(&ok);
    if (ok && budget > 0)
        manager->setBudget(category, budget);

    double remaining = manager->budgetDifference(category);
    remainingLabel->setText(QString::number(remaining, 'f', 2) + " " + currency);

    if (remaining < 0)
        remainingLabel->setStyleSheet("color: red;");
    else
        remainingLabel->setStyleSheet("color: green;");
}

void statistics::updateStats()
{
    if (!manager) return;

    QDate now = QDate::currentDate();
    QString currency = manager->getDisplayCurrency();

    QMap<QString, double> categoryExpenses;
    double monthIncome   = 0.0;
    double monthExpenses = 0.0;
    int    totalCount    = 0;

    for (const Transaction& t : manager->getAllTransactions())
    {
        QString type   = t.getType().trimmed().toLower();
        double  amount = manager->convertToDisplay(t.getAmount(), t.getCurrency());
        ++totalCount;

        if (t.getDate().month() == now.month() &&
            t.getDate().year()  == now.year())
        {
            if (type == "income")  monthIncome   += amount;
            if (type == "expense") monthExpenses += amount;
        }

        if (type == "expense")
            categoryExpenses[t.getCategory()] += amount;
    }

    ui->monthIncome  ->setText(QString::number(monthIncome,               'f', 2) + " " + currency);
    ui->monthExpenses->setText(QString::number(monthExpenses,             'f', 2) + " " + currency);
    ui->monthSaving  ->setText(QString::number(monthIncome - monthExpenses,'f', 2) + " " + currency);
    ui->totalTransactions->setText(QString::number(totalCount));

    QString highestCat;
    double  highestAmt = -1.0;
    for (auto it = categoryExpenses.constBegin(); it != categoryExpenses.constEnd(); ++it)
    {
        if (it.value() > highestAmt)
        {
            highestAmt = it.value();
            highestCat = it.key();
        }
    }
    ui->Highestspending->setText(highestCat.isEmpty() ? "N/A" : highestCat);

    updateCategoryRow("Utilities",      categoryExpenses,
                      ui->Utilities,      ui->budgetUtilities,      ui->remainingUtilities,      currency);
    updateCategoryRow("Groceries",      categoryExpenses,
                      ui->Groceries,      ui->budgetGroceries,      ui->remainingGroceries,      currency);
    updateCategoryRow("Rent",           categoryExpenses,
                      ui->Rent,           ui->budgetRent,           ui->remainingRent,           currency);
    updateCategoryRow("Food",           categoryExpenses,
                      ui->Food,           ui->budgetFood,           ui->remainingFood,           currency);
    updateCategoryRow("Entertainment",  categoryExpenses,
                      ui->Entertainment,  ui->budgetEntertainment,  ui->remainingEntertainment,  currency);
    updateCategoryRow("Education",      categoryExpenses,
                      ui->Education,      ui->budgetEducation,      ui->remainingEducation,      currency);
    updateCategoryRow("Health",         categoryExpenses,
                      ui->Health,         ui->budgetHealth,         ui->remainingHealth,         currency);
    updateCategoryRow("Transportation", categoryExpenses,
                      ui->Transportation, ui->budgetTransportation, ui->remainingTransportation, currency);
    updateCategoryRow("Other",          categoryExpenses,
                      ui->Other,          ui->budgetOther,          ui->remainingOther,          currency);
}

void statistics::on_back1Button_clicked()
{
    this->close();
    if (mainWindow)
    {
        mainWindow->refreshLabels();
        mainWindow->show();
    }
}