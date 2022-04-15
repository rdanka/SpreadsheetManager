#ifndef SPREADSHEETMANAGERCELL_H
#define SPREADSHEETMANAGERCELL_H

#include <QTableWidgetItem>
#include <QObject>
#include <QWidget>

class SpreadsheetManagerCell : public QTableWidgetItem
{
public:
    SpreadsheetManagerCell();
    void setFormula(const QString& formula);
    QString getFormula() const;
};

#endif // SPREADSHEETMANAGERCELL_H
