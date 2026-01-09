#include "add_saving_dialog.h"
#include "ui_add_saving_dialog.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

AddSavingDialog::AddSavingDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddSavingDialog)
    , m_isEditMode(false)
{
    ui->setupUi(this);

    // Set default date time to now
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());

    setupConnections();
    setupValidation();

    // Focus on name field
    ui->nameLineEdit->setFocus();
}

AddSavingDialog::~AddSavingDialog()
{
    delete ui;
}

void AddSavingDialog::setupConnections()
{
    connect(ui->submitButton, &QPushButton::clicked, this, &AddSavingDialog::onSubmitClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &AddSavingDialog::onCancelClicked);
    connect(ui->targetAmountLineEdit, &QLineEdit::textChanged, this, &AddSavingDialog::validateAmount);
    connect(ui->currentAmountLineEdit, &QLineEdit::textChanged, this, &AddSavingDialog::validateAmount);
}

void AddSavingDialog::setupValidation()
{
    // Only allow numbers for amounts
    QRegularExpression amountRegex("^[0-9]*\\.?[0-9]*$");
    QRegularExpressionValidator *amountValidator = new QRegularExpressionValidator(amountRegex, this);
    ui->targetAmountLineEdit->setValidator(amountValidator);
    ui->currentAmountLineEdit->setValidator(amountValidator);
}

void AddSavingDialog::validateAmount()
{
    // Validate target amount
    QString targetText = ui->targetAmountLineEdit->text();
    if (!targetText.isEmpty()) {
        bool ok;
        double amount = targetText.toDouble(&ok);
        if (ok && amount > 0) {
            ui->targetAmountLineEdit->setStyleSheet(
                "QLineEdit { border: 2px solid #28a745; background: white; }"
            );
        } else {
            ui->targetAmountLineEdit->setStyleSheet(
                "QLineEdit { border: 2px solid #dc3545; background: white; }"
            );
        }
    } else {
        ui->targetAmountLineEdit->setStyleSheet("");
    }

    // Validate current amount
    QString currentText = ui->currentAmountLineEdit->text();
    if (!currentText.isEmpty()) {
        bool ok;
        double amount = currentText.toDouble(&ok);
        if (ok && amount >= 0) {
            ui->currentAmountLineEdit->setStyleSheet(
                "QLineEdit { border: 2px solid #28a745; background: white; }"
            );
        } else {
            ui->currentAmountLineEdit->setStyleSheet(
                "QLineEdit { border: 2px solid #dc3545; background: white; }"
            );
        }
    } else {
        ui->currentAmountLineEdit->setStyleSheet("");
    }
}

bool AddSavingDialog::validateForm()
{
    // Check name
    if (ui->nameLineEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Thông tin thiếu",
            "Vui lòng nhập tên mục tiêu tiết kiệm!");
        ui->nameLineEdit->setFocus();
        return false;
    }

    // Check target amount
    bool ok;
    double targetAmount = ui->targetAmountLineEdit->text().toDouble(&ok);
    if (!ok || targetAmount <= 0) {
        QMessageBox::warning(this, "Số tiền không hợp lệ",
            "Vui lòng nhập số tiền mục tiêu hợp lệ (lớn hơn 0)!");
        ui->targetAmountLineEdit->setFocus();
        return false;
    }

    // Check current amount
    double currentAmount = ui->currentAmountLineEdit->text().toDouble(&ok);
    if (!ok || currentAmount < 0) {
        QMessageBox::warning(this, "Số tiền không hợp lệ",
            "Vui lòng nhập số tiền hiện tại hợp lệ (lớn hơn hoặc bằng 0)!");
        ui->currentAmountLineEdit->setFocus();
        return false;
    }

    // Check if current amount exceeds target
    if (currentAmount > targetAmount) {
        QMessageBox::information(this, "Chúc mừng!",
            "Số tiền hiện tại đã vượt mục tiêu! Bạn có muốn tiếp tục?");
    }

    return true;
}

void AddSavingDialog::onSubmitClicked()
{
    if (!validateForm()) {
        return;
    }

    accept();
}

void AddSavingDialog::onCancelClicked()
{
    reject();
}

void AddSavingDialog::setEditMode(bool isEdit)
{
    m_isEditMode = isEdit;

    if (isEdit) {
        ui->titleLabel->setText("Chỉnh Sửa Mục Tiêu Tiết Kiệm");
        ui->submitButton->setText("Cập nhật");
    } else {
        ui->titleLabel->setText("Thêm Mục Tiêu Tiết Kiệm");
        ui->submitButton->setText("Thêm");
    }
}

void AddSavingDialog::setSavingData(const Saving& saving)
{
    // Set name
    ui->nameLineEdit->setText(saving.getName());

    // Set target amount
    ui->targetAmountLineEdit->setText(QString::number(saving.getTargetAmount(), 'f', 0));

    // Set current amount
    ui->currentAmountLineEdit->setText(QString::number(saving.getCurrentAmount(), 'f', 0));

    // Set date time
    ui->dateTimeEdit->setDateTime(saving.getCreatedAt());

    // Set description
    ui->descriptionTextEdit->setPlainText(saving.getDescription());
}

// Getters
QString AddSavingDialog::getName() const
{
    return ui->nameLineEdit->text().trimmed();
}

double AddSavingDialog::getTargetAmount() const
{
    return ui->targetAmountLineEdit->text().toDouble();
}

double AddSavingDialog::getCurrentAmount() const
{
    return ui->currentAmountLineEdit->text().toDouble();
}

QDateTime AddSavingDialog::getDateTime() const
{
    return ui->dateTimeEdit->dateTime();
}

QString AddSavingDialog::getDescription() const
{
    return ui->descriptionTextEdit->toPlainText().trimmed();
}
