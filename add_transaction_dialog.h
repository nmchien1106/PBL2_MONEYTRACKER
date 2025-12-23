#ifndef ADD_TRANSACTION_DIALOG_H
#define ADD_TRANSACTION_DIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QDateTimeEdit>
#include <QPushButton>
#include "app.h"

namespace Ui {
class AddTransactionDialog;
}

class AddTransactionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddTransactionDialog(const QString& transactionType, QWidget *parent = nullptr);
    ~AddTransactionDialog();

private slots:
    void onSubmitClicked();
    void onCancelClicked();
    void onManageCategoryClicked();

private:
    Ui::AddTransactionDialog *ui;
    QString transactionType;

    void loadCategories();
    void setupConnections();
    void applyTypeSpecificStyles();
    bool validateInputs();
};

#endif
