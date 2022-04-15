#include "SpreadsheetManagerCell.h"

SpreadsheetManagerCell::SpreadsheetManagerCell(): QTableWidgetItem()
{

}

void SpreadsheetManagerCell::setFormula(const QString &formula)
{
    setData(Qt::EditRole, formula);
}

QString SpreadsheetManagerCell::getFormula() const
{
    return data(Qt::EditRole).toString();
}
