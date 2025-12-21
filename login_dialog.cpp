#include "login_dialog.h"
#include "ui_login_dialog.h"
#include "app.h"

LoginUI::LoginUI(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginUI)
{
    ui->setupUi(this);

}

LoginUI::~LoginUI()
{
    delete ui;
}



void LoginUI::on_connectDashboard_btn_clicked()
{
    emit LoginSuccess();
}

