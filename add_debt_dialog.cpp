#include "add_debt_dialog.h"
#include "ui_add_debt_dialog.h"
#include "app.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QTime>

AddDebtDialog::AddDebtDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddDebtDialog)
    , m_isEditMode(false)
{
    ui->setupUi(this);

    // Set default date time to now
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->dueDateEdit->setDate(QDate::currentDate().addDays(30)); // Default 30 days from now

    setupConnections();
    loadCategories();
    setupValidation();

    // Focus on debtor field
    ui->debtorLineEdit->setFocus();
}

AddDebtDialog::~AddDebtDialog()
{
    delete ui;
}

void AddDebtDialog::setupConnections()
{
    connect(ui->submitButton, &QPushButton::clicked, this, &AddDebtDialog::onSubmitClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &AddDebtDialog::onCancelClicked);
    connect(ui->amountLineEdit, &QLineEdit::textChanged, this, &AddDebtDialog::validateAmount);
}

void AddDebtDialog::loadCategories()
{
    ui->categoryComboBox->clear();

    // Get categories from App
    const QVector<Category>& categories = App::getCategoryList();

    for (const Category& cat : categories) {
        // Only show "Income" categories for debt (can be both borrowed and lent money)
        if (cat.getType() == "Income") {
            ui->categoryComboBox->addItem(
                cat.getName(),
                cat.getID()
            );
        }
    }

    if (ui->categoryComboBox->count() == 0) {
        ui->categoryComboBox->addItem("⚠️ Không có danh mục", "");
    }
}

void AddDebtDialog::setupValidation()
{
    // Only allow numbers for amount
    QRegularExpression amountRegex("^[0-9]*\\.?[0-9]*$");
    QRegularExpressionValidator *amountValidator = new QRegularExpressionValidator(amountRegex, this);
    ui->amountLineEdit->setValidator(amountValidator);
}

void AddDebtDialog::validateAmount()
{
    QString text = ui->amountLineEdit->text();
    if (!text.isEmpty()) {
        bool ok;
        double amount = text.toDouble(&ok);
        if (ok && amount > 0) {
            // Format the amount with thousand separators
            ui->amountLineEdit->setStyleSheet(
                "QLineEdit { border: 2px solid #28a745; background: white; }"
            );
        } else {
            ui->amountLineEdit->setStyleSheet(
                "QLineEdit { border: 2px solid #dc3545; background: white; }"
            );
        }
    } else {
        ui->amountLineEdit->setStyleSheet("");
    }
}

bool AddDebtDialog::validateForm()
{
    // Check debtor name
    if (ui->debtorLineEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Thông tin thiếu",
            "Vui lòng nhập tên người liên quan!");
        ui->debtorLineEdit->setFocus();
        return false;
    }

    // Check amount
    bool ok;
    double amount = ui->amountLineEdit->text().toDouble(&ok);
    if (!ok || amount <= 0) {
        QMessageBox::warning(this, "Số tiền không hợp lệ",
            "Vui lòng nhập số tiền hợp lệ (lớn hơn 0)!");
        ui->amountLineEdit->setFocus();
        return false;
    }

    // Check category
    if (ui->categoryComboBox->currentData().toString().isEmpty()) {
        QMessageBox::warning(this, "Chưa chọn danh mục",
            "Vui lòng chọn danh mục cho khoản nợ!");
        ui->categoryComboBox->setFocus();
        return false;
    }

    // Check due date must be after creation date
    if (ui->dueDateEdit->date() < ui->dateTimeEdit->date()) {
        QMessageBox::warning(this, "Ngày không hợp lệ",
            "Ngày đáo hạn phải sau thời gian tạo!");
        ui->dueDateEdit->setFocus();
        return false;
    }

    return true;
}

void AddDebtDialog::onSubmitClicked()
{
    if (!validateForm()) {
        return;
    }

    accept();
}

void AddDebtDialog::onCancelClicked()
{
    reject();
}

void AddDebtDialog::setEditMode(bool isEdit)
{
    m_isEditMode = isEdit;

    if (isEdit) {
        ui->titleLabel->setText("Chỉnh Sửa Khoản Nợ");
        ui->submitButton->setText("Cập nhật");
    } else {
        ui->titleLabel->setText("Thêm Khoản Nợ Mới");
        ui->submitButton->setText("Thêm");
    }
}

void AddDebtDialog::setDebtData(const Debt& debt)
{
    // Set type
    if (debt.getType() == "borrow") {
        ui->typeComboBox->setCurrentIndex(0);
    } else {
        ui->typeComboBox->setCurrentIndex(1);
    }

    // Set category
    if (debt.getCategory()) {
        QString categoryID = debt.getCategory()->getID();
        int index = ui->categoryComboBox->findData(categoryID);
        if (index >= 0) {
            ui->categoryComboBox->setCurrentIndex(index);
        }
    }

    // Set debtor
    ui->debtorLineEdit->setText(debt.getDebtorName());

    // Set amount
    ui->amountLineEdit->setText(QString::number(debt.getAmount(), 'f', 0));

    // Set due date
    ui->dueDateEdit->setDate(debt.getDueDate());

    // Set date time
    ui->dateTimeEdit->setDateTime(debt.getCreatedAt());

    // Set description
    ui->descriptionTextEdit->setPlainText(debt.getDescription());

    // Set paid status
    ui->paidCheckBox->setChecked(debt.getIsPaid());
}

// Getters
QString AddDebtDialog::getType() const
{
    return ui->typeComboBox->currentIndex() == 0 ? "borrow" : "lend";
}

QString AddDebtDialog::getCategory() const
{
    return ui->categoryComboBox->currentData().toString();
}

QString AddDebtDialog::getDebtor() const
{
    return ui->debtorLineEdit->text().trimmed();
}

double AddDebtDialog::getAmount() const
{
    return ui->amountLineEdit->text().toDouble();
}

QDateTime AddDebtDialog::getDueDate() const
{
    return QDateTime(ui->dueDateEdit->date(), QTime(0, 0));
}

QDateTime AddDebtDialog::getDateTime() const
{
    return ui->dateTimeEdit->dateTime();
}

QString AddDebtDialog::getDescription() const
{
    return ui->descriptionTextEdit->toPlainText().trimmed();
}

bool AddDebtDialog::isPaid() const
{
    return ui->paidCheckBox->isChecked();
}
