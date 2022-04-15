#include "FindDialog.h"

FindDialog::FindDialog(QWidget *parent)
    : QDialog(parent)
{
    _label = new QLabel("Find &what:");
    _lineEdit = new QLineEdit;
    _label->setBuddy(_lineEdit);

    _caseCheckBox = new QCheckBox("Match case");
    _backwardCheckBox = new QCheckBox("Search backward");

    _findButton = new QPushButton("&Find");
    _findButton->setDefault(true);
    _findButton->setEnabled(false);

    _closeButton = new QPushButton("Close");;

    QHBoxLayout *topLeftLayout = new QHBoxLayout;
    topLeftLayout->addWidget(_label);
    topLeftLayout->addWidget(_lineEdit);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(topLeftLayout);
    leftLayout->addWidget(_caseCheckBox);
    leftLayout->addWidget(_backwardCheckBox);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(_findButton);
    rightLayout->addWidget(_closeButton);
    rightLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);

    setWindowTitle("Find");
    setFixedHeight(sizeHint().height());

    connect(_lineEdit, &QLineEdit::textChanged, this, &FindDialog::enableFindButton);
    connect(_findButton, &QPushButton::clicked, this, &FindDialog::findClicked);
    connect(_closeButton, &QPushButton::clicked, this, &FindDialog::close);
}

void FindDialog::findClicked()
{
    QString text = _lineEdit->text();
    auto cs = _caseCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
    if (_backwardCheckBox->isChecked()) {
        emit findPrevious(text, cs);
    } else {
        emit findNext(text, cs);
    }
}

void FindDialog::enableFindButton(const QString &text)
{
    _findButton->setEnabled(!text.isEmpty());
}
