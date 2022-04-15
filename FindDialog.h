#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QObject>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QLayout>
#include <QDialog>


class FindDialog : public QDialog
{
    Q_OBJECT
public:
    FindDialog(QWidget *parent = nullptr);

signals:
    void findNext(const QString &str, Qt::CaseSensitivity cs);
    void findPrevious(const QString &str, Qt::CaseSensitivity cs);

private slots:
    void findClicked();
    void enableFindButton(const QString &text);

private:
    QLabel *_label;
    QLineEdit *_lineEdit;
    QCheckBox *_caseCheckBox;
    QCheckBox *_backwardCheckBox;
    QPushButton *_findButton;
    QPushButton *_closeButton;
};

#endif // FINDDIALOG_H
