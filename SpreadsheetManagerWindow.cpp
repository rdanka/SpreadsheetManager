#include "SpreadsheetManagerWindow.h"

#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QToolBar>
#include <QClipboard>
#include <QtDebug>
#include <QInputDialog>
#include <QCloseEvent>

SpreadsheetManagerWindow::SpreadsheetManagerWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowState(Qt::WindowMaximized);
    setWindowIcon(QIcon(":/actions/tablazatkezelo_images/icon.png"));
    setWindowTitle("Spreadsheet Manager[*]");
    _file = "";
    _findDialog = new FindDialog(this);
    initializeMenu();
    initializeActions();
    initializeSpreadsheet();
    connectSlots();
}

SpreadsheetManagerWindow::~SpreadsheetManagerWindow()
{
}

void SpreadsheetManagerWindow::initializeActions()
{
    _actionNew = new QAction(QIcon(":/actions/tablazatkezelo_images/new.png"), "New", this);
    _actionNew->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    _actionNew->setStatusTip("Create a new file.");
    _fileToolBar->addAction(_actionNew);
    _fileMenu->addAction(_actionNew);

    _actionOpen = new QAction(QIcon(":/actions/tablazatkezelo_images/open.png"), "Open", this);
    _actionOpen->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    _actionOpen->setStatusTip("Open an existing file.");
    _fileToolBar->addAction(_actionOpen);
    _fileMenu->addAction(_actionOpen);

    _actionSave = new QAction(QIcon(":/actions/tablazatkezelo_images/save.png"), "Save", this);
    _actionSave->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    _actionSave->setStatusTip("Save current file.");
    _fileToolBar->addAction(_actionSave);
    _fileMenu->addAction(_actionSave);

    _actionCut = new QAction(QIcon(":/actions/tablazatkezelo_images/cut.png"), "Cut", this);
    _actionCut->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
    _actionCut->setStatusTip("Cut current selection.");
    _editToolBar->addAction(_actionCut);
    _editMenu->addAction(_actionCut);

    _actionCopy = new QAction(QIcon(":/actions/tablazatkezelo_images/copy.png"), "Copy", this);
    _actionCopy->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
    _actionCopy->setStatusTip("Copy current selection.");
    _editToolBar->addAction(_actionCopy);
    _editMenu->addAction(_actionCopy);

    _actionPaste = new QAction(QIcon(":/actions/tablazatkezelo_images/paste.png"), "Paste", this);
    _actionPaste->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
    _actionPaste->setStatusTip("Paste clipboard content.");
    _editToolBar->addAction(_actionPaste);
    _editMenu->addAction(_actionPaste);

    _actionSearch = new QAction(QIcon(":/actions/tablazatkezelo_images/find.png"), "Search", this);
    _actionSearch->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F));
    _actionSearch->setStatusTip("Search for text in spreadsheet.");
    _toolsToolBar->addAction(_actionSearch);
    _toolMenu->addAction(_actionSearch);

    _actionGoto = new QAction(QIcon(":/actions/tablazatkezelo_images/gotocell.png"), "Go to", this);
    _actionGoto->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
    _actionGoto->setStatusTip("Go to a specific cell in the spreadsheet.");
    _toolsToolBar->addAction(_actionGoto);
    _toolMenu->addAction(_actionGoto);

    _actionDel = new QAction(this);
    _actionDel->setShortcut(QKeySequence(Qt::Key_Delete));
    addAction(_actionDel);
}

void SpreadsheetManagerWindow::initializeMenu()
{
    _fileToolBar = new QToolBar("File", this);
    _editToolBar = new QToolBar("Edit", this);
    _toolsToolBar = new QToolBar("Tools", this);
    addToolBar(_fileToolBar);
    addToolBar(_editToolBar);
    addToolBar(_toolsToolBar);

    _menuBar = new QMenuBar(this);
    _fileMenu = new QMenu("File", this);
    _editMenu = new QMenu("Edit", this);
    _toolMenu = new QMenu("Tools", this);
    _menuBar->addMenu(_fileMenu);
    _menuBar->addMenu(_editMenu);
    _menuBar->addMenu(_toolMenu);
    setMenuBar(_menuBar);
}

void SpreadsheetManagerWindow::initializeSpreadsheet()
{
    _spreadsheet = new SpreadsheetManagerSpreadsheet();
    setCentralWidget(_spreadsheet);
}

void SpreadsheetManagerWindow::connectSlots()
{
    connect(_actionSave, &QAction::triggered, this, &SpreadsheetManagerWindow::onSaveAction);
    connect(_actionOpen,  &QAction::triggered, this, &SpreadsheetManagerWindow::onLoadAction);
    connect(_actionNew, &QAction::triggered, this, &SpreadsheetManagerWindow::onNewAction);
    connect(_actionCopy, &QAction::triggered, this, &SpreadsheetManagerWindow::onCopyAction);
    connect(_actionPaste, &QAction::triggered, this, &SpreadsheetManagerWindow::onPasteAction);
    connect(_actionCut, &QAction::triggered, this, &SpreadsheetManagerWindow::onCutAction);
    connect(_actionDel, &QAction::triggered, this, &SpreadsheetManagerWindow::onDelAction);
    connect(_actionSearch, &QAction::triggered, this, &SpreadsheetManagerWindow::onSearchAction);
    connect(_actionGoto, &QAction::triggered, this, &SpreadsheetManagerWindow::onGotoAction);
    connect(_findDialog, &FindDialog::findNext, _spreadsheet, &SpreadsheetManagerSpreadsheet::onFindNext);
    connect(_findDialog, &FindDialog::findPrevious, _spreadsheet, &SpreadsheetManagerSpreadsheet::onFindPrev);
    connect(_spreadsheet, &SpreadsheetManagerSpreadsheet::itemChanged, this, [=](){setWindowModified(true);});
}

void SpreadsheetManagerWindow::closeEvent(QCloseEvent *event)
{
    if(isWindowModified()) {
        auto result = QMessageBox::question(this,
                              "Start new spreadsheet",
                              "Would you like to save the current spreadsheet before starting a new one?",
                              QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if(result == QMessageBox::Cancel) {
            event->ignore();
            return;
        }
        if(result == QMessageBox::Yes) {
            onSaveAction();
        }
    }
    event->accept();
}

void SpreadsheetManagerWindow::onSaveAction()
{
    auto path = _file.isEmpty() ? QFileDialog::getSaveFileName(this, "Save spreadsheet") : _file;
    if(!path.isEmpty() && !_spreadsheet->saveFile(path)) {
        QMessageBox::warning(this, "Error", "Unable to save spreadsheet");
        return;
    }
    _file = path;
    setWindowModified(false);
}

void SpreadsheetManagerWindow::onLoadAction()
{
    if(isWindowModified()) {
        auto result = QMessageBox::question(this,
                              "Start new spreadsheet",
                              "Would you like to save the current spreadsheet before starting a new one?",
                              QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if(result == QMessageBox::Cancel) {
            return;
        }
        if(result == QMessageBox::Yes) {
            onSaveAction();
        }
    }
    auto path = QFileDialog::getOpenFileName(this, "Open spreadsheet");
    if(!path.isEmpty() && !_spreadsheet->loadFile(path)) {
        QMessageBox::warning(this, "Error", "Unable to load spreadsheet");
        return;
    }
    _file = path;
}

void SpreadsheetManagerWindow::onNewAction()
{
    if(isWindowModified()) {
        auto result = QMessageBox::question(this,
                              "Start new spreadsheet",
                              "Would you like to save the current spreadsheet before starting a new one?",
                              QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if(result == QMessageBox::Cancel) {
            return;
        }
        if(result == QMessageBox::Yes) {
            onSaveAction();
        }
    }
    _spreadsheet->clear();
    _file = "";
    setWindowModified(false);
}

void SpreadsheetManagerWindow::onCopyAction()
{
    auto ranges = _spreadsheet->selectedRanges();
    auto range = ranges.first();
    QString str = "";

    for(int i=0; i< range.rowCount(); i++) {
        for(int j=0; j<range.columnCount(); j++) {
            str += _spreadsheet->getFormula(range.topRow()+i, range.leftColumn()+j);
            str += "\t";
        }
        str.chop(1);
        str += "\n";
    }
    str.chop(1);

    QApplication::clipboard()->setText(str);
}

void SpreadsheetManagerWindow::onPasteAction()
{
    auto str = QApplication::clipboard()->text();
    auto rows = str.split("\n");

    auto selection = _spreadsheet->selectedRanges();
    if(selection.isEmpty()) {
        return;
    }

    int startRow = selection.first().topRow();
    int startCol = selection.first().leftColumn();

    for(int i=startRow; i<startRow+rows.length(); i++) {
        auto columns = rows[i - startRow].split("\t");
        for(int j=startCol; j< startCol+columns.length(); j++) {
            QString formula = columns[j - startCol];
            _spreadsheet->setFormula(i, j, formula);
        }
    }
}

void SpreadsheetManagerWindow::onCutAction()
{
    onCopyAction();
    onDelAction();
}

void SpreadsheetManagerWindow::onDelAction()
{
    foreach(auto item, _spreadsheet->selectedItems()) {
        delete item;
    }
}

void SpreadsheetManagerWindow::onSearchAction()
{
    _findDialog->show();
}

void SpreadsheetManagerWindow::onGotoAction()
{
    auto text = QInputDialog::getText(this,
                                      "Go to Cell",
                                      "Cell location:").toUpper();
    QRegExp regExp("[A-Za-z][1-9][0-9]{0,2}");

    if(!regExp.exactMatch(text)) {
        QMessageBox::warning(this, "Error", "Invalid cell!");
        return;
    }
    _spreadsheet->setCurrentCell(text.midRef(1).toInt() - 1,
                                 text[0].unicode() - 'A');
}


