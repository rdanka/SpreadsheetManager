#ifndef SPREADSHEETMANAGERSPREADSHEET_H
#define SPREADSHEETMANAGERSPREADSHEET_H

#include "SpreadsheetManagerCell.h"

#include <QTableWidget>
#include <QObject>
#include <QWidget>

class SpreadsheetManagerSpreadsheet : public QTableWidget
{
    Q_OBJECT
public:
    SpreadsheetManagerSpreadsheet();
    SpreadsheetManagerCell* getCell(int row, int column) const;
    QString getFormula(int row, int column) const;
    void setFormula(int row, int column, QString formula);
    void clearSpreadsheet();
    bool saveFile(QString path);
    bool loadFile(QString path);

    bool isModified() const;

private:
    const int ROW_COUNT = 999;
    const int COLUMN_COUNT = 26;

public slots:
    void onFindNext(QString text, Qt::CaseSensitivity sens);
    void onFindPrev(QString text, Qt::CaseSensitivity sens);
};

#endif // SPREADSHEETMANAGERSPREADSHEET_H
