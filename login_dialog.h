#ifndef LOGIN_DIALOG_H
#define LOGIN_DIALOG_H

#include <QDialog>
#include <QMessageBox>

#include "ui_login_dialog.h"

namespace Ui {
class LoginUI;
}

class LoginUI : public QDialog
{
    Q_OBJECT

public:
    explicit LoginUI(QWidget *parent = nullptr);
    ~LoginUI();

private:
    Ui::LoginUI *ui;

signals:
    void LoginSuccess();
private slots:
    void on_connectDashboard_btn_clicked();
};

#endif 
