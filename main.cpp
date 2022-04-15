#include "SpreadsheetManagerWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SpreadsheetManagerWindow w;
    w.show();
    return a.exec();
}
