#include "SpreadsheetManagerSpreadsheet.h"

#include <QFile>
#include <QMessageBox>
#include <QApplication>
#include <QTextStream>

SpreadsheetManagerSpreadsheet::SpreadsheetManagerSpreadsheet(): QTableWidget()
{
    setItemPrototype(new SpreadsheetManagerCell);
    setSelectionMode(ContiguousSelection);
    clearSpreadsheet();
}

SpreadsheetManagerCell *SpreadsheetManagerSpreadsheet::getCell(int row, int column) const
{
    return static_cast<SpreadsheetManagerCell*>(item(row, column));
}

QString SpreadsheetManagerSpreadsheet::getFormula(int row, int column) const
{
    auto cell = getCell(row, column);
    return cell ? cell->getFormula() : "";
}

void SpreadsheetManagerSpreadsheet::setFormula(int row, int column, QString formula)
{
    auto cell = getCell(row, column);
    if(!cell) {
        cell = new SpreadsheetManagerCell();
        setItem(row, column, cell);
    }
    cell->setFormula(formula);
}

void SpreadsheetManagerSpreadsheet::clearSpreadsheet()
{
    clear();
    setRowCount(ROW_COUNT);
    setColumnCount(COLUMN_COUNT);

    for(int i=0; i< COLUMN_COUNT; i++) {
        auto item = new QTableWidgetItem();
        item->setText(QString(QChar('A' + i)));
        setHorizontalHeaderItem(i, item);
    }
    setCurrentCell(0, 0);
}

bool SpreadsheetManagerSpreadsheet::saveFile(QString path)
{
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream writer(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    for(int i=0; i<ROW_COUNT; i++) {
        for(int j=0; j<COLUMN_COUNT; j++) {
            auto field = getFormula(i, j);
            if(!field.isEmpty()) {
                writer << i << " " << j << " " << field << "\n";
            }
        }
    }
    QApplication::restoreOverrideCursor();
    return true;
}

bool SpreadsheetManagerSpreadsheet::loadFile(QString path)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    clear();

    QTextStream reader(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    int row, col;
    QString val;
    while (!reader.atEnd()) {
        reader >> row >> col >> val;
        setFormula(row, col, val);
    }

    QApplication::restoreOverrideCursor();
    return true;
}

void SpreadsheetManagerSpreadsheet::onFindNext(QString text, Qt::CaseSensitivity sens)
{
    int row = currentRow();
    int col = currentColumn() + 1;

    while(row < ROW_COUNT) {
        while(col < COLUMN_COUNT) {
            if(getFormula(row, col).contains(text, sens)){
                clearSelection();
                setCurrentCell(row, col);
                activateWindow();
                return;
            }
            col++;
        }
        col = 0;
        row++;
    }
    QApplication::beep();
}

void SpreadsheetManagerSpreadsheet::onFindPrev(QString text, Qt::CaseSensitivity sens)
{
    int row = currentRow();
    int col = currentColumn() - 1;

    while(row >= 0) {
        while(col >= 0) {
            if(getFormula(row, col).contains(text, sens)){
                clearSelection();
                setCurrentCell(row, col);
                activateWindow();
                return;
            }
            col--;
        }
        col = COLUMN_COUNT-1;
        row--;
    }
    QApplication::beep();
}
