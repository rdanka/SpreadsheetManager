#ifndef SPREADSHEETMANAGERWINDOW_H
#define SPREADSHEETMANAGERWINDOW_H

#include "FindDialog.h"
#include "SpreadsheetManagerSpreadsheet.h"

#include <QMainWindow>

class SpreadsheetManagerWindow : public QMainWindow
{
    Q_OBJECT

public:
    SpreadsheetManagerWindow(QWidget *parent = nullptr);
    ~SpreadsheetManagerWindow();

private:
    QToolBar* _fileToolBar;
    QToolBar* _editToolBar;
    QToolBar* _toolsToolBar;

    QMenuBar* _menuBar;
    QMenu* _fileMenu;
    QMenu* _editMenu;
    QMenu* _toolMenu;

    QAction* _actionNew;
    QAction* _actionOpen;
    QAction* _actionSave;
    QAction* _actionCut;
    QAction* _actionCopy;
    QAction* _actionPaste;
    QAction* _actionSearch;
    QAction* _actionGoto;
    QAction* _actionDel;

    QString _file;

    SpreadsheetManagerSpreadsheet* _spreadsheet;

    FindDialog* _findDialog;

    void initializeActions();
    void initializeMenu();
    void initializeSpreadsheet();
    void connectSlots();
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onSaveAction();
    void onLoadAction();
    void onNewAction();
    void onCopyAction();
    void onPasteAction();
    void onCutAction();
    void onDelAction();
    void onSearchAction();
    void onGotoAction();
};
#endif // SPREADSHEETMANAGERWINDOW_H
