#include "history.h"
#include "ui_history.h"
#include "addtransaction.h"
#include "transaction.h"
#include <QInputDialog>
#include <QMessageBox>

History::History(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::History)
{
    ui->setupUi(this);

    connect(ui->deleteButton, &QPushButton::clicked,
            this, &History::on_deleteButton_clicked);
    connect(ui->backButton,   &QPushButton::clicked,
            this, &History::on_backButton_clicked);
    connect(ui->editButton,   &QPushButton::clicked,
            this, &History::on_editButton_clicked);
    connect(ui->filterBox,    &QComboBox::currentTextChanged,
            this, &History::refreshTable);

    // FIXED: removed manager->applyRecurringTransactions() from here.
    // manager is nullptr at construction time — moved to setManager().
}

History::~History()
{
    delete ui;
}

void History::setManager(BudgetManager* m)
{
    manager = m;
    if (manager)
        manager->applyRecurringTransactions(QDate::currentDate());
}

void History::setMainWindow(Addtransaction* w)
{
    mainWindow = w;
}

void History::refreshTable()
{
    ui->historyTable->setRowCount(0);
    if (!manager) return;

    QString filter = ui->filterBox->currentText().trimmed().toLower();
    QString displayCur = manager->getDisplayCurrency();
    auto transactions = manager->getAllTransactions();

    for (const Transaction& t : transactions)
    {
        QString type = t.getType().trimmed().toLower();

        if (filter != "all" && type != filter)
            continue;

        int row = ui->historyTable->rowCount();
        ui->historyTable->insertRow(row);

        double displayAmt = manager->convertToDisplay(t.getAmount(), t.getCurrency());
        QString amountStr = QString::number(displayAmt, 'f', 2) + " " + displayCur;

        ui->historyTable->setItem(row, 0,
                                  new QTableWidgetItem(amountStr));
        ui->historyTable->setItem(row, 1,
                                  new QTableWidgetItem(t.getCategory()));
        ui->historyTable->setItem(row, 2,
                                  new QTableWidgetItem(t.getType()));
        ui->historyTable->setItem(row, 3,
                                  new QTableWidgetItem(t.getDate().toString("yyyy-MM-dd")));
        ui->historyTable->setItem(row, 4,
                                  new QTableWidgetItem(t.getCurrency()));
    }
}

void History::on_backButton_clicked()
{
    this->close();
    if (mainWindow)
    {
        mainWindow->refreshLabels();
        mainWindow->show();
    }
}

void History::on_deleteButton_clicked()
{
    int row = ui->historyTable->currentRow();
    if (row < 0)
    {
        QMessageBox::information(this, "Delete", "Please select a row to delete.");
        return;
    }
    if (!manager) return;

    QString filter = ui->filterBox->currentText().trimmed().toLower();
    auto transactions = manager->getAllTransactions();

    int managerIndex = -1;
    int visibleCount = 0;
    for (int i = 0; i < (int)transactions.size(); ++i)
    {
        QString type = transactions[i].getType().trimmed().toLower();
        if (filter != "all" && type != filter) continue;
        if (visibleCount == row) { managerIndex = i; break; }
        ++visibleCount;
    }

    if (managerIndex < 0) return;

    manager->removeTransaction(managerIndex);
    refreshTable();

    if (mainWindow) mainWindow->refreshLabels();
}

void History::on_editButton_clicked()
{
    int row = ui->historyTable->currentRow();
    if (row < 0)
    {
        QMessageBox::information(this, "Edit", "Please select a row to edit.");
        return;
    }
    if (!manager) return;

    QString filter = ui->filterBox->currentText().trimmed().toLower();
    auto transactions = manager->getAllTransactions();

    int managerIndex = -1;
    int visibleCount = 0;
    for (int i = 0; i < (int)transactions.size(); ++i)
    {
        QString type = transactions[i].getType().trimmed().toLower();
        if (filter != "all" && type != filter) continue;
        if (visibleCount == row) { managerIndex = i; break; }
        ++visibleCount;
    }
    if (managerIndex < 0) return;

    bool ok = false;
    QString amountStr = QInputDialog::getText(this, "Edit Transaction",
                                              "New Amount:", QLineEdit::Normal,
                                              QString::number(transactions[managerIndex].getAmount()),
                                              &ok);
    if (!ok) return;

    QString type = QInputDialog::getText(this, "Edit Transaction",
                                         "New Type (income / expense):", QLineEdit::Normal,
                                         transactions[managerIndex].getType(), &ok);
    if (!ok) return;

    QString category = QInputDialog::getText(this, "Edit Transaction",
                                             "New Category:", QLineEdit::Normal,
                                             transactions[managerIndex].getCategory(), &ok);
    if (!ok) return;

    double  amount   = amountStr.toDouble();
    QString currency = transactions[managerIndex].getCurrency();

    manager->updateTransaction(managerIndex, amount, type, category, currency);
    refreshTable();

    if (mainWindow) mainWindow->refreshLabels();
}