#include "add_transaction_dialog.h"
#include "ui_add_transaction_dialog.h"
#include <QMessageBox>
#include <QDateTime>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDialog>
#include <QListWidget>

AddTransactionDialog::AddTransactionDialog(const QString& transactionType, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddTransactionDialog)
    , transactionType(transactionType)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    if (transactionType == "expense") {
        setWindowTitle("Thêm Chi Tiêu");
        ui->titleLabel->setText("Thêm Chi Tiêu Mới");
    } else {
        setWindowTitle("Thêm Thu Nhập");
        ui->titleLabel->setText("Thêm Thu Nhập Mới");
    }

    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());

    loadCategories();
    setupConnections();
    applyTypeSpecificStyles();
}

AddTransactionDialog::~AddTransactionDialog()
{
    delete ui;
}

void AddTransactionDialog::loadCategories()
{
    ui->categoryComboBox->clear();

    for (const Category& category : App::getCategoryList()) {
        QString categoryType = category.getType();
        if ((transactionType == "expense" && categoryType == "Expense") ||
            (transactionType == "income" && categoryType == "Income") ||
            categoryType == "system") {
            ui->categoryComboBox->addItem(category.getName(), category.getID());
        }
    }
}

void AddTransactionDialog::setupConnections()
{
    connect(ui->submitButton, &QPushButton::clicked, this, &AddTransactionDialog::onSubmitClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &AddTransactionDialog::onCancelClicked);
    connect(ui->manageCategoryButton, &QPushButton::clicked, this, &AddTransactionDialog::onManageCategoryClicked);
}

void AddTransactionDialog::applyTypeSpecificStyles()
{
    QString submitStyle;
    if (transactionType == "expense") {
        submitStyle = R"(
            QPushButton#submitButton {
                background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                            stop:0 #dc3545, stop:1 #b02a37);
                color: white;
            }
            QPushButton#submitButton:hover {
                background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                            stop:0 #b02a37, stop:1 #8b2332);
            }
        )";
    } else {
        submitStyle = R"(
            QPushButton#submitButton {
                background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                            stop:0 #198754, stop:1 #146c43);
                color: white;
            }
            QPushButton#submitButton:hover {
                background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                            stop:0 #146c43, stop:1 #0d5132);
            }
        )";
    }

    ui->submitButton->setStyleSheet(ui->submitButton->styleSheet() + submitStyle);
}

bool AddTransactionDialog::validateInputs()
{
    if (ui->categoryComboBox->currentIndex() < 0) {
        QMessageBox::warning(this, "Lỗi xác thực", "Vui lòng chọn danh mục!");
        ui->categoryComboBox->setFocus();
        return false;
    }

    QString amountText = ui->amountLineEdit->text().trimmed();
    if (amountText.isEmpty()) {
        QMessageBox::warning(this, "Lỗi xác thực", "Vui lòng nhập số tiền!");
        ui->amountLineEdit->setFocus();
        return false;
    }

    bool ok;
    double amount = amountText.toDouble(&ok);
    if (!ok || amount <= 0) {
        QMessageBox::warning(this, "Lỗi xác thực", "Số tiền phải là số dương hợp lệ!");
        ui->amountLineEdit->selectAll();
        ui->amountLineEdit->setFocus();
        return false;
    }

    return true;
}

void AddTransactionDialog::onSubmitClicked()
{
    if (!validateInputs()) {
        return;
    }

    QString categoryId = ui->categoryComboBox->currentData().toString();
    double amount = ui->amountLineEdit->text().toDouble();
    QString description = ui->descriptionTextEdit->toPlainText().trimmed();
    QDateTime selectedDateTime = ui->dateTimeEdit->dateTime();

    Category* categoryPtr = App::findCategoryByIDFast(categoryId);

    if (transactionType == "expense") {
        Expense newExpense(
            App::generateNextExpenseId(),
            categoryPtr,
            selectedDateTime,
            QDateTime::currentDateTime().date(),
            amount,
            description
        );
        App::getExpenseListMutable().append(newExpense);
        App::addToRecentTransactions(newExpense.getID());
    } else {
        Income newIncome(
            App::generateNextIncomeId(),
            categoryPtr,
            selectedDateTime,
            QDateTime::currentDateTime().date(),
            amount,
            description
        );
        App::getIncomeListMutable().append(newIncome);
        App::addToRecentTransactions(newIncome.getID());
    }

    App::saveData();

    QMessageBox::information(this, "Thành công",
                             transactionType == "expense"
                             ? "Đã thêm chi tiêu thành công!"
                             : "Đã thêm thu nhập thành công!");

    accept();
}

void AddTransactionDialog::onCancelClicked()
{
    reject();
}

void AddTransactionDialog::onManageCategoryClicked()
{
    QDialog categoryDialog(this);
    categoryDialog.setWindowTitle("Quản Lý Danh Mục");
    categoryDialog.setModal(true);
    categoryDialog.resize(500, 600);
    categoryDialog.setStyleSheet(R"(
        QDialog {
            background: #f8f9fa;
        }
        QListWidget {
            border: 2px solid #ced4da;
            border-radius: 8px;
            background: white;
            padding: 5px;
            font-size: 13px;
        }
        QListWidget::item {
            padding: 10px;
            border-bottom: 1px solid #e9ecef;
        }
        QListWidget::item:selected {
            background: #e7f1ff;
            color: #0d6efd;
        }
        QPushButton {
            padding: 10px 20px;
            border: none;
            border-radius: 6px;
            font-size: 13px;
            font-weight: 600;
            min-width: 100px;
        }
        QPushButton#addButton {
            background: #198754;
            color: white;
        }
        QPushButton#addButton:hover {
            background: #146c43;
        }
        QPushButton#deleteButton {
            background: #dc3545;
            color: white;
        }
        QPushButton#deleteButton:hover {
            background: #b02a37;
        }
        QPushButton#closeButton {
            background: #6c757d;
            color: white;
        }
        QPushButton#closeButton:hover {
            background: #5a6268;
        }
    )");

    QVBoxLayout* mainLayout = new QVBoxLayout(&categoryDialog);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QLabel* titleLabel = new QLabel("Danh Sách Danh Mục", &categoryDialog);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: 700; color: #212529;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    QListWidget* categoryList = new QListWidget(&categoryDialog);
    categoryList->setSelectionMode(QAbstractItemView::SingleSelection);

    auto refreshCategoryList = [categoryList, this]() {
        categoryList->clear();
        for (const Category& category : App::getCategoryList()) {
            QString categoryType = category.getType();
            if ((transactionType == "expense" && categoryType == "Expense") ||
                (transactionType == "income" && categoryType == "Income") ||
                categoryType == "system") {
                QListWidgetItem* item = new QListWidgetItem(
                    QString("%1 - %2").arg(category.getName()).arg(category.getDescription())
                );
                item->setData(Qt::UserRole, category.getID());
                categoryList->addItem(item);
            }
        }
    };

    refreshCategoryList();
    mainLayout->addWidget(categoryList);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);

    QPushButton* addButton = new QPushButton("Thêm Mới", &categoryDialog);
    addButton->setObjectName("addButton");
    QPushButton* deleteButton = new QPushButton("Xóa", &categoryDialog);
    deleteButton->setObjectName("deleteButton");
    QPushButton* closeButton = new QPushButton("Đóng", &categoryDialog);
    closeButton->setObjectName("closeButton");

    buttonLayout->addStretch();
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(closeButton);

    mainLayout->addLayout(buttonLayout);

    connect(addButton, &QPushButton::clicked, [&categoryDialog, &refreshCategoryList, this]() {
        QDialog addDialog(&categoryDialog);
        addDialog.setWindowTitle("Thêm Danh Mục Mới");
        addDialog.setModal(true);
        addDialog.resize(400, 300);
        addDialog.setStyleSheet(categoryDialog.styleSheet());

        QVBoxLayout* layout = new QVBoxLayout(&addDialog);
        layout->setSpacing(15);
        layout->setContentsMargins(20, 20, 20, 20);

        QLabel* nameLabel = new QLabel("Tên danh mục:", &addDialog);
        nameLabel->setStyleSheet("font-size: 13px; font-weight: 500;");
        QLineEdit* nameEdit = new QLineEdit(&addDialog);
        nameEdit->setPlaceholderText("Nhập tên danh mục...");
        nameEdit->setStyleSheet("padding: 10px; border: 2px solid #ced4da; border-radius: 6px; font-size: 13px;");

        QLabel* descLabel = new QLabel("Mô tả:", &addDialog);
        descLabel->setStyleSheet("font-size: 13px; font-weight: 500;");
        QLineEdit* descEdit = new QLineEdit(&addDialog);
        descEdit->setPlaceholderText("Nhập mô tả...");
        descEdit->setStyleSheet("padding: 10px; border: 2px solid #ced4da; border-radius: 6px; font-size: 13px;");

        layout->addWidget(nameLabel);
        layout->addWidget(nameEdit);
        layout->addWidget(descLabel);
        layout->addWidget(descEdit);
        layout->addStretch();

        QHBoxLayout* btnLayout = new QHBoxLayout();
        QPushButton* saveBtn = new QPushButton("Lưu", &addDialog);
        saveBtn->setObjectName("addButton");
        QPushButton* cancelBtn = new QPushButton("Hủy", &addDialog);
        cancelBtn->setObjectName("closeButton");

        btnLayout->addStretch();
        btnLayout->addWidget(cancelBtn);
        btnLayout->addWidget(saveBtn);
        layout->addLayout(btnLayout);

        connect(saveBtn, &QPushButton::clicked, [&addDialog, nameEdit, descEdit, &refreshCategoryList, this]() {
            QString name = nameEdit->text().trimmed();
            QString desc = descEdit->text().trimmed();

            if (name.isEmpty()) {
                QMessageBox::warning(&addDialog, "Lỗi", "Vui lòng nhập tên danh mục!");
                return;
            }

            QString newId = "CAT" + QString::number(QDateTime::currentMSecsSinceEpoch());
            QString type = (transactionType == "expense") ? "Expense" : "Income";

            Category newCategory(newId, QDate::currentDate(), name, desc, type);
            App::getCategoryListMutable().append(newCategory);
            App::buildCategoryHashTable();
            App::saveData();

            refreshCategoryList();
            loadCategories();

            QMessageBox::information(&addDialog, "Thành công", "Đã thêm danh mục thành công!");
            addDialog.accept();
        });

        connect(cancelBtn, &QPushButton::clicked, &addDialog, &QDialog::reject);

        addDialog.exec();
    });

    connect(deleteButton, &QPushButton::clicked, [&categoryDialog, categoryList, &refreshCategoryList, this]() {
        QListWidgetItem* selectedItem = categoryList->currentItem();
        if (!selectedItem) {
            QMessageBox::warning(&categoryDialog, "Lỗi", "Vui lòng chọn danh mục cần xóa!");
            return;
        }

        QString categoryId = selectedItem->data(Qt::UserRole).toString();

        QMessageBox::StandardButton reply = QMessageBox::question(
            &categoryDialog, "Xác nhận",
            "Bạn có chắc muốn xóa danh mục này?",
            QMessageBox::Yes | QMessageBox::No
        );

        if (reply == QMessageBox::Yes) {
            QVector<Category>& categories = App::getCategoryListMutable();
            for (int i = 0; i < categories.size(); i++) {
                if (categories[i].getID() == categoryId) {
                    categories.removeAt(i);
                    break;
                }
            }

            App::saveData();
            refreshCategoryList();
            loadCategories();

            QMessageBox::information(&categoryDialog, "Thành công", "Đã xóa danh mục thành công!");
        }
    });

    connect(closeButton, &QPushButton::clicked, &categoryDialog, &QDialog::accept);

    categoryDialog.exec();
}
