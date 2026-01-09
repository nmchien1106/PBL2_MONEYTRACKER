#ifndef ADD_DEBT_DIALOG_H
#define ADD_DEBT_DIALOG_H

#include <QDialog>
#include <QDateTime>
#include <QString>
#include "debt.h"

namespace Ui {
class AddDebtDialog;
}

class AddDebtDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDebtDialog(QWidget *parent = nullptr);
    ~AddDebtDialog();

    // Setters cho edit mode
    void setEditMode(bool isEdit);
    void setDebtData(const Debt& debt);

    // Getters
    QString getType() const;
    QString getCategory() const;
    QString getDebtor() const;
    double getAmount() const;
    QDateTime getDueDate() const;
    QDateTime getDateTime() const;
    QString getDescription() const;
    bool isPaid() const;

private slots:
    void onSubmitClicked();
    void onCancelClicked();
    void validateAmount();

private:
    Ui::AddDebtDialog *ui;
    bool m_isEditMode;

    void setupConnections();
    void loadCategories();
    void setupValidation();
    bool validateForm();
};

#endif // ADD_DEBT_DIALOG_H
