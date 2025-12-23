#include "dashboard.h"
#include "./ui_dashboard.h"
#include "app.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QIcon>
#include <QDebug>
#include <QMessageBox>
#include <cmath>
#include <QDialog>
#include <QComboBox>
#include <QTextEdit>
#include <QTimer>
#include <QDateEdit>
#include <QCheckBox>
#include <algorithm>
#include <QListWidget>

Dashboard::Dashboard(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Dashboard)
{
    ui->setupUi(this);
    connect(ui->expenseSearchInput, &QLineEdit::textChanged, this, &Dashboard::searchExpense);
    connect(ui->incomeSearchInput, &QLineEdit::textChanged, this, &Dashboard::searchIncome);

    connect(ui->expenseSearchInput, &QLineEdit::returnPressed, this, &Dashboard::on_expenseSearchButton_clicked);
    connect(ui->incomeSearchInput, &QLineEdit::returnPressed, this, &Dashboard::on_incomeSearchButton_clicked);

    // Connect debt search
    QLineEdit* debtSearchInput = this->findChild<QLineEdit*>("debtSearchInput");
    if (debtSearchInput) {
        connect(debtSearchInput, &QLineEdit::textChanged, this, &Dashboard::searchDebt);
        connect(debtSearchInput, &QLineEdit::returnPressed, this, &Dashboard::on_debtSearchButton_clicked);
    }

    // Connect debt filter combo
    QComboBox* debtFilterCombo = this->findChild<QComboBox*>("debtFilterCombo");
    if (debtFilterCombo) {
        connect(debtFilterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Dashboard::onDebtFilterChanged);
    }

    QPushButton* expenseExpandingBtn = this->findChild<QPushButton*>("expenseExpandingButton");
    if (expenseExpandingBtn) {
        connect(expenseExpandingBtn, &QPushButton::clicked, this, &Dashboard::on_expenseExpandingButton_clicked);
    }

    QPushButton* incomeExpandingBtn = this->findChild<QPushButton*>("incomeExpandingButton");
    if (incomeExpandingBtn) {
        connect(incomeExpandingBtn, &QPushButton::clicked, this, &Dashboard::on_incomeExpandingButton_clicked);
    }

    if (!expenseExpandingBtn) {
        expenseExpandingBtn = new QPushButton("Quản lý Chi tiêu", this);
        expenseExpandingBtn->setObjectName("expenseExpandingButton");
        expenseExpandingBtn->setStyleSheet(
            "QPushButton {"
            "background-color: #dc3545;"
            "color: white;"
            "border: none;"
            "padding: 8px 16px;"
            "border-radius: 4px;"
            "font-weight: bold;"
            "}"
            "QPushButton:hover {"
            "background-color: #c82333;"
            "}"
        );

        QWidget* expenseWidget = this->findChild<QWidget*>("Expense_2");
        if (expenseWidget) {
            expenseExpandingBtn->setParent(expenseWidget);
            expenseExpandingBtn->move(20, 20);
            expenseExpandingBtn->show();
        }

        connect(expenseExpandingBtn, &QPushButton::clicked, this, &Dashboard::on_expenseExpandingButton_clicked);
    }

    if (!incomeExpandingBtn) {
        incomeExpandingBtn = new QPushButton("Quản lý Thu nhập", this);
        incomeExpandingBtn->setObjectName("incomeExpandingButton");
        incomeExpandingBtn->setStyleSheet(
            "QPushButton {"
            "background-color: #28a745;"
            "color: white;"
            "border: none;"
            "padding: 8px 16px;"
            "border-radius: 4px;"
            "font-weight: bold;"
            "}"
            "QPushButton:hover {"
            "background-color: #218838;"
            "}"
        );

        QWidget* incomeWidget = this->findChild<QWidget*>("Income_2");
        if (incomeWidget) {
            incomeExpandingBtn->setParent(incomeWidget);
            incomeExpandingBtn->move(20, 20);
            incomeExpandingBtn->show();
        }

        connect(incomeExpandingBtn, &QPushButton::clicked, this, &Dashboard::on_incomeExpandingButton_clicked);
    }

    renderCards();
    updateMonthlyExpenseTotal();
    updateMonthlyIncomeTotal();
    updateDashboardOverview();

    QTimer::singleShot(100, this, [this]() {
        setupChart();
    });

    connect(ui->savingSortCombo, &QComboBox::currentTextChanged, this, &Dashboard::onSavingSortChanged);
    connect(ui->savingSearchInput, &QLineEdit::textChanged, this, [this](const QString& text) {
        if (text.isEmpty()) {
            on_savingClearButton_clicked();
        }
    });

    // Note: updateSavingStatistics and refreshSavingGoals are now called from App::loadData()
}

Dashboard::~Dashboard()
{
    delete ui;
}

void Dashboard::on_DashBoard_btn_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Dashboard::on_expense_btn_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void Dashboard::on_income_btn_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void Dashboard::on_saving_btn_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Dashboard::on_dept_btn_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    // Data already loaded on app startup, no need to reload
}

void Dashboard::on_debt_btn_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    // Data already loaded on app startup, no need to reload
}


void Dashboard::on_addExpenseButton_clicked()
{
    showAddTransactionDialog("expense");
}

void Dashboard::on_addIncomeButton_clicked()
{
    showAddTransactionDialog("income");
}


void Dashboard::on_saving_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Dashboard::renderCards(){
    if (expenseFilterState.active || incomeFilterState.active) {
        if (expenseFilterState.active) {
            applyExpenseFiltersToMainList();
        } else {
            int index = 0;
            for (const Expense &exp : App::getExpenseList()) {
                QFrame* card = exp.createCard(QString::number(index++));

                QPushButton* editBtn = card->findChild<QPushButton*>("edit_" + exp.getID());
                QPushButton* deleteBtn = card->findChild<QPushButton*>("delete_" + exp.getID());

                if (editBtn) {
                    connect(editBtn, &QPushButton::clicked, this, [this, id = exp.getID()]() {
                        editExpenseTransaction(id);
                    });
                }
                if (deleteBtn) {
                    connect(deleteBtn, &QPushButton::clicked, this, [this, id = exp.getID()]() {
                        removeExpenseTransaction(id);
                    });
                }

                QScrollArea *scroll = ui->expenseList;
                if (scroll && scroll->widget() && scroll->widget()->layout()) {
                    qobject_cast<QVBoxLayout*>(scroll->widget()->layout())->insertWidget(0, card);
                }
            }
        }

        if (incomeFilterState.active) {
            applyIncomeFiltersToMainList();
        } else {
            int index = 0;
            for (const Income &inc : App::getIncomeList()) {
                QFrame* card = inc.createCard(QString::number(index++));

                QPushButton* editBtn = card->findChild<QPushButton*>("edit_" + inc.getID());
                QPushButton* deleteBtn = card->findChild<QPushButton*>("delete_" + inc.getID());

                if (editBtn) {
                    connect(editBtn, &QPushButton::clicked, this, [this, id = inc.getID()]() {
                        editIncomeTransaction(id);
                    });
                }
                if (deleteBtn) {
                    connect(deleteBtn, &QPushButton::clicked, this, [this, id = inc.getID()]() {
                        removeIncomeTransaction(id);
                    });
                }

                QScrollArea *scroll = ui->incomeList;
                if (scroll && scroll->widget() && scroll->widget()->layout()) {
                    qobject_cast<QVBoxLayout*>(scroll->widget()->layout())->insertWidget(0, card);
                }
            }
        }
        return;
    }

    int incomeIndex = 0;
    for (const Income &inc : App::getIncomeList())
    {
        QFrame* card = inc.createCard(QString::number(incomeIndex++));

        QPushButton* editBtn = card->findChild<QPushButton*>("edit_" + inc.getID());
        QPushButton* deleteBtn = card->findChild<QPushButton*>("delete_" + inc.getID());

        if (editBtn) {
            connect(editBtn, &QPushButton::clicked, this, [this, id = inc.getID()]() {
                editIncomeTransaction(id);
            });
        }
        if (deleteBtn) {
            connect(deleteBtn, &QPushButton::clicked, this, [this, id = inc.getID()]() {
                removeIncomeTransaction(id);
            });
        }

        QScrollArea *scroll = ui->incomeList;
        if (scroll && scroll->widget() && scroll->widget()->layout()) {
            qobject_cast<QVBoxLayout*>(scroll->widget()->layout())->insertWidget(0, card);
        }
    }

    int expenseIndex = 0;
    for (const Expense &exp : App::getExpenseList())
    {
        QFrame* card = exp.createCard(QString::number(expenseIndex++));

        QPushButton* editBtn = card->findChild<QPushButton*>("edit_" + exp.getID());
        QPushButton* deleteBtn = card->findChild<QPushButton*>("delete_" + exp.getID());

        if (editBtn) {
            connect(editBtn, &QPushButton::clicked, this, [this, id = exp.getID()]() {
                editExpenseTransaction(id);
            });
        }
        if (deleteBtn) {
            connect(deleteBtn, &QPushButton::clicked, this, [this, id = exp.getID()]() {
                removeExpenseTransaction(id);
            });
        }

        QScrollArea *scroll = ui->expenseList;
        if (scroll && scroll->widget() && scroll->widget()->layout()) {
            qobject_cast<QVBoxLayout*>(scroll->widget()->layout())->insertWidget(0, card);
        }
    }

}

void Dashboard::updateMonthlyExpenseTotal()
{
    double totalExpense = App::getMonthExpenseTotal();
    QString formattedAmount = App::formatMoney(totalExpense);

    QLabel *expenseTotalLabel = this->findChild<QLabel*>("totalExpenseLabel");
    if (expenseTotalLabel) {
        expenseTotalLabel->setText(formattedAmount);
    }
}

void Dashboard::updateMonthlyIncomeTotal()
{
    double totalIncome = App::getMonthIncomeTotal();
    QString formattedAmount = App::formatMoney(totalIncome);

    QLabel *incomeTotalLabel = this->findChild<QLabel*>("totalIncomeLabel");
    if (incomeTotalLabel) {
        incomeTotalLabel->setText(formattedAmount);
    }
}

void Dashboard::refreshAllDataViews()
{
    if (searchModeActive) {
        originalExpenseCards.clear();
        searchModeActive = false;
        if (ui->expenseSearchInput) {
            ui->expenseSearchInput->clear();
        }
    }

    if (incomeSearchModeActive) {
        originalIncomeCards.clear();
        incomeSearchModeActive = false;
        if (ui->incomeSearchInput) {
            ui->incomeSearchInput->clear();
        }
    }

    clearScrollAreaSafely(ui->incomeList);
    clearScrollAreaSafely(ui->expenseList);

    QTimer::singleShot(50, this, [this]() {
        renderCards();

        ensureCardsAtTop(ui->expenseList);
        ensureCardsAtTop(ui->incomeList);

        updateMonthlyExpenseTotal();
        updateMonthlyIncomeTotal();
        updateDashboardOverview();

        updateChart();

        updateSavingStatistics();
        refreshSavingGoals();
    });
}

void Dashboard::clearScrollAreaSafely(QScrollArea* scrollArea) {
    if (!scrollArea || !scrollArea->widget() || !scrollArea->widget()->layout()) {
        return;
    }

    QLayout* layout = scrollArea->widget()->layout();

    QVector<QWidget*> widgetsToDelete;
    for (int i = 0; i < layout->count(); i++) {
        QLayoutItem* item = layout->itemAt(i);
        if (item && item->widget()) {
            widgetsToDelete.append(item->widget());
        }
    }

    while (layout->count() > 0) {
        QLayoutItem* item = layout->takeAt(0);
        if (item) {
            delete item;
        }
    }

    for (QWidget* widget : widgetsToDelete) {
        widget->deleteLater();
    }
}

QVector<int> Dashboard::computeLPS(const QString& pattern) {
    int m = pattern.length();
    QVector<int> lps(m, 0);
    int len = 0;
    int i = 1;

    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

bool Dashboard::KMPSearch(const QString& text, const QString& pattern) {
    if (pattern.isEmpty()) return true;
    if (text.isEmpty()) return false;

    QString lowerText = text.toLower();
    QString lowerPattern = pattern.toLower();

    int n = lowerText.length();
    int m = lowerPattern.length();

    QVector<int> lps = computeLPS(lowerPattern);

    int i = 0;
    int j = 0;

    while (i < n) {
        if (lowerPattern[j] == lowerText[i]) {
            j++;
            i++;
        }

        if (j == m) {
            return true;
        } else if (i < n && lowerPattern[j] != lowerText[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
    return false;
}

void Dashboard::on_expenseSearchButton_clicked() {
    QString keyword = ui->expenseSearchInput->text().trimmed();
    if (keyword.isEmpty()) {
        clearExpenseSearch();
    } else {
        searchExpense(keyword);
    }
}

void Dashboard::on_expenseClearButton_clicked() {
    clearExpenseSearch();
}

void Dashboard::on_incomeSearchButton_clicked() {
    QString keyword = ui->incomeSearchInput->text().trimmed();
    if (keyword.isEmpty()) {
        clearIncomeSearch();
    } else {
        searchIncome(keyword);
    }
}

void Dashboard::on_incomeClearButton_clicked() {
    clearIncomeSearch();
}

void Dashboard::searchExpense(const QString& keyword) {
    if (keyword.isEmpty()) {
        clearExpenseSearch();
        return;
    }

    QScrollArea* expenseScroll = ui->expenseList;
    if (!expenseScroll || !expenseScroll->widget()) return;

    QLayout* expenseLayout = expenseScroll->widget()->layout();
    if (!expenseLayout) return;

    if (!searchModeActive) {
        originalExpenseCards.clear();
        for (int i = 0; i < expenseLayout->count(); i++) {
            QLayoutItem* item = expenseLayout->itemAt(i);
            if (item && item->widget()) {
                QFrame* card = qobject_cast<QFrame*>(item->widget());
                if (card) {
                    QString cardText = extractCardText(card);
                    originalExpenseCards.append({cardText, card});
                }
            }
        }
        searchModeActive = true;
    }

    for (int i = 0; i < expenseLayout->count(); i++) {
        QLayoutItem* item = expenseLayout->itemAt(i);
        if (item && item->widget()) {
            item->widget()->hide();
        }
    }

    int visibleCount = 0;
    for (const auto& cardPair : originalExpenseCards) {
        const QString& cardText = cardPair.first;
        QFrame* card = cardPair.second;

        if (KMPSearch(cardText, keyword)) {
            card->show();
            visibleCount++;
        }
    }

    ensureCardsAtTop(expenseScroll);

    if (visibleCount == 0) {
        QMessageBox::information(this, "Kết quả tìm kiếm",
                                 QString("Không tìm thấy chi tiêu nào chứa từ khóa: %1").arg(keyword));
    }
}

void Dashboard::clearExpenseSearch() {
    if (!searchModeActive) return;

    QScrollArea* expenseScroll = ui->expenseList;
    if (!expenseScroll || !expenseScroll->widget()) return;

    QLayout* expenseLayout = expenseScroll->widget()->layout();
    if (!expenseLayout) return;

    for (int i = 0; i < expenseLayout->count(); i++) {
        QLayoutItem* item = expenseLayout->itemAt(i);
        if (item && item->widget()) {
            item->widget()->show();
        }
    }

    ensureCardsAtTop(expenseScroll);

    originalExpenseCards.clear();
    searchModeActive = false;
    ui->expenseSearchInput->clear();
}

QString Dashboard::extractCardText(QFrame* card) {
    if (!card) return "";

    QString text;
    QList<QLabel*> labels = card->findChildren<QLabel*>();
    for (QLabel* label : labels) {
        text += label->text() + " ";
    }
    return text;
}

void Dashboard::ensureCardsAtTop(QScrollArea* scrollArea) {
    if (!scrollArea || !scrollArea->widget()) return;

    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(scrollArea->widget()->layout());
    if (!layout) return;

    if (layout->count() > 0) {
        QLayoutItem* lastItem = layout->itemAt(layout->count() - 1);
        if (lastItem && lastItem->spacerItem()) {
            return;
        }
    }

    layout->addStretch();
}

void Dashboard::searchIncome(const QString& keyword) {
    if (keyword.isEmpty()) {
        clearIncomeSearch();
        return;
    }

    QScrollArea* incomeScroll = ui->incomeList;
    if (!incomeScroll || !incomeScroll->widget()) return;

    QLayout* incomeLayout = incomeScroll->widget()->layout();
    if (!incomeLayout) return;

    if (!incomeSearchModeActive) {
        originalIncomeCards.clear();
        for (int i = 0; i < incomeLayout->count(); i++) {
            QLayoutItem* item = incomeLayout->itemAt(i);
            if (item && item->widget()) {
                QFrame* card = qobject_cast<QFrame*>(item->widget());
                if (card) {
                    QString cardText = extractCardText(card);
                    originalIncomeCards.append({cardText, card});
                }
            }
        }
        incomeSearchModeActive = true;
    }

    for (int i = 0; i < incomeLayout->count(); i++) {
        QLayoutItem* item = incomeLayout->itemAt(i);
        if (item && item->widget()) {
            item->widget()->hide();
        }
    }

    int visibleCount = 0;
    for (const auto& cardPair : originalIncomeCards) {
        const QString& cardText = cardPair.first;
        QFrame* card = cardPair.second;

        if (KMPSearch(cardText, keyword)) {
            card->show();
            visibleCount++;
        }
    }

    ensureCardsAtTop(incomeScroll);

    if (visibleCount == 0) {
        QMessageBox::information(this, "Kết quả tìm kiếm",
                                 QString("Không tìm thấy thu nhập nào chứa từ khóa: %1").arg(keyword));
    }
}

void Dashboard::clearIncomeSearch() {
    if (!incomeSearchModeActive) return;


    QScrollArea* incomeScroll = ui->incomeList;
    if (!incomeScroll || !incomeScroll->widget()) return;

    QLayout* incomeLayout = incomeScroll->widget()->layout();
    if (!incomeLayout) return;


    for (int i = 0; i < incomeLayout->count(); i++) {
        QLayoutItem* item = incomeLayout->itemAt(i);
        if (item && item->widget()) {
            item->widget()->show();
        }
    }


    ensureCardsAtTop(incomeScroll);

    originalIncomeCards.clear();
    incomeSearchModeActive = false;
    ui->incomeSearchInput->clear();
}

void Dashboard::showAddTransactionDialog(const QString& type) {
    QDialog dialog(this);
    dialog.setWindowTitle(type == "expense" ? "Thêm Chi Tiêu" : "Thêm Thu Nhập");
    dialog.setModal(true);
    dialog.resize(400, 300);


    QVBoxLayout* layout = new QVBoxLayout(&dialog);


    QLabel* categoryLabel = new QLabel("Danh mục:", &dialog);
    QComboBox* categoryCombo = new QComboBox(&dialog);

    for (const Category& category : App::getCategoryList()) {
        QString categoryType = category.getType();
        if ((type == "expense" && categoryType == "Expense") ||
            (type == "income" && categoryType == "Income") ||
            categoryType == "system") {
            categoryCombo->addItem(category.getName(), category.getID());
        }
    }


    QLabel* amountLabel = new QLabel("Số tiền:", &dialog);
    QLineEdit* amountEdit = new QLineEdit(&dialog);
    amountEdit->setPlaceholderText("Nhập số tiền...");

    QLabel* descLabel = new QLabel("Mô tả:", &dialog);
    QTextEdit* descEdit = new QTextEdit(&dialog);
    descEdit->setPlaceholderText("Nhập mô tả (tùy chọn)...");
    descEdit->setMaximumHeight(80);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* okButton = new QPushButton("Thêm", &dialog);
    QPushButton* cancelButton = new QPushButton("Hủy", &dialog);

    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    layout->addWidget(categoryLabel);
    layout->addWidget(categoryCombo);
    layout->addWidget(amountLabel);
    layout->addWidget(amountEdit);
    layout->addWidget(descLabel);
    layout->addWidget(descEdit);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, [&dialog, categoryCombo, amountEdit, descEdit, type, this]() {
        QString categoryId = categoryCombo->currentData().toString();
        QString amountText = amountEdit->text().trimmed();
        QString description = descEdit->toPlainText().trimmed();

        if (categoryId.isEmpty()) {
            QMessageBox::warning(&dialog, "Lỗi", "Vui lòng chọn danh mục!");
            return;
        }

        if (amountText.isEmpty()) {
            QMessageBox::warning(&dialog, "Lỗi", "Vui lòng nhập số tiền!");
            return;
        }

        bool ok;
        double amount = amountText.toDouble(&ok);
        if (!ok || amount <= 0) {
            QMessageBox::warning(&dialog, "Lỗi", "Số tiền không hợp lệ!");
            return;
        }

        if (type == "expense") {
            App::addExpense(categoryId, amount, description);
        } else {
            App::addIncome(categoryId, amount, description);
        }

        refreshAllDataViews();

        updateMonthlyExpenseTotal();
        updateMonthlyIncomeTotal();

        QMessageBox::information(&dialog, "Thành công",
                                 type == "expense" ? "Đã thêm chi tiêu thành công!" : "Đã thêm thu nhập thành công!");
        dialog.accept();
    });

    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    dialog.exec();
}

void Dashboard::updateDashboardOverview() {

    double totalIncome = 0;
    double totalExpense = 0;
    double monthIncome = 0;
    double monthExpense = 0;

    QDateTime now = QDateTime::currentDateTime();
    int currentMonth = now.date().month();
    int currentYear = now.date().year();


    const QVector<Income>& incomes = App::getIncomeList();
    for (const Income& inc : incomes) {
        totalIncome += inc.getAmount();
        if (inc.getCreatedAt().date().month() == currentMonth &&
            inc.getCreatedAt().date().year() == currentYear) {
            monthIncome += inc.getAmount();
        }
    }


    const QVector<Expense>& expenses = App::getExpenseList();
    for (const Expense& exp : expenses) {
        totalExpense += exp.getAmount();
        if (exp.getCreatedAt().date().month() == currentMonth &&
            exp.getCreatedAt().date().year() == currentYear) {
            monthExpense += exp.getAmount();
        }
    }

    double balance = totalIncome - totalExpense;

    qDebug() << "Balance:" << balance << "Month Expense:" << monthExpense << "Month Income:" << monthIncome;
    qDebug() << "Total Income:" << totalIncome << "Total Expense:" << totalExpense;


    QLabel* balanceLabel = this->findChild<QLabel*>("label_135");
    if (balanceLabel) {
        QString color = balance >= 0 ? "#26af08" : "#af1d00";
        QString formattedBalance = App::formatMoney(balance) + " VNĐ";
        QString htmlText = QString("<html><head/><body><p><span style=\" font-size:18pt; font-weight:700; color:%1;\">%2</span></p></body></html>")
                          .arg(color).arg(formattedBalance);
        balanceLabel->setText(htmlText);
        qDebug() << "Updated label_135 with:" << htmlText;
    } else {
        qDebug() << "Label_135 not found!";
    }


    QLabel* expenseLabel = this->findChild<QLabel*>("label_137");
    if (expenseLabel) {
        QString formattedExpense = App::formatMoney(monthExpense) + " VNĐ";
        QString htmlText = QString("<html><head/><body><p><span style=\" font-size:18pt; font-weight:700; color:#af1d00;\">%1</span></p></body></html>")
                          .arg(formattedExpense);
        expenseLabel->setText(htmlText);
        qDebug() << "Updated label_137 with:" << formattedExpense;
    } else {
        qDebug() << "Label_137 not found!";
    }


    QLabel* incomeLabel = this->findChild<QLabel*>("label_139");
    if (incomeLabel) {
        QString formattedIncome = App::formatMoney(monthIncome) + " VNĐ";
        QString htmlText = QString("<html><head/><body><p><span style=\" font-size:18pt; font-weight:700; color:#26af08;\">%1</span></p></body></html>")
                          .arg(formattedIncome);
        incomeLabel->setText(htmlText);
        qDebug() << "Updated label_139 with:" << formattedIncome;
    } else {
        qDebug() << "Label_139 not found!";
    }

}


void Dashboard::on_expenseExpandingButton_clicked() {
    QDialog dialog(this);
    dialog.setWindowTitle("Quản lý Chi tiêu nâng cao");
    dialog.setModal(true);
    dialog.resize(800, 600);

    QVBoxLayout* mainLayout = new QVBoxLayout(&dialog);

    QGroupBox* filterGroup = new QGroupBox("Bộ lọc và Sắp xếp", &dialog);
    QGridLayout* filterLayout = new QGridLayout(filterGroup);

    QLabel* sortLabel = new QLabel("Sắp xếp theo:", &dialog);
    QComboBox* sortCombo = new QComboBox(&dialog);
    sortCombo->addItem("Số tiền", "amount");
    sortCombo->addItem("Thời gian", "date");
    sortCombo->addItem("Danh mục", "category");

    QComboBox* orderCombo = new QComboBox(&dialog);
    orderCombo->addItem("Tăng dần", true);
    orderCombo->addItem("Giảm dần", false);

    QLabel* dateLabel = new QLabel("Khoảng thời gian:", &dialog);
    QDateEdit* fromDate = new QDateEdit(&dialog);
    fromDate->setCalendarPopup(true);
    fromDate->setDate(QDate::currentDate().addMonths(-1));

    QLabel* toLabel = new QLabel("đến", &dialog);
    QDateEdit* toDate = new QDateEdit(&dialog);
    toDate->setCalendarPopup(true);
    toDate->setDate(QDate::currentDate());

    QLabel* amountLabel = new QLabel("Khoảng số tiền:", &dialog);
    QLineEdit* minAmount = new QLineEdit(&dialog);
    minAmount->setPlaceholderText("Từ (VNĐ)");

    QLabel* toAmountLabel = new QLabel("đến", &dialog);
    QLineEdit* maxAmount = new QLineEdit(&dialog);
    maxAmount->setPlaceholderText("Đến (VNĐ)");

    filterLayout->addWidget(sortLabel, 0, 0);
    filterLayout->addWidget(sortCombo, 0, 1);
    filterLayout->addWidget(orderCombo, 0, 2);

    filterLayout->addWidget(dateLabel, 1, 0);
    filterLayout->addWidget(fromDate, 1, 1);
    filterLayout->addWidget(toLabel, 1, 2);
    filterLayout->addWidget(toDate, 1, 3);

    filterLayout->addWidget(amountLabel, 2, 0);
    filterLayout->addWidget(minAmount, 2, 1);
    filterLayout->addWidget(toAmountLabel, 2, 2);
    filterLayout->addWidget(maxAmount, 2, 3);

    QPushButton* applyFilterBtn = new QPushButton("Áp dụng bộ lọc", &dialog);
    QPushButton* clearFilterBtn = new QPushButton("Xóa bộ lọc", &dialog);

    QHBoxLayout* filterBtnLayout = new QHBoxLayout();
    filterBtnLayout->addWidget(applyFilterBtn);
    filterBtnLayout->addWidget(clearFilterBtn);
    filterBtnLayout->addStretch();

    QListWidget* expenseList = new QListWidget(&dialog);


    auto populateList = [expenseList, sortCombo, orderCombo, fromDate, toDate, minAmount, maxAmount](bool applyFilters = false) {
        expenseList->clear();

        QVector<Expense> expenses = App::getExpenseList();

        if (applyFilters) {
            QVector<Expense> filteredExpenses;
            QDate startDate = fromDate->date();
            QDate endDate = toDate->date();
            double minAmt = minAmount->text().isEmpty() ? 0 : minAmount->text().toDouble();
            double maxAmt = maxAmount->text().isEmpty() ? -1 : maxAmount->text().toDouble();

            for (const Expense& expense : expenses) {
                QDate expenseDate = expense.getCreatedAt().date();
                double amount = expense.getAmount();

                if (expenseDate < startDate || expenseDate > endDate) continue;

                if (amount < minAmt) continue;
                if (maxAmt > 0 && amount > maxAmt) continue;

                filteredExpenses.append(expense);
            }
            expenses = filteredExpenses;
        }

        QString sortType = sortCombo->currentData().toString();
        bool ascending = orderCombo->currentData().toBool();

        if (sortType == "amount") {
            std::sort(expenses.begin(), expenses.end(), [ascending](const Expense& a, const Expense& b) {
                return ascending ? a.getAmount() < b.getAmount() : a.getAmount() > b.getAmount();
            });
        } else if (sortType == "date") {
            std::sort(expenses.begin(), expenses.end(), [ascending](const Expense& a, const Expense& b) {
                return ascending ? a.getCreatedAt() < b.getCreatedAt() : a.getCreatedAt() > b.getCreatedAt();
            });
        } else if (sortType == "category") {
            std::sort(expenses.begin(), expenses.end(), [ascending](const Expense& a, const Expense& b) {
                QString catA = a.getCategory() ? a.getCategory()->getName() : "";
                QString catB = b.getCategory() ? b.getCategory()->getName() : "";
                return ascending ? catA < catB : catA > catB;
            });
        }

        for (const Expense& expense : expenses) {
            QString categoryName = expense.getCategory() ? expense.getCategory()->getName() : "Không xác định";
            QString itemText = QString("%1 - %2 VNĐ - %3 - %4")
                              .arg(categoryName)
                              .arg(App::formatMoney(expense.getAmount()))
                              .arg(expense.getCreatedAt().toString("dd/MM/yyyy"))
                              .arg(expense.getDescription());

            QListWidgetItem* item = new QListWidgetItem(itemText);
            item->setData(Qt::UserRole, expense.getID());
            expenseList->addItem(item);
        }
    };

    populateList(false);

    connect(applyFilterBtn, &QPushButton::clicked, [populateList, fromDate, toDate]() {
        if (fromDate->date() > toDate->date()) {
            QMessageBox::warning(nullptr, "Lỗi", "Ngày bắt đầu không thể sau ngày kết thúc!");
            return;
        }
        populateList(true);
    });

    connect(clearFilterBtn, &QPushButton::clicked, [populateList]() {
        populateList(false);
    });

    connect(sortCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [populateList]() {
        populateList(false);
    });

    connect(orderCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [populateList]() {
        populateList(false);
    });

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* editButton = new QPushButton("Sửa", &dialog);
    QPushButton* removeButton = new QPushButton("Xóa", &dialog);
    QPushButton* closeButton = new QPushButton("Đóng", &dialog);

    editButton->setEnabled(false);
    removeButton->setEnabled(false);

    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(removeButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);


    mainLayout->addWidget(filterGroup);
    mainLayout->addLayout(filterBtnLayout);
    mainLayout->addWidget(new QLabel("Danh sách chi tiêu:"));
    mainLayout->addWidget(expenseList);
    mainLayout->addLayout(buttonLayout);


    connect(expenseList, &QListWidget::itemSelectionChanged, [editButton, removeButton, expenseList]() {
        bool hasSelection = !expenseList->selectedItems().isEmpty();
        editButton->setEnabled(hasSelection);
        removeButton->setEnabled(hasSelection);
    });


    connect(editButton, &QPushButton::clicked, [this, expenseList, &dialog]() {
        auto selectedItems = expenseList->selectedItems();
        if (!selectedItems.isEmpty()) {
            QString expenseId = selectedItems.first()->data(Qt::UserRole).toString();
            editExpenseTransaction(expenseId);
            dialog.accept();
        }
    });


    connect(removeButton, &QPushButton::clicked, [this, expenseList, &dialog]() {
        auto selectedItems = expenseList->selectedItems();
        if (!selectedItems.isEmpty()) {
            QString expenseId = selectedItems.first()->data(Qt::UserRole).toString();

            QMessageBox::StandardButton reply = QMessageBox::question(&dialog, "Xác nhận xóa",
                "Bạn có chắc chắn muốn xóa chi tiêu này?",
                QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes) {
                removeExpenseTransaction(expenseId);
                dialog.accept();
            }
        }
    });

    connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.exec();
}

void Dashboard::on_incomeExpandingButton_clicked() {
    QDialog dialog(this);
    dialog.setWindowTitle("Quản lý Thu nhập nâng cao");
    dialog.setModal(true);
    dialog.resize(800, 600);

    QVBoxLayout* mainLayout = new QVBoxLayout(&dialog);


    QGroupBox* filterGroup = new QGroupBox("Bộ lọc và Sắp xếp", &dialog);
    QGridLayout* filterLayout = new QGridLayout(filterGroup);


    QLabel* sortLabel = new QLabel("Sắp xếp theo:", &dialog);
    QComboBox* sortCombo = new QComboBox(&dialog);
    sortCombo->addItem("Số tiền", "amount");
    sortCombo->addItem("Thời gian", "date");
    sortCombo->addItem("Danh mục", "category");

    QComboBox* orderCombo = new QComboBox(&dialog);
    orderCombo->addItem("Tăng dần", true);
    orderCombo->addItem("Giảm dần", false);


    QLabel* dateLabel = new QLabel("Khoảng thời gian:", &dialog);
    QDateEdit* fromDate = new QDateEdit(&dialog);
    fromDate->setCalendarPopup(true);
    fromDate->setDate(QDate::currentDate().addMonths(-1));

    QLabel* toLabel = new QLabel("đến", &dialog);
    QDateEdit* toDate = new QDateEdit(&dialog);
    toDate->setCalendarPopup(true);
    toDate->setDate(QDate::currentDate());


    QLabel* amountLabel = new QLabel("Khoảng số tiền:", &dialog);
    QLineEdit* minAmount = new QLineEdit(&dialog);
    minAmount->setPlaceholderText("Từ (VNĐ)");

    QLabel* toAmountLabel = new QLabel("đến", &dialog);
    QLineEdit* maxAmount = new QLineEdit(&dialog);
    maxAmount->setPlaceholderText("Đến (VNĐ)");


    filterLayout->addWidget(sortLabel, 0, 0);
    filterLayout->addWidget(sortCombo, 0, 1);
    filterLayout->addWidget(orderCombo, 0, 2);

    filterLayout->addWidget(dateLabel, 1, 0);
    filterLayout->addWidget(fromDate, 1, 1);
    filterLayout->addWidget(toLabel, 1, 2);
    filterLayout->addWidget(toDate, 1, 3);

    filterLayout->addWidget(amountLabel, 2, 0);
    filterLayout->addWidget(minAmount, 2, 1);
    filterLayout->addWidget(toAmountLabel, 2, 2);
    filterLayout->addWidget(maxAmount, 2, 3);

    QPushButton* applyFilterBtn = new QPushButton("Áp dụng bộ lọc", &dialog);
    QPushButton* clearFilterBtn = new QPushButton("Xóa bộ lọc", &dialog);

    QHBoxLayout* filterBtnLayout = new QHBoxLayout();
    filterBtnLayout->addWidget(applyFilterBtn);
    filterBtnLayout->addWidget(clearFilterBtn);
    filterBtnLayout->addStretch();


    QListWidget* incomeList = new QListWidget(&dialog);


    auto populateList = [incomeList, sortCombo, orderCombo, fromDate, toDate, minAmount, maxAmount](bool applyFilters = false) {
        incomeList->clear();

        QVector<Income> incomes = App::getIncomeList();


        if (applyFilters) {
            QVector<Income> filteredIncomes;
            QDate startDate = fromDate->date();
            QDate endDate = toDate->date();
            double minAmt = minAmount->text().isEmpty() ? 0 : minAmount->text().toDouble();
            double maxAmt = maxAmount->text().isEmpty() ? -1 : maxAmount->text().toDouble();

            for (const Income& income : incomes) {
                QDate incomeDate = income.getCreatedAt().date();
                double amount = income.getAmount();


                if (incomeDate < startDate || incomeDate > endDate) continue;


                if (amount < minAmt) continue;
                if (maxAmt > 0 && amount > maxAmt) continue;

                filteredIncomes.append(income);
            }
            incomes = filteredIncomes;
        }


        QString sortType = sortCombo->currentData().toString();
        bool ascending = orderCombo->currentData().toBool();

        if (sortType == "amount") {
            std::sort(incomes.begin(), incomes.end(), [ascending](const Income& a, const Income& b) {
                return ascending ? a.getAmount() < b.getAmount() : a.getAmount() > b.getAmount();
            });
        } else if (sortType == "date") {
            std::sort(incomes.begin(), incomes.end(), [ascending](const Income& a, const Income& b) {
                return ascending ? a.getCreatedAt() < b.getCreatedAt() : a.getCreatedAt() > b.getCreatedAt();
            });
        } else if (sortType == "category") {
            std::sort(incomes.begin(), incomes.end(), [ascending](const Income& a, const Income& b) {
                QString catA = a.getCategory() ? a.getCategory()->getName() : "";
                QString catB = b.getCategory() ? b.getCategory()->getName() : "";
                return ascending ? catA < catB : catA > catB;
            });
        }


        for (const Income& income : incomes) {
            QString categoryName = income.getCategory() ? income.getCategory()->getName() : "Không xác định";
            QString itemText = QString("%1 - %2 VNĐ - %3 - %4")
                              .arg(categoryName)
                              .arg(App::formatMoney(income.getAmount()))
                              .arg(income.getCreatedAt().toString("dd/MM/yyyy"))
                              .arg(income.getDescription());

            QListWidgetItem* item = new QListWidgetItem(itemText);
            item->setData(Qt::UserRole, income.getID());
            incomeList->addItem(item);
        }
    };


    populateList(false);


    connect(applyFilterBtn, &QPushButton::clicked, [populateList, fromDate, toDate]() {
        if (fromDate->date() > toDate->date()) {
            QMessageBox::warning(nullptr, "Lỗi", "Ngày bắt đầu không thể sau ngày kết thúc!");
            return;
        }
        populateList(true);
    });

    connect(clearFilterBtn, &QPushButton::clicked, [populateList]() {
        populateList(false);
    });


    connect(sortCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [populateList]() {
        populateList(false);
    });

    connect(orderCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [populateList]() {
        populateList(false);
    });


    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* editButton = new QPushButton("Sửa", &dialog);
    QPushButton* removeButton = new QPushButton("Xóa", &dialog);
    QPushButton* closeButton = new QPushButton("Đóng", &dialog);

    editButton->setEnabled(false);
    removeButton->setEnabled(false);

    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(removeButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);


    mainLayout->addWidget(filterGroup);
    mainLayout->addLayout(filterBtnLayout);
    mainLayout->addWidget(new QLabel("Danh sách thu nhập:"));
    mainLayout->addWidget(incomeList);
    mainLayout->addLayout(buttonLayout);


    connect(incomeList, &QListWidget::itemSelectionChanged, [editButton, removeButton, incomeList]() {
        bool hasSelection = !incomeList->selectedItems().isEmpty();
        editButton->setEnabled(hasSelection);
        removeButton->setEnabled(hasSelection);
    });


    connect(editButton, &QPushButton::clicked, [this, incomeList, &dialog]() {
        auto selectedItems = incomeList->selectedItems();
        if (!selectedItems.isEmpty()) {
            QString incomeId = selectedItems.first()->data(Qt::UserRole).toString();
            editIncomeTransaction(incomeId);
            dialog.accept();
        }
    });


    connect(removeButton, &QPushButton::clicked, [this, incomeList, &dialog]() {
        auto selectedItems = incomeList->selectedItems();
        if (!selectedItems.isEmpty()) {
            QString incomeId = selectedItems.first()->data(Qt::UserRole).toString();

            QMessageBox::StandardButton reply = QMessageBox::question(&dialog, "Xác nhận xóa",
                "Bạn có chắc chắn muốn xóa thu nhập này?",
                QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes) {
                removeIncomeTransaction(incomeId);
                dialog.accept();
            }
        }
    });

    connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.exec();
}

void Dashboard::handleExpenseCardAction() {

    on_expenseExpandingButton_clicked();
}

void Dashboard::handleIncomeCardAction() {

    on_incomeExpandingButton_clicked();
}

void Dashboard::editIncomeTransaction(const QString& incomeId) {

    Income* incomeToEdit = nullptr;
    auto& incomeList = const_cast<QVector<Income>&>(App::getIncomeList());
    for (Income& income : incomeList) {
        if (income.getID() == incomeId) {
            incomeToEdit = &income;
            break;
        }
    }

    if (!incomeToEdit) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy thu nhập!");
        return;
    }


    QDialog dialog(this);
    dialog.setWindowTitle("Sửa Thu Nhập");
    dialog.setModal(true);
    dialog.resize(400, 300);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);


    QLabel* categoryLabel = new QLabel("Danh mục:", &dialog);
    QComboBox* categoryCombo = new QComboBox(&dialog);

    for (const Category& category : App::getCategoryList()) {
        QString categoryType = category.getType();
        if (categoryType == "Income" || categoryType == "system") {
            categoryCombo->addItem(category.getName(), category.getID());
            if (incomeToEdit->getCategory() && category.getID() == incomeToEdit->getCategory()->getID()) {
                categoryCombo->setCurrentIndex(categoryCombo->count() - 1);
            }
        }
    }


    QLabel* amountLabel = new QLabel("Số tiền:", &dialog);
    QLineEdit* amountEdit = new QLineEdit(&dialog);
    amountEdit->setText(QString::number(incomeToEdit->getAmount()));


    QLabel* descLabel = new QLabel("Mô tả:", &dialog);
    QTextEdit* descEdit = new QTextEdit(&dialog);
    descEdit->setPlainText(incomeToEdit->getDescription());
    descEdit->setMaximumHeight(80);


    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* saveButton = new QPushButton("Lưu", &dialog);
    QPushButton* cancelButton = new QPushButton("Hủy", &dialog);

    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);

    layout->addWidget(categoryLabel);
    layout->addWidget(categoryCombo);
    layout->addWidget(amountLabel);
    layout->addWidget(amountEdit);
    layout->addWidget(descLabel);
    layout->addWidget(descEdit);
    layout->addLayout(buttonLayout);

    connect(saveButton, &QPushButton::clicked, [&dialog, categoryCombo, amountEdit, descEdit, incomeToEdit, this]() {
        QString categoryId = categoryCombo->currentData().toString();
        QString amountText = amountEdit->text().trimmed();
        QString description = descEdit->toPlainText().trimmed();

        if (categoryId.isEmpty() || amountText.isEmpty()) {
            QMessageBox::warning(&dialog, "Lỗi", "Vui lòng điền đầy đủ thông tin!");
            return;
        }

        bool ok;
        double amount = amountText.toDouble(&ok);
        if (!ok || amount <= 0) {
            QMessageBox::warning(&dialog, "Lỗi", "Số tiền không hợp lệ!");
            return;
        }


        incomeToEdit->setAmount(amount);
        incomeToEdit->setDescription(description);
        Category* newCategory = App::findCategoryByID(categoryId);
        if (newCategory) {
            incomeToEdit->setCategory(newCategory);
        }


        App::writeData();


        refreshAllDataViews();
        updateDashboardOverview();

        QMessageBox::information(&dialog, "Thành công", "Đã cập nhật thu nhập thành công!");
        dialog.accept();
    });

    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    dialog.exec();
}

void Dashboard::editExpenseTransaction(const QString& expenseId) {

    Expense* expenseToEdit = nullptr;
    auto& expenseList = const_cast<QVector<Expense>&>(App::getExpenseList());
    for (Expense& expense : expenseList) {
        if (expense.getID() == expenseId) {
            expenseToEdit = &expense;
            break;
        }
    }

    if (!expenseToEdit) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy chi tiêu!");
        return;
    }


    QDialog dialog(this);
    dialog.setWindowTitle("Sửa Chi Tiêu");
    dialog.setModal(true);
    dialog.resize(400, 300);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);


    QLabel* categoryLabel = new QLabel("Danh mục:", &dialog);
    QComboBox* categoryCombo = new QComboBox(&dialog);

    for (const Category& category : App::getCategoryList()) {
        QString categoryType = category.getType();
        if (categoryType == "Expense" || categoryType == "system") {
            categoryCombo->addItem(category.getName(), category.getID());
            if (expenseToEdit->getCategory() && category.getID() == expenseToEdit->getCategory()->getID()) {
                categoryCombo->setCurrentIndex(categoryCombo->count() - 1);
            }
        }
    }


    QLabel* amountLabel = new QLabel("Số tiền:", &dialog);
    QLineEdit* amountEdit = new QLineEdit(&dialog);
    amountEdit->setText(QString::number(expenseToEdit->getAmount()));


    QLabel* descLabel = new QLabel("Mô tả:", &dialog);
    QTextEdit* descEdit = new QTextEdit(&dialog);
    descEdit->setPlainText(expenseToEdit->getDescription());
    descEdit->setMaximumHeight(80);


    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* saveButton = new QPushButton("Lưu", &dialog);
    QPushButton* cancelButton = new QPushButton("Hủy", &dialog);

    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);

    layout->addWidget(categoryLabel);
    layout->addWidget(categoryCombo);
    layout->addWidget(amountLabel);
    layout->addWidget(amountEdit);
    layout->addWidget(descLabel);
    layout->addWidget(descEdit);
    layout->addLayout(buttonLayout);

    connect(saveButton, &QPushButton::clicked, [&dialog, categoryCombo, amountEdit, descEdit, expenseToEdit, this]() {
        QString categoryId = categoryCombo->currentData().toString();
        QString amountText = amountEdit->text().trimmed();
        QString description = descEdit->toPlainText().trimmed();

        if (categoryId.isEmpty() || amountText.isEmpty()) {
            QMessageBox::warning(&dialog, "Lỗi", "Vui lòng điền đầy đủ thông tin!");
            return;
        }

        bool ok;
        double amount = amountText.toDouble(&ok);
        if (!ok || amount <= 0) {
            QMessageBox::warning(&dialog, "Lỗi", "Số tiền không hợp lệ!");
            return;
        }


        expenseToEdit->setAmount(amount);
        expenseToEdit->setDescription(description);
        Category* newCategory = App::findCategoryByID(categoryId);
        if (newCategory) {
            expenseToEdit->setCategory(newCategory);
        }


        App::writeData();


        refreshAllDataViews();
        updateDashboardOverview();

        QMessageBox::information(&dialog, "Thành công", "Đã cập nhật chi tiêu thành công!");
        dialog.accept();
    });

    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    dialog.exec();
}

void Dashboard::removeIncomeTransaction(const QString& incomeId) {

    bool found = App::removeIncomeById(incomeId);

    if (!found) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy thu nhập để xóa!");
        return;
    }


    App::writeData();


    refreshAllDataViews();
    updateDashboardOverview();

    QMessageBox::information(this, "Thành công", "Đã xóa thu nhập thành công!");
}

void Dashboard::removeExpenseTransaction(const QString& expenseId) {

    bool found = App::removeExpenseById(expenseId);

    if (!found) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy chi tiêu để xóa!");
        return;
    }


    App::writeData();


    refreshAllDataViews();
    updateDashboardOverview();

    QMessageBox::information(this, "Thành công", "Đã xóa chi tiêu thành công!");
}

void Dashboard::applyExpenseFiltersToMainList() {
    if (!expenseFilterState.active) {
        renderCards();
        return;
    }


    if (ui->expenseList->widget()) {
        QWidget* contents = ui->expenseList->widget();
        if (contents->layout()) {
            while (QLayoutItem* item = contents->layout()->takeAt(0)) {
                if (item->widget()) {
                    item->widget()->deleteLater();
                }
                delete item;
            }
        }
    }


    QVector<Expense> filteredExpenses;
    for (const Expense& exp : App::getExpenseList()) {
        bool passesFilter = true;


        if (expenseFilterState.categoryFilter != "all") {
            QString categoryName = exp.getCategory() ? exp.getCategory()->getName() : "Unknown Category";
            if (categoryName != expenseFilterState.categoryFilter) {
                passesFilter = false;
            }
        }


        if (expenseFilterState.dateFilterActive) {
            QDate expenseDate = exp.getCreatedAt().date();
            if (expenseDate < expenseFilterState.startDate ||
                expenseDate > expenseFilterState.endDate) {
                passesFilter = false;
            }
        }


        if (expenseFilterState.amountFilterActive) {
            double amount = exp.getAmount();
            if (amount < expenseFilterState.minAmount ||
                amount > expenseFilterState.maxAmount) {
                passesFilter = false;
            }
        }

        if (passesFilter) {
            filteredExpenses.append(exp);
        }
    }


    std::sort(filteredExpenses.begin(), filteredExpenses.end(),
        [this](const Expense& a, const Expense& b) {
            if (expenseFilterState.sortBy == "amount") {
                return expenseFilterState.ascending ? a.getAmount() < b.getAmount() : a.getAmount() > b.getAmount();
            } else if (expenseFilterState.sortBy == "date") {
                return expenseFilterState.ascending ? a.getCreatedAt() < b.getCreatedAt() : a.getCreatedAt() > b.getCreatedAt();
            } else {
                QString catA = a.getCategory() ? a.getCategory()->getName() : "Unknown Category";
                QString catB = b.getCategory() ? b.getCategory()->getName() : "Unknown Category";
                return expenseFilterState.ascending ? catA < catB : catA > catB;
            }
        });


    int index = 0;
    for (const Expense& exp : filteredExpenses) {
        QFrame* card = exp.createCard(QString::number(index++));

        QPushButton* editBtn = card->findChild<QPushButton*>("edit_" + exp.getID());
        QPushButton* deleteBtn = card->findChild<QPushButton*>("delete_" + exp.getID());

        if (editBtn) {
            connect(editBtn, &QPushButton::clicked, this, [this, id = exp.getID()]() {
                editExpenseTransaction(id);
            });
        }
        if (deleteBtn) {
            connect(deleteBtn, &QPushButton::clicked, this, [this, id = exp.getID()]() {
                removeExpenseTransaction(id);
            });
        }

        QScrollArea *scroll = ui->expenseList;
        if (scroll && scroll->widget() && scroll->widget()->layout()) {
            qobject_cast<QVBoxLayout*>(scroll->widget()->layout())->insertWidget(0, card);
        }
    }
}

void Dashboard::applyIncomeFiltersToMainList() {
    if (!incomeFilterState.active) {
        renderCards();
        return;
    }


    if (ui->incomeList->widget()) {
        QWidget* contents = ui->incomeList->widget();
        if (contents->layout()) {
            while (QLayoutItem* item = contents->layout()->takeAt(0)) {
                if (item->widget()) {
                    item->widget()->deleteLater();
                }
                delete item;
            }
        }
    }


    QVector<Income> filteredIncomes;
    for (const Income& inc : App::getIncomeList()) {
        bool passesFilter = true;


        if (incomeFilterState.categoryFilter != "all") {
            QString categoryName = inc.getCategory() ? inc.getCategory()->getName() : "Unknown Category";
            if (categoryName != incomeFilterState.categoryFilter) {
                passesFilter = false;
            }
        }


        if (incomeFilterState.dateFilterActive) {
            QDate incomeDate = inc.getCreatedAt().date();
            if (incomeDate < incomeFilterState.startDate ||
                incomeDate > incomeFilterState.endDate) {
                passesFilter = false;
            }
        }


        if (incomeFilterState.amountFilterActive) {
            double amount = inc.getAmount();
            if (amount < incomeFilterState.minAmount ||
                amount > incomeFilterState.maxAmount) {
                passesFilter = false;
            }
        }

        if (passesFilter) {
            filteredIncomes.append(inc);
        }
    }


    std::sort(filteredIncomes.begin(), filteredIncomes.end(),
        [this](const Income& a, const Income& b) {
            if (incomeFilterState.sortBy == "amount") {
                return incomeFilterState.ascending ? a.getAmount() < b.getAmount() : a.getAmount() > b.getAmount();
            } else if (incomeFilterState.sortBy == "date") {
                return incomeFilterState.ascending ? a.getCreatedAt() < b.getCreatedAt() : a.getCreatedAt() > b.getCreatedAt();
            } else {
                QString catA = a.getCategory() ? a.getCategory()->getName() : "Unknown Category";
                QString catB = b.getCategory() ? b.getCategory()->getName() : "Unknown Category";
                return incomeFilterState.ascending ? catA < catB : catA > catB;
            }
        });


    int index = 0;
    for (const Income& inc : filteredIncomes) {
        QFrame* card = inc.createCard(QString::number(index++));

        QPushButton* editBtn = card->findChild<QPushButton*>("edit_" + inc.getID());
        QPushButton* deleteBtn = card->findChild<QPushButton*>("delete_" + inc.getID());

        if (editBtn) {
            connect(editBtn, &QPushButton::clicked, this, [this, id = inc.getID()]() {
                editIncomeTransaction(id);
            });
        }
        if (deleteBtn) {
            connect(deleteBtn, &QPushButton::clicked, this, [this, id = inc.getID()]() {
                removeIncomeTransaction(id);
            });
        }

        QScrollArea *scroll = ui->incomeList;
        if (scroll && scroll->widget() && scroll->widget()->layout()) {
            qobject_cast<QVBoxLayout*>(scroll->widget()->layout())->insertWidget(0, card);
        }
    }
}

void Dashboard::clearAllFilters() {
    expenseFilterState.active = false;
    incomeFilterState.active = false;
    renderCards();
}







void Dashboard::applySortToMainList(const QString& type, const QString& sortBy, bool ascending) {
    if (type == "expense") {
        expenseFilterState.sortBy = sortBy;
        expenseFilterState.ascending = ascending;
        expenseFilterState.active = true;
        applyExpenseFiltersToMainList();
    } else if (type == "income") {
        incomeFilterState.sortBy = sortBy;
        incomeFilterState.ascending = ascending;
        incomeFilterState.active = true;
        applyIncomeFiltersToMainList();
    }
}


ChartWidget::ChartWidget(QWidget *parent) : QWidget(parent), m_totalIncome(0.0), m_totalExpense(0.0)
{
    setMinimumSize(500, 300);
    setAttribute(Qt::WA_OpaquePaintEvent, false);
}

void ChartWidget::setData(double totalIncome, double totalExpense)
{
    m_totalIncome = totalIncome;
    m_totalExpense = totalExpense;
    update();
}

void ChartWidget::updateChart()
{
    update();
}

void ChartWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);


    painter.fillRect(rect(), QColor(255, 255, 255));


    QRect chartRect = rect().adjusted(50, 30, -150, -50);


    if (m_totalIncome > 0 || m_totalExpense > 0) {
        drawPieChart(&painter, chartRect);
        drawLegend(&painter, rect());
    } else {

        painter.setPen(QColor(100, 100, 100));
        painter.setFont(QFont("Segoe UI", 14));
        painter.drawText(rect(), Qt::AlignCenter, "Không có dữ liệu");
    }
}

void ChartWidget::drawPieChart(QPainter* painter, const QRect& rect)
{
    painter->save();

    double total = m_totalIncome + m_totalExpense;
    if (total <= 0) {
        painter->restore();
        return;
    }


    int incomeAngle = (int)(m_totalIncome / total * 360 * 16);
    int expenseAngle = (int)(m_totalExpense / total * 360 * 16);


    int size = qMin(rect.width(), rect.height()) - 20;
    QRect pieRect((rect.width() - size) / 2 + rect.left(),
                  (rect.height() - size) / 2 + rect.top(),
                  size, size);


    if (incomeAngle > 0) {
        painter->setBrush(QColor(34, 139, 34));
        painter->setPen(QPen(QColor(255, 255, 255), 2));
        painter->drawPie(pieRect, 0, incomeAngle);
    }


    if (expenseAngle > 0) {
        painter->setBrush(QColor(220, 20, 60));
        painter->setPen(QPen(QColor(255, 255, 255), 2));
        painter->drawPie(pieRect, incomeAngle, expenseAngle);
    }

    painter->restore();
}

void ChartWidget::drawLegend(QPainter* painter, const QRect& rect)
{
    painter->save();


    painter->setFont(QFont("Segoe UI", 12, QFont::Bold));


    int legendX = rect.right() - 140;
    int legendY = rect.top() + 50;


    painter->setBrush(QColor(34, 139, 34));
    painter->setPen(Qt::NoPen);
    painter->drawRect(legendX, legendY, 15, 15);

    painter->setPen(QColor(50, 50, 50));
    painter->drawText(legendX + 25, legendY + 12, "Thu nhập");


    painter->setFont(QFont("Segoe UI", 10));
    QString incomeText = QString::number(m_totalIncome, 'f', 0) + " đ";
    painter->drawText(legendX + 25, legendY + 28, incomeText);


    legendY += 50;
    painter->setBrush(QColor(220, 20, 60));
    painter->setPen(Qt::NoPen);
    painter->drawRect(legendX, legendY, 15, 15);

    painter->setPen(QColor(50, 50, 50));
    painter->setFont(QFont("Segoe UI", 12, QFont::Bold));
    painter->drawText(legendX + 25, legendY + 12, "Chi tiêu");


    painter->setFont(QFont("Segoe UI", 10));
    QString expenseText = QString::number(m_totalExpense, 'f', 0) + " đ";
    painter->drawText(legendX + 25, legendY + 28, expenseText);


    legendY += 50;
    painter->setPen(QColor(80, 80, 80));
    painter->setFont(QFont("Segoe UI", 11, QFont::Bold));
    painter->drawText(legendX, legendY, "Tổng cộng:");

    painter->setFont(QFont("Segoe UI", 10));
    double total = m_totalIncome + m_totalExpense;
    QString totalText = QString::number(total, 'f', 0) + " đ";
    painter->drawText(legendX, legendY + 18, totalText);


    if (total > 0) {
        legendY += 40;
        painter->setFont(QFont("Segoe UI", 9));
        painter->setPen(QColor(100, 100, 100));

        double incomePercent = (m_totalIncome / total) * 100;
        double expensePercent = (m_totalExpense / total) * 100;

        painter->drawText(legendX, legendY, QString("Thu nhập: %1%").arg(QString::number(incomePercent, 'f', 1)));
        painter->drawText(legendX, legendY + 15, QString("Chi tiêu: %1%").arg(QString::number(expensePercent, 'f', 1)));
    }

    painter->restore();
}


void Dashboard::setupChart()
{

    QWidget* chartWidget = this->findChild<QWidget*>("chartWidget");
    if (chartWidget) {

        m_chartWidget = new ChartWidget(chartWidget);


        QVBoxLayout* layout = new QVBoxLayout(chartWidget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(m_chartWidget);


        qDebug() << "Setting up chart with initial data";
        updateChart();


        QTimer* chartUpdateTimer = new QTimer(this);
        connect(chartUpdateTimer, &QTimer::timeout, this, &Dashboard::updateChart);
        chartUpdateTimer->start(5000);
    } else {
        qDebug() << "Chart widget not found in UI!";
    }
}

void Dashboard::updateChart()
{
    if (!m_chartWidget) {
        qDebug() << "Chart widget is null!";
        return;
    }

    double totalIncome = getTotalIncomeForCurrentMonth();
    double totalExpense = getTotalExpenseForCurrentMonth();

    qDebug() << "Updating chart with income:" << totalIncome << "expense:" << totalExpense;

    m_chartWidget->setData(totalIncome, totalExpense);
}

double Dashboard::getTotalIncomeForCurrentMonth()
{
    double total = 0.0;

    QDate currentDate = QDate::currentDate();
    int currentYear = currentDate.year();
    int currentMonth = currentDate.month();


    const QVector<Income>& incomes = App::getIncomeList();

    qDebug() << "Total incomes in list:" << incomes.size();
    qDebug() << "Looking for month:" << currentMonth << "year:" << currentYear;

    for (const Income& income : incomes) {
        QDate incomeDate = income.getCreatedAt().date();
        qDebug() << "Income date:" << incomeDate << "amount:" << income.getAmount();

        if (incomeDate.year() == currentYear && incomeDate.month() == currentMonth) {
            total += income.getAmount();
            qDebug() << "Added income:" << income.getAmount() << "new total:" << total;
        }
    }

    qDebug() << "Final income total:" << total;


    if (total == 0 && incomes.isEmpty()) {
        qDebug() << "No income data found, using sample data";
        return 5000000;
    }

    return total;
}

double Dashboard::getTotalExpenseForCurrentMonth()
{
    double total = 0.0;

    QDate currentDate = QDate::currentDate();
    int currentYear = currentDate.year();
    int currentMonth = currentDate.month();


    const QVector<Expense>& expenses = App::getExpenseList();

    qDebug() << "Total expenses in list:" << expenses.size();
    qDebug() << "Looking for month:" << currentMonth << "year:" << currentYear;

    for (const Expense& expense : expenses) {
        QDate expenseDate = expense.getCreatedAt().date();
        qDebug() << "Expense date:" << expenseDate << "amount:" << expense.getAmount();

        if (expenseDate.year() == currentYear && expenseDate.month() == currentMonth) {
            total += expense.getAmount();
            qDebug() << "Added expense:" << expense.getAmount() << "new total:" << total;
        }
    }

    qDebug() << "Final expense total:" << total;

    if (total == 0 && expenses.isEmpty()) {
        qDebug() << "No expense data found, using sample data";
        return 3000000;
    }

    return total;
}

void Dashboard::updateSavingStatistics()
{
    const QVector<Saving>& savings = App::getSavingList();

    double totalSavings = 0.0;
    int goalsCount = savings.size();
    double totalProgress = 0.0;

    for (const Saving& saving : savings) {
        totalSavings += saving.getCurrentAmount();
        if (saving.getTargetAmount() > 0) {
            totalProgress += (saving.getCurrentAmount() / saving.getTargetAmount()) * 100.0;
        }
    }

    double averageProgress = goalsCount > 0 ? totalProgress / goalsCount : 0.0;


    if (ui->totalSavingAmount) {
        ui->totalSavingAmount->setText(App::formatMoney(totalSavings));
    }

    if (ui->goalsCountNumber) {
        ui->goalsCountNumber->setText(QString::number(goalsCount) + " mục tiêu");
    }

    if (ui->progressPercent) {
        ui->progressPercent->setText(QString::number(averageProgress, 'f', 1) + "%");
    }
}

void Dashboard::refreshSavingGoals()
{
    clearSavingGoalsDisplay();

    const QVector<Saving>& savings = App::getSavingList();

    if (savings.isEmpty()) {
        if (ui->noGoalsLabel) {
            ui->noGoalsLabel->show();
        }
        return;
    }

    if (ui->noGoalsLabel) {
        ui->noGoalsLabel->hide();
    }


    for (const Saving& saving : savings) {
        createSavingGoalCard(saving);
    }
}

void Dashboard::createSavingGoalCard(const Saving& saving)
{
    QFrame* card = saving.createCard(saving.getID());

    // Connect buttons
    QPushButton* depositBtn = card->findChild<QPushButton*>("deposit_" + saving.getID());
    QPushButton* withdrawBtn = card->findChild<QPushButton*>("withdraw_" + saving.getID());
    QPushButton* editBtn = card->findChild<QPushButton*>("edit_" + saving.getID());
    QPushButton* deleteBtn = card->findChild<QPushButton*>("delete_" + saving.getID());

    if (depositBtn) {
        connect(depositBtn, &QPushButton::clicked, this, [this, id = saving.getID()]() {
            showSavingTransactionDialog("deposit", id);
        });
    }
    if (withdrawBtn) {
        connect(withdrawBtn, &QPushButton::clicked, this, [this, id = saving.getID()]() {
            showSavingTransactionDialog("withdraw", id);
        });
    }
    if (editBtn) {
        connect(editBtn, &QPushButton::clicked, this, [this, id = saving.getID()]() {
            showEditSavingGoalDialog(id);
        });
    }
    if (deleteBtn) {
        connect(deleteBtn, &QPushButton::clicked, this, [this, id = saving.getID()]() {
            deleteSavingGoal(id);
        });
    }

    if (card && ui->savingListContentLayout) {
        ui->savingListContentLayout->insertWidget(ui->savingListContentLayout->count() - 1, card);
    }
}

void Dashboard::clearSavingGoalsDisplay()
{
    if (!ui->savingListContentLayout) return;

    QLayoutItem* item;
    while ((item = ui->savingListContentLayout->takeAt(0)) != nullptr) {
        if (item->widget() && item->widget()->objectName().startsWith("savingGoal_")) {
            delete item->widget();
        }
        delete item;
    }


    QSpacerItem* spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->savingListContentLayout->addItem(spacer);
}

void Dashboard::on_addSavingGoalBtn_clicked()
{
    showAddSavingGoalDialog();
}

void Dashboard::on_depositBtn_clicked()
{

    showSavingTransactionDialog("deposit", "");
}

void Dashboard::on_withdrawBtn_clicked()
{

    showSavingTransactionDialog("withdraw", "");
}

void Dashboard::on_savingReportBtn_clicked()
{
    showSavingReportDialog();
}

void Dashboard::on_savingSearchButton_clicked()
{
    QString keyword = ui->savingSearchInput->text().trimmed();
    if (keyword.isEmpty()) {
        on_savingClearButton_clicked();
        return;
    }

    searchSavingGoals(keyword);
}

void Dashboard::on_savingClearButton_clicked()
{
    ui->savingSearchInput->clear();
    savingSearchModeActive = false;
    originalSavingCards.clear();
    refreshSavingGoals();
}

void Dashboard::onSavingSortChanged(const QString& sortType)
{
    applySavingSorting(sortType);
}


void Dashboard::showAddSavingGoalDialog()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Thêm Mục Tiêu Tiết Kiệm");
    dialog.setModal(true);
    dialog.resize(400, 300);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);


    QLabel* nameLabel = new QLabel("Tên mục tiêu:");
    QLineEdit* nameEdit = new QLineEdit();
    nameEdit->setPlaceholderText("VD: Mua nhà, Du lịch...");


    QLabel* amountLabel = new QLabel("Số tiền mục tiêu:");
    QSpinBox* amountSpinBox = new QSpinBox();
    amountSpinBox->setRange(1000, 999999999);
    amountSpinBox->setSuffix(" VNĐ");
    amountSpinBox->setValue(1000000);


    QLabel* descLabel = new QLabel("Mô tả:");
    QTextEdit* descEdit = new QTextEdit();
    descEdit->setMaximumHeight(80);
    descEdit->setPlaceholderText("Mô tả chi tiết về mục tiêu tiết kiệm...");


    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* okBtn = new QPushButton("Thêm");
    QPushButton* cancelBtn = new QPushButton("Hủy");

    okBtn->setStyleSheet("QPushButton { background-color: rgb(34, 139, 34); color: white; padding: 8px 16px; border-radius: 4px; }");
    cancelBtn->setStyleSheet("QPushButton { background-color: rgb(128, 128, 128); color: white; padding: 8px 16px; border-radius: 4px; }");

    buttonLayout->addStretch();
    buttonLayout->addWidget(okBtn);
    buttonLayout->addWidget(cancelBtn);

    layout->addWidget(nameLabel);
    layout->addWidget(nameEdit);
    layout->addWidget(amountLabel);
    layout->addWidget(amountSpinBox);
    layout->addWidget(descLabel);
    layout->addWidget(descEdit);
    layout->addLayout(buttonLayout);

    connect(okBtn, &QPushButton::clicked, [&dialog, nameEdit, amountSpinBox, descEdit, this]() {
        QString name = nameEdit->text().trimmed();
        if (name.isEmpty()) {
            QMessageBox::warning(&dialog, "Lỗi", "Vui lòng nhập tên mục tiêu!");
            return;
        }


        QString id = generateNextSavingId();
        Saving newSaving(id, QDateTime::currentDateTime(), QDate::currentDate(),
                        0.0, descEdit->toPlainText(), name, QDate::currentDate(),
                        amountSpinBox->value());


        const_cast<QVector<Saving>&>(App::getSavingList()).append(newSaving);
        App::writeData();


        updateSavingStatistics();
        refreshSavingGoals();

        dialog.accept();
    });

    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    dialog.exec();
}

void Dashboard::showSavingTransactionDialog(const QString& type, const QString& goalId)
{
    QDialog dialog(this);
    dialog.setWindowTitle(type == "deposit" ? "Nạp Tiền Tiết Kiệm" : "Rút Tiền Tiết Kiệm");
    dialog.setModal(true);
    dialog.resize(350, 250);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);


    QLabel* goalLabel = new QLabel("Chọn mục tiêu:");
    QComboBox* goalCombo = new QComboBox();

    const QVector<Saving>& savings = App::getSavingList();
    for (const Saving& saving : savings) {
        goalCombo->addItem(saving.getName(), saving.getID());
        if (!goalId.isEmpty() && saving.getID() == goalId) {
            goalCombo->setCurrentIndex(goalCombo->count() - 1);
        }
    }

    if (goalCombo->count() == 0) {
        QMessageBox::information(this, "Thông báo", "Chưa có mục tiêu tiết kiệm nào. Hãy tạo mục tiêu trước!");
        return;
    }


    QLabel* amountLabel = new QLabel("Số tiền:");
    QSpinBox* amountSpinBox = new QSpinBox();
    amountSpinBox->setRange(1000, 999999999);
    amountSpinBox->setSuffix(" VNĐ");
    amountSpinBox->setValue(100000);


    QLabel* descLabel = new QLabel("Ghi chú:");
    QLineEdit* descEdit = new QLineEdit();
    descEdit->setPlaceholderText("Ghi chú về giao dịch...");


    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* okBtn = new QPushButton(type == "deposit" ? "Nạp Tiền" : "Rút Tiền");
    QPushButton* cancelBtn = new QPushButton("Hủy");

    QString okColor = type == "deposit" ? "rgb(34, 139, 34)" : "rgb(255, 87, 34)";
    okBtn->setStyleSheet(QString("QPushButton { background-color: %1; color: white; padding: 8px 16px; border-radius: 4px; }").arg(okColor));
    cancelBtn->setStyleSheet("QPushButton { background-color: rgb(128, 128, 128); color: white; padding: 8px 16px; border-radius: 4px; }");

    buttonLayout->addStretch();
    buttonLayout->addWidget(okBtn);
    buttonLayout->addWidget(cancelBtn);

    layout->addWidget(goalLabel);
    layout->addWidget(goalCombo);
    layout->addWidget(amountLabel);
    layout->addWidget(amountSpinBox);
    layout->addWidget(descLabel);
    layout->addWidget(descEdit);
    layout->addLayout(buttonLayout);

    connect(okBtn, &QPushButton::clicked, [&dialog, goalCombo, amountSpinBox, descEdit, type, this]() {
        QString selectedGoalId = goalCombo->currentData().toString();
        double amount = amountSpinBox->value();


        QVector<Saving>& savingsRef = const_cast<QVector<Saving>&>(App::getSavingList());
        for (Saving& saving : savingsRef) {
            if (saving.getID() == selectedGoalId) {
                if (type == "deposit") {
                    saving.setCurrentAmount(saving.getCurrentAmount() + amount);
                } else {
                    if (saving.getCurrentAmount() < amount) {
                        QMessageBox::warning(&dialog, "Lỗi", "Số tiền rút vượt quá số tiền hiện có!");
                        return;
                    }
                    saving.setCurrentAmount(saving.getCurrentAmount() - amount);
                }
                break;
            }
        }

        App::writeData();
        updateSavingStatistics();
        refreshSavingGoals();

        QString message = type == "deposit" ?
            QString("Đã nạp %1 VNĐ vào mục tiêu tiết kiệm!").arg(App::formatMoney(amount)) :
            QString("Đã rút %1 VNĐ từ mục tiêu tiết kiệm!").arg(App::formatMoney(amount));
        QMessageBox::information(&dialog, "Thành công", message);

        dialog.accept();
    });

    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    dialog.exec();
}

void Dashboard::showEditSavingGoalDialog(const QString& goalId)
{

    const QVector<Saving>& savings = App::getSavingList();
    const Saving* targetSaving = nullptr;

    for (const Saving& saving : savings) {
        if (saving.getID() == goalId) {
            targetSaving = &saving;
            break;
        }
    }

    if (!targetSaving) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy mục tiêu tiết kiệm!");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Chỉnh Sửa Mục Tiêu");
    dialog.setModal(true);
    dialog.resize(400, 300);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);


    QLabel* nameLabel = new QLabel("Tên mục tiêu:");
    QLineEdit* nameEdit = new QLineEdit(targetSaving->getName());


    QLabel* amountLabel = new QLabel("Số tiền mục tiêu:");
    QSpinBox* amountSpinBox = new QSpinBox();
    amountSpinBox->setRange(1000, 999999999);
    amountSpinBox->setSuffix(" VNĐ");
    amountSpinBox->setValue(targetSaving->getTargetAmount());


    QLabel* descLabel = new QLabel("Mô tả:");
    QTextEdit* descEdit = new QTextEdit();
    descEdit->setMaximumHeight(80);
    descEdit->setPlainText(targetSaving->getDescription());


    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* okBtn = new QPushButton("Lưu");
    QPushButton* cancelBtn = new QPushButton("Hủy");

    okBtn->setStyleSheet("QPushButton { background-color: rgb(0, 123, 255); color: white; padding: 8px 16px; border-radius: 4px; }");
    cancelBtn->setStyleSheet("QPushButton { background-color: rgb(128, 128, 128); color: white; padding: 8px 16px; border-radius: 4px; }");

    buttonLayout->addStretch();
    buttonLayout->addWidget(okBtn);
    buttonLayout->addWidget(cancelBtn);

    layout->addWidget(nameLabel);
    layout->addWidget(nameEdit);
    layout->addWidget(amountLabel);
    layout->addWidget(amountSpinBox);
    layout->addWidget(descLabel);
    layout->addWidget(descEdit);
    layout->addLayout(buttonLayout);

    connect(okBtn, &QPushButton::clicked, [&dialog, nameEdit, amountSpinBox, descEdit, goalId, this]() {
        QString name = nameEdit->text().trimmed();
        if (name.isEmpty()) {
            QMessageBox::warning(&dialog, "Lỗi", "Vui lòng nhập tên mục tiêu!");
            return;
        }


        QVector<Saving>& savingsRef = const_cast<QVector<Saving>&>(App::getSavingList());
        for (Saving& saving : savingsRef) {
            if (saving.getID() == goalId) {
                saving.setName(name);
                saving.setTargetAmount(amountSpinBox->value());
                saving.setDescription(descEdit->toPlainText());
                break;
            }
        }

        App::writeData();
        updateSavingStatistics();
        refreshSavingGoals();

        dialog.accept();
    });

    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    dialog.exec();
}

void Dashboard::showSavingReportDialog()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Báo Cáo Tiết Kiệm");
    dialog.setModal(true);
    dialog.resize(500, 400);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);

    QTextEdit* reportText = new QTextEdit();
    reportText->setReadOnly(true);


    const QVector<Saving>& savings = App::getSavingList();
    QString report = "<h2>Báo Cáo Tiết Kiệm</h2>";
    report += QString("<p><strong>Ngày tạo báo cáo:</strong> %1</p>").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm"));

    if (savings.isEmpty()) {
        report += "<p>Chưa có mục tiêu tiết kiệm nào.</p>";
    } else {
        double totalTarget = 0, totalCurrent = 0;

        report += "<h3>Danh Sách Mục Tiêu:</h3>";
        report += "<table border='1' style='border-collapse: collapse; width: 100%;'>";
        report += "<tr style='background-color: #f0f0f0;'><th>Tên Mục Tiêu</th><th>Hiện Tại</th><th>Mục Tiêu</th><th>Tiến Độ</th></tr>";

        for (const Saving& saving : savings) {
            totalTarget += saving.getTargetAmount();
            totalCurrent += saving.getCurrentAmount();

            double progress = saving.calculateProgress();
            QString progressColor = progress >= 100 ? "green" : (progress >= 50 ? "orange" : "red");

            report += QString("<tr>");
            report += QString("<td>%1</td>").arg(saving.getName());
            report += QString("<td>%1</td>").arg(App::formatMoney(saving.getCurrentAmount()));
            report += QString("<td>%1</td>").arg(App::formatMoney(saving.getTargetAmount()));
            report += QString("<td style='color: %1;'>%2%</td>").arg(progressColor).arg(QString::number(progress, 'f', 1));
            report += QString("</tr>");
        }

        report += "</table>";

        report += "<h3>Tóm Tắt:</h3>";
        report += QString("<p><strong>Tổng số mục tiêu:</strong> %1</p>").arg(savings.size());
        report += QString("<p><strong>Tổng tiền hiện tại:</strong> %1</p>").arg(App::formatMoney(totalCurrent));
        report += QString("<p><strong>Tổng tiền mục tiêu:</strong> %1</p>").arg(App::formatMoney(totalTarget));

        double overallProgress = totalTarget > 0 ? (totalCurrent / totalTarget) * 100.0 : 0.0;
        report += QString("<p><strong>Tiến độ tổng thể:</strong> %1%</p>").arg(QString::number(overallProgress, 'f', 1));
    }

    reportText->setHtml(report);

    QPushButton* closeBtn = new QPushButton("Đóng");
    closeBtn->setStyleSheet("QPushButton { background-color: rgb(128, 128, 128); color: white; padding: 8px 16px; border-radius: 4px; }");

    layout->addWidget(reportText);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeBtn);
    layout->addLayout(buttonLayout);

    connect(closeBtn, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.exec();
}

void Dashboard::searchSavingGoals(const QString& keyword)
{
    if (keyword.isEmpty()) {
        on_savingClearButton_clicked();
        return;
    }


    if (!savingSearchModeActive) {
        originalSavingCards.clear();
        if (ui->savingListContentLayout) {
            for (int i = 0; i < ui->savingListContentLayout->count(); ++i) {
                QLayoutItem* item = ui->savingListContentLayout->itemAt(i);
                if (item && item->widget() && item->widget()->objectName().startsWith("savingGoal_")) {
                    QFrame* card = qobject_cast<QFrame*>(item->widget());
                    if (card) {
                        originalSavingCards.append(qMakePair(keyword, card));
                    }
                }
            }
        }
        savingSearchModeActive = true;
    }


    clearSavingGoalsDisplay();


    const QVector<Saving>& savings = App::getSavingList();
    bool hasResults = false;

    for (const Saving& saving : savings) {
        bool matches = saving.getName().contains(keyword, Qt::CaseInsensitive) ||
                      saving.getDescription().contains(keyword, Qt::CaseInsensitive);

        if (matches) {
            createSavingGoalCard(saving);
            hasResults = true;
        }
    }

    if (!hasResults) {
        QLabel* noResultsLabel = new QLabel("Không tìm thấy mục tiêu nào phù hợp.");
        noResultsLabel->setStyleSheet("font: 12pt 'Segoe UI'; color: rgb(128, 128, 128); padding: 40px;");
        noResultsLabel->setAlignment(Qt::AlignCenter);
        ui->savingListContentLayout->insertWidget(0, noResultsLabel);
    }
}

void Dashboard::applySavingSorting(const QString& sortType)
{
    QVector<Saving> sortedSavings = App::getSavingList();

    if (sortType.contains("tên")) {
        std::sort(sortedSavings.begin(), sortedSavings.end(),
                 [](const Saving& a, const Saving& b) {
                     return a.getName().compare(b.getName(), Qt::CaseInsensitive) < 0;
                 });
    } else if (sortType.contains("số tiền")) {
        std::sort(sortedSavings.begin(), sortedSavings.end(),
                 [](const Saving& a, const Saving& b) {
                     return a.getTargetAmount() > b.getTargetAmount();
                 });
    } else if (sortType.contains("tiến độ")) {
        std::sort(sortedSavings.begin(), sortedSavings.end(),
                 [](const Saving& a, const Saving& b) {
                     double progressA = a.calculateProgress();
                     double progressB = b.calculateProgress();
                     return progressA > progressB;
                 });
    } else if (sortType.contains("ngày tạo")) {
        std::sort(sortedSavings.begin(), sortedSavings.end(),
                 [](const Saving& a, const Saving& b) {
                     return a.getCreatedAt() > b.getCreatedAt();
                 });
    }


    clearSavingGoalsDisplay();

    for (const Saving& saving : sortedSavings) {
        createSavingGoalCard(saving);
    }
}

void Dashboard::deleteSavingGoal(const QString& goalId)
{
    QMessageBox::StandardButton reply = QMessageBox::question(this,
        "Xác Nhận Xóa",
        "Bạn có chắc chắn muốn xóa mục tiêu tiết kiệm này?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QVector<Saving>& savingsRef = const_cast<QVector<Saving>&>(App::getSavingList());

        for (int i = 0; i < savingsRef.size(); ++i) {
            if (savingsRef[i].getID() == goalId) {
                savingsRef.removeAt(i);
                break;
            }
        }

        App::writeData();
        updateSavingStatistics();
        refreshSavingGoals();

        QMessageBox::information(this, "Thành công", "Đã xóa mục tiêu tiết kiệm!");
    }
}

QString Dashboard::generateNextSavingId()
{
    const QVector<Saving>& savings = App::getSavingList();
    int maxId = 0;

    for (const Saving& saving : savings) {
        QString idStr = saving.getID();
        if (idStr.startsWith("SV")) {
            bool ok;
            int id = idStr.mid(2).toInt(&ok);
            if (ok && id > maxId) {
                maxId = id;
            }
        }
    }

    return QString("SV%1").arg(maxId + 1, 4, 10, QChar('0'));
}

// Debt management methods
void Dashboard::renderDebtCards() {
    renderDebtCards(App::getDebtList());
}

void Dashboard::renderDebtCards(const QVector<Debt>& debts) {
    // Try to find existing widget, if not create it dynamically
    QWidget* debtPage = ui->stackedWidget->widget(2);  // Index 2 is for debt page
    if (!debtPage) {
        qDebug() << "Error: Debt page not found at index 2";
        return;
    }

    // Look for scroll area or create one
    QScrollArea* scrollArea = debtPage->findChild<QScrollArea*>("debtList");
    if (!scrollArea) {
        // Create scroll area dynamically if not in UI
        scrollArea = new QScrollArea(debtPage);
        scrollArea->setObjectName("debtList");
        scrollArea->setWidgetResizable(true);

        // Find or create layout for debt page
        if (!debtPage->layout()) {
            QVBoxLayout* pageLayout = new QVBoxLayout(debtPage);
            pageLayout->setContentsMargins(10, 10, 10, 10);
        }
        debtPage->layout()->addWidget(scrollArea);
    }

    // Get or create content widget
    QWidget* debtListContent = scrollArea->widget();
    if (!debtListContent) {
        debtListContent = new QWidget();
        debtListContent->setObjectName("debtListContent");
        scrollArea->setWidget(debtListContent);
    }

    // Clear existing layout
    QLayout* existingLayout = debtListContent->layout();
    if (existingLayout) {
        QLayoutItem* item;
        while ((item = existingLayout->takeAt(0)) != nullptr) {
            if (item->widget()) {
                delete item->widget();
            }
            delete item;
        }
        delete existingLayout;
    }

    QVBoxLayout* layout = new QVBoxLayout(debtListContent);
    layout->setSpacing(10);
    layout->setContentsMargins(10, 10, 10, 10);

    originalDebtCards.clear();

    for (int i = 0; i < debts.size(); ++i) {
        const Debt& debt = debts[i];
        QFrame* card = debt.createCard(QString::number(i));

        // Connect edit button
        QPushButton* editBtn = card->findChild<QPushButton*>("edit_" + debt.getID());
        if (editBtn) {
            connect(editBtn, &QPushButton::clicked, this, &Dashboard::handleDebtCardAction);
        }

        // Connect delete button
        QPushButton* deleteBtn = card->findChild<QPushButton*>("delete_" + debt.getID());
        if (deleteBtn) {
            connect(deleteBtn, &QPushButton::clicked, this, &Dashboard::handleDebtCardAction);
        }

        // Connect mark paid button
        QPushButton* markPaidBtn = card->findChild<QPushButton*>("markPaid_" + debt.getID());
        if (markPaidBtn) {
            connect(markPaidBtn, &QPushButton::clicked, this, &Dashboard::handleDebtCardAction);
        }

        layout->addWidget(card);
        originalDebtCards.append(qMakePair(debt.getID(), card));
    }

    layout->addStretch();
}

void Dashboard::updateDebtOverview() {
    const QVector<Debt>& debts = App::getDebtList();

    double totalBorrowed = Debt::getTotalBorrowed(debts);
    double totalLent = Debt::getTotalLent(debts);
    double unpaidTotal = Debt::getTotalUnpaid(debts, "");

    // Update UI labels
    QLabel* borrowedLabel = this->findChild<QLabel*>("totalBorrowedLabel");
    if (borrowedLabel) {
        borrowedLabel->setText(QString("<html><head/><body><p><span style=\" font-size:18pt; color:#ffc107;\">%1</span></p></body></html>")
                               .arg(App::formatMoney(totalBorrowed)));
    }

    QLabel* lentLabel = this->findChild<QLabel*>("totalLentLabel");
    if (lentLabel) {
        lentLabel->setText(QString("<html><head/><body><p><span style=\" font-size:18pt; color:#28a745;\">%1</span></p></body></html>")
                          .arg(App::formatMoney(totalLent)));
    }

    QLabel* unpaidLabel = this->findChild<QLabel*>("totalUnpaidLabel");
    if (unpaidLabel) {
        unpaidLabel->setText(QString("<html><head/><body><p><span style=\" font-size:18pt; color:#dc3545;\">%1</span></p></body></html>")
                            .arg(App::formatMoney(unpaidTotal)));
    }
}

void Dashboard::handleDebtCardAction() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    QString objectName = button->objectName();
    QString debtId = objectName.mid(objectName.indexOf('_') + 1);

    if (objectName.startsWith("edit_")) {
        editDebtTransaction(debtId);
    } else if (objectName.startsWith("delete_")) {
        removeDebtTransaction(debtId);
    } else if (objectName.startsWith("markPaid_")) {
        markDebtAsPaid(debtId);
    }
}

void Dashboard::editDebtTransaction(const QString& debtId) {
    qDebug() << "Edit debt with ID:" << debtId;
    QMessageBox::information(this, "Chỉnh sửa khoản nợ", "Chức năng chỉnh sửa đang được phát triển.");
}

void Dashboard::removeDebtTransaction(const QString& debtId) {
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Xác nhận xóa",
        "Bạn có chắc chắn muốn xóa khoản nợ này?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        if (App::removeDebtById(debtId)) {
            renderDebtCards();
            updateDebtOverview();
            App::writeData();
            QMessageBox::information(this, "Thành công", "Đã xóa khoản nợ!");
        } else {
            QMessageBox::warning(this, "Lỗi", "Không tìm thấy khoản nợ!");
        }
    }
}

void Dashboard::markDebtAsPaid(const QString& debtId) {
    const QVector<Debt>& debts = App::getDebtList();
    bool currentStatus = false;

    for (const Debt& debt : debts) {
        if (debt.getID() == debtId) {
            currentStatus = debt.getIsPaid();
            break;
        }
    }

    if (App::markDebtAsPaid(debtId, !currentStatus)) {
        renderDebtCards();
        updateDebtOverview();
        App::writeData();
        QString message = currentStatus ? "Đã đánh dấu chưa thanh toán!" : "Đã đánh dấu đã thanh toán!";
        QMessageBox::information(this, "Thành công", message);
    } else {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy khoản nợ!");
    }
}

void Dashboard::searchDebt(const QString& keyword) {
    if (keyword.isEmpty()) {
        clearDebtSearch();
        return;
    }

    debtSearchModeActive = true;
    QVector<Debt> filteredDebts;

    for (const Debt& debt : App::getDebtList()) {
        if (debt.getDescription().contains(keyword, Qt::CaseInsensitive) ||
            debt.getDebtorName().contains(keyword, Qt::CaseInsensitive) ||
            debt.getDebtType().contains(keyword, Qt::CaseInsensitive) ||
            QString::number(debt.getAmount()).contains(keyword)) {
            filteredDebts.append(debt);
        }
    }

    renderDebtCards(filteredDebts);
}

void Dashboard::clearDebtSearch() {
    debtSearchModeActive = false;
    renderDebtCards();
}

void Dashboard::on_addDebtButton_clicked() {
    QDialog dialog(this);
    dialog.setWindowTitle("Thêm Khoản Nợ");
    dialog.setModal(true);
    dialog.resize(450, 450);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);

    // Loại khoản nợ
    QLabel* typeLabel = new QLabel("Loại khoản nợ:", &dialog);
    QComboBox* typeCombo = new QComboBox(&dialog);
    typeCombo->addItem("Vay nợ (Tôi đi vay)", "borrow");
    typeCombo->addItem("Cho vay (Tôi cho vay)", "lend");

    // Danh mục
    QLabel* categoryLabel = new QLabel("Danh mục:", &dialog);
    QComboBox* categoryCombo = new QComboBox(&dialog);
    for (const Category& category : App::getCategoryList()) {
        categoryCombo->addItem(category.getName(), category.getID());
    }

    // Tên người vay/cho vay
    QLabel* debtorLabel = new QLabel("Tên người liên quan:", &dialog);
    QLineEdit* debtorEdit = new QLineEdit(&dialog);
    debtorEdit->setPlaceholderText("Nhập tên người vay/cho vay...");

    // Số tiền
    QLabel* amountLabel = new QLabel("Số tiền:", &dialog);
    QLineEdit* amountEdit = new QLineEdit(&dialog);
    amountEdit->setPlaceholderText("Nhập số tiền...");

    // Ngày đáo hạn
    QLabel* dueDateLabel = new QLabel("Ngày đáo hạn:", &dialog);
    QDateEdit* dueDateEdit = new QDateEdit(&dialog);
    dueDateEdit->setDate(QDate::currentDate().addMonths(1));
    dueDateEdit->setCalendarPopup(true);
    dueDateEdit->setDisplayFormat("dd/MM/yyyy");

    // Mô tả
    QLabel* descLabel = new QLabel("Mô tả:", &dialog);
    QTextEdit* descEdit = new QTextEdit(&dialog);
    descEdit->setPlaceholderText("Nhập mô tả (tùy chọn)...");
    descEdit->setMaximumHeight(80);

    // Trạng thái thanh toán
    QCheckBox* paidCheckBox = new QCheckBox("Đã thanh toán", &dialog);

    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* okButton = new QPushButton("Thêm", &dialog);
    QPushButton* cancelButton = new QPushButton("Hủy", &dialog);
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    // Add all widgets to layout
    layout->addWidget(typeLabel);
    layout->addWidget(typeCombo);
    layout->addWidget(categoryLabel);
    layout->addWidget(categoryCombo);
    layout->addWidget(debtorLabel);
    layout->addWidget(debtorEdit);
    layout->addWidget(amountLabel);
    layout->addWidget(amountEdit);
    layout->addWidget(dueDateLabel);
    layout->addWidget(dueDateEdit);
    layout->addWidget(descLabel);
    layout->addWidget(descEdit);
    layout->addWidget(paidCheckBox);
    layout->addLayout(buttonLayout);

    // Update label based on debt type
    connect(typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [debtorLabel, typeCombo]() {
        QString debtType = typeCombo->currentData().toString();
        if (debtType == "borrow") {
            debtorLabel->setText("Tên người cho vay:");
        } else {
            debtorLabel->setText("Tên người đi vay:");
        }
    });

    connect(okButton, &QPushButton::clicked, [&dialog, typeCombo, categoryCombo, debtorEdit,
                                              amountEdit, dueDateEdit, descEdit, paidCheckBox, this]() {
        QString debtType = typeCombo->currentData().toString();
        QString categoryId = categoryCombo->currentData().toString();
        QString debtorName = debtorEdit->text().trimmed();
        QString amountText = amountEdit->text().trimmed();
        QString description = descEdit->toPlainText().trimmed();
        QDate dueDate = dueDateEdit->date();
        bool isPaid = paidCheckBox->isChecked();

        // Validation
        if (categoryId.isEmpty()) {
            QMessageBox::warning(&dialog, "Lỗi", "Vui lòng chọn danh mục!");
            return;
        }

        if (debtorName.isEmpty()) {
            QMessageBox::warning(&dialog, "Lỗi", "Vui lòng nhập tên người liên quan!");
            return;
        }

        if (amountText.isEmpty()) {
            QMessageBox::warning(&dialog, "Lỗi", "Vui lòng nhập số tiền!");
            return;
        }

        bool ok;
        double amount = amountText.toDouble(&ok);
        if (!ok || amount <= 0) {
            QMessageBox::warning(&dialog, "Lỗi", "Số tiền không hợp lệ!");
            return;
        }

        if (!dueDate.isValid() || dueDate < QDate::currentDate()) {
            QMessageBox::StandardButton reply = QMessageBox::question(
                &dialog,
                "Xác nhận",
                "Ngày đáo hạn đã qua hoặc không hợp lệ. Bạn có muốn tiếp tục?",
                QMessageBox::Yes | QMessageBox::No
            );
            if (reply == QMessageBox::No) {
                return;
            }
        }

        // Add debt
        App::addDebt(categoryId, amount, description, debtorName, dueDate, debtType, isPaid);

        // Refresh UI
        renderDebtCards();
        updateDebtOverview();
        updateDashboardOverview();
        App::writeData();

        QString typeText = (debtType == "borrow") ? "vay nợ" : "cho vay";
        QMessageBox::information(&dialog, "Thành công",
                                QString("Đã thêm khoản %1 thành công!").arg(typeText));
        dialog.accept();
    });

    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    dialog.exec();
}

void Dashboard::on_debtSearchButton_clicked() {
    QLineEdit* searchInput = ui->stackedWidget->findChild<QLineEdit*>("debtSearchInput");
    if (searchInput) {
        searchDebt(searchInput->text());
    }
}

void Dashboard::on_debtClearButton_clicked() {
    QLineEdit* searchInput = ui->stackedWidget->findChild<QLineEdit*>("debtSearchInput");
    if (searchInput) {
        searchInput->clear();
    }

    QComboBox* filterCombo = this->findChild<QComboBox*>("debtFilterCombo");
    if (filterCombo) {
        filterCombo->setCurrentIndex(0); // Reset to "Tất cả"
    }

    clearDebtSearch();
}

void Dashboard::onDebtFilterChanged(int index) {
    QVector<Debt> filteredDebts;
    const QVector<Debt>& allDebts = App::getDebtList();

    switch(index) {
        case 0: // Tất cả
            filteredDebts = allDebts;
            break;
        case 1: // Vay nợ
            for (const Debt& debt : allDebts) {
                if (debt.getDebtType() == "borrow") {
                    filteredDebts.append(debt);
                }
            }
            break;
        case 2: // Cho vay
            for (const Debt& debt : allDebts) {
                if (debt.getDebtType() == "lend") {
                    filteredDebts.append(debt);
                }
            }
            break;
        case 3: // Chưa thanh toán
            for (const Debt& debt : allDebts) {
                if (!debt.getIsPaid()) {
                    filteredDebts.append(debt);
                }
            }
            break;
        case 4: // Đã thanh toán
            for (const Debt& debt : allDebts) {
                if (debt.getIsPaid()) {
                    filteredDebts.append(debt);
                }
            }
            break;
        default:
            filteredDebts = allDebts;
    }

    renderDebtCards(filteredDebts);
}


