#include "addtransaction.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // FIXED: removed the orphaned BudgetManager that was created here but
    // never used. Addtransaction owns its own BudgetManager internally.
    Addtransaction w;
    w.show();

    return a.exec();
}