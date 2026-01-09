#ifndef ADD_SAVING_DIALOG_H
#define ADD_SAVING_DIALOG_H

#include <QDialog>
#include <QDateTime>
#include <QString>
#include "saving.h"

namespace Ui {
class AddSavingDialog;
}

class AddSavingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddSavingDialog(QWidget *parent = nullptr);
    ~AddSavingDialog();

    void setEditMode(bool isEdit);
    void setSavingData(const Saving& saving);

    QString getName() const;
    double getTargetAmount() const;
    double getCurrentAmount() const;
    QDateTime getDateTime() const;
    QString getDescription() const;

private slots:
    void onSubmitClicked();
    void onCancelClicked();
    void validateAmount();

private:
    Ui::AddSavingDialog *ui;
    bool m_isEditMode;

    void setupConnections();
    void setupValidation();
    bool validateForm();
};

#endif // ADD_SAVING_DIALOG_H
