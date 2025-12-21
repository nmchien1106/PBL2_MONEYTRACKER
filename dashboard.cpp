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

    updateSavingStatistics();
    refreshSavingGoals();
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

void Dashboard::on_debt_btn_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
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

void Dashboard::renderHtmlCard(
    const QString &categoryName,
    double amount,
    const QDateTime &time,
    const QString &description,
    QString type,
    const QString& parent)
{
    QString amountHtml = QString("<span style='font-size:10pt;'>%1 VNĐ</span>").arg(App::formatMoney(amount));

    QString timeStr = time.toString("dd/MM/yyyy HH:mm");

    QString categoryHtml = QString("<span style='color:#1b76c5;'>%1</span>").arg(categoryName);

    QString descHtml = QString("<span style='font-size:8pt;font-style:italic;color:#848484;'>%1</span>").arg(description);

    QFrame *card = createCategoryCard(amountHtml,timeStr,categoryHtml,descHtml,type);

    QScrollArea *scroll = findChild<QScrollArea *>(parent);
    Q_ASSERT(scroll);

    QWidget *contents = scroll->widget();
    Q_ASSERT(contents);

    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(contents->layout());
    if (layout) {
        
        layout->insertWidget(0, card);
        ensureCardsAtTop(scroll);
    } else {
        contents->layout()->addWidget(card);
    }
}

void Dashboard::renderCards(){
    if (expenseFilterState.active || incomeFilterState.active) {
        if (expenseFilterState.active) {
            applyExpenseFiltersToMainList();
        } else {
            for (const Expense &exp : App::getExpenseList()) {
                QString cat = exp.getCategory() ? exp.getCategory()->getName() : "Unknown Category";
                renderHtmlCard(cat, exp.getAmount(), exp.getCreatedAt(), exp.getDescription(), exp.getID(), "expenseList");
            }
        }

        if (incomeFilterState.active) {
            applyIncomeFiltersToMainList();
        } else {
            for (const Income &inc : App::getIncomeList()) {
                QString cat = inc.getCategory() ? inc.getCategory()->getName() : "Unknown Category";
                renderHtmlCard(cat, inc.getAmount(), inc.getCreatedAt(), inc.getDescription(), inc.getID(), "incomeList");
            }
        }
        return;
    }

    for (const Income &inc : App::getIncomeList())
    {
        QString cat = inc.getCategory() ? inc.getCategory()->getName() : "Unknown Category";
        renderHtmlCard(cat, inc.getAmount(), inc.getCreatedAt(), inc.getDescription(),inc.getID(), "incomeList");
    }

    for (const Expense &exp : App::getExpenseList())
    {
        QString cat = exp.getCategory() ? exp.getCategory()->getName() : "Unknown Category";
        renderHtmlCard(cat, exp.getAmount(), exp.getCreatedAt(), exp.getDescription(), exp.getID(), "expenseList");
    }

}

QFrame *Dashboard::createCategoryCard(
    const QString &amount,
    const QString &time,
    const QString &title,
    const QString &description,
    QString index)
{

    QFrame *cardTemplate = this->findChild<QFrame *>("card_danhmuc");
    cardTemplate->setVisible(false);
    QFrame *card = new QFrame(cardTemplate->parentWidget());


    card->setStyleSheet(cardTemplate->styleSheet());
    card->setMinimumSize(cardTemplate->minimumSize());
    card->setMaximumSize(cardTemplate->maximumSize());
    card->setFrameShape(QFrame::StyledPanel);
    card->setFrameShadow(QFrame::Raised);


    QHBoxLayout *mainLayout = new QHBoxLayout(card);
    mainLayout->setContentsMargins(9, 9, 9, 9);

    QLabel *amountLabel = new QLabel(amount);
    amountLabel->setTextFormat(Qt::RichText);

    QLabel *timeLabel = new QLabel(time);

    QVBoxLayout *infoLayout = new QVBoxLayout();
    infoLayout->setSpacing(0);
    infoLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *titleLabel = new QLabel(title);
    QLabel *descLabel = new QLabel(description);

    titleLabel->setTextFormat(Qt::RichText);
    descLabel->setTextFormat(Qt::RichText);

    infoLayout->addWidget(titleLabel);
    infoLayout->addWidget(descLabel);

    mainLayout->addWidget(amountLabel);
    mainLayout->addStretch();
    mainLayout->addWidget(timeLabel);
    mainLayout->addStretch();
    mainLayout->addLayout(infoLayout);

    card->setProperty("index", index);

    return card;
}

void Dashboard::updateMonthlyExpenseTotal()
{
    double totalExpense = App::getMonthExpenseTotal();
    QString formattedAmount = App::formatMoney(totalExpense);

    QLabel *expenseTotalLabel = this->findChild<QLabel*>("totalExpenseLabel");
    if (expenseTotalLabel) {
        expenseTotalLabel->setText(formattedAmount);

        updateExpenseChange();
        updateExpenseTrend();
    }
}

void Dashboard::updateMonthlyIncomeTotal()
{
    double totalIncome = App::getMonthIncomeTotal();
    QString formattedAmount = App::formatMoney(totalIncome);

    QLabel *incomeTotalLabel = this->findChild<QLabel*>("totalIncomeLabel");
    if (incomeTotalLabel) {
        incomeTotalLabel->setText(formattedAmount);

        updateIncomeChange();
        updateIncomeTrend();
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
        updateExpenseChange();
        updateExpenseTrend();
        updateIncomeChange();
        updateIncomeTrend();
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

double Dashboard::calculateMonthlyExpense(int year, int month) {
    double total = 0.0;

    for (const auto& expense : App::getExpenseList()) {
        QDate expenseDate = expense.getCreatedAt().date();
        if (expenseDate.year() == year && expenseDate.month() == month) {
            total += expense.getAmount();
        }
    }

    return total;
}

QVector<double> Dashboard::getLastThreeMonthsExpenses() {
    QVector<double> expenses;
    QDate currentDate = QDate::currentDate();

    for (int i = 2; i >= 0; i--) {
        QDate targetDate = currentDate.addMonths(-i);
        double monthlyExpense = calculateMonthlyExpense(targetDate.year(), targetDate.month());
        expenses.append(monthlyExpense);
    }

    return expenses;
}

void Dashboard::updateExpenseChange() {
    QDate currentDate = QDate::currentDate();
    QDate lastMonth = currentDate.addMonths(-1);

    double currentMonthExpense = calculateMonthlyExpense(currentDate.year(), currentDate.month());
    double lastMonthExpense = calculateMonthlyExpense(lastMonth.year(), lastMonth.month());

    double changePercent = 0.0;
    QString arrow = "→";
    QString color = "#666666";

    if (lastMonthExpense > 0) {
        changePercent = ((currentMonthExpense - lastMonthExpense) / lastMonthExpense) * 100.0;

        if (changePercent > 0) {
            arrow = "↗";
            color = "#aa0b20"; 
        } else if (changePercent < 0) {
            arrow = "↘";
            color = "#28a745"; 
            changePercent = abs(changePercent);
        }
    }

    QString changeText = QString("<html><head/><body><p><span style=\"font-size:18pt; color:%1;\">%2 %3%</span></p></body></html>")
                             .arg(color)
                             .arg(arrow)
                             .arg(QString::number(changePercent, 'f', 1));

    ui->label_4->setText(changeText);
}

void Dashboard::updateExpenseTrend() {
    QVector<double> lastThreeMonths = getLastThreeMonthsExpenses();

    if (lastThreeMonths.size() < 3) {
        ui->label_9->setText("<html><head/><body><p><span style=\"font-size:14pt; color:#666666;\">Chưa đủ dữ liệu</span></p></body></html>");
        return;
    }

    
    double firstMonth = lastThreeMonths[0];
    double lastMonth = lastThreeMonths[2];

    QString trendText;
    QString color;

    if (firstMonth == 0 && lastMonth == 0) {
        trendText = "Không có chi tiêu";
        color = "#666666";
    } else if (firstMonth == 0) {
        trendText = "Chi tiêu bắt đầu tăng";
        color = "#aa0b20";
    } else {
        double trendPercent = ((lastMonth - firstMonth) / firstMonth) * 100.0;

        if (trendPercent > 10) {
            trendText = "Chi tiêu đang tăng mạnh";
            color = "#dc3545"; 
        } else if (trendPercent > 0) {
            trendText = "Chi tiêu đang tăng";
            color = "#aa0b20"; 
        } else if (trendPercent < -10) {
            trendText = "Chi tiêu đang giảm mạnh";
            color = "#28a745"; 
        } else if (trendPercent < 0) {
            trendText = "Chi tiêu đang giảm";
            color = "#6c757d"; 
        } else {
            trendText = "Chi tiêu ổn định";
            color = "#17a2b8"; 
        }
    }

    QString html = QString("<html><head/><body><p><span style=\"font-size:14pt; color:%1;\">%2</span></p></body></html>")
                       .arg(color)
                       .arg(trendText);

    ui->label_9->setText(html);
}

double Dashboard::calculateMonthlyIncome(int year, int month) {
    double total = 0.0;

    for (const auto& income : App::getIncomeList()) {
        QDate incomeDate = income.getCreatedAt().date();
        if (incomeDate.year() == year && incomeDate.month() == month) {
            total += income.getAmount();
        }
    }

    return total;
}

QVector<double> Dashboard::getLastThreeMonthsIncomes() {
    QVector<double> incomes;
    QDate currentDate = QDate::currentDate();

    for (int i = 2; i >= 0; i--) {
        QDate targetDate = currentDate.addMonths(-i);
        double monthlyIncome = calculateMonthlyIncome(targetDate.year(), targetDate.month());
        incomes.append(monthlyIncome);
    }

    return incomes;
}

void Dashboard::updateIncomeChange() {
    QDate currentDate = QDate::currentDate();
    QDate lastMonth = currentDate.addMonths(-1);

    double currentMonthIncome = calculateMonthlyIncome(currentDate.year(), currentDate.month());
    double lastMonthIncome = calculateMonthlyIncome(lastMonth.year(), lastMonth.month());

    double changePercent = 0.0;
    QString arrow = "→";
    QString color = "#666666";

    if (lastMonthIncome > 0) {
        changePercent = ((currentMonthIncome - lastMonthIncome) / lastMonthIncome) * 100.0;

        if (changePercent > 0) {
            arrow = "↗";
            color = "#28a745"; 
        } else if (changePercent < 0) {
            arrow = "↘";
            color = "#aa0b20"; 
            changePercent = abs(changePercent);
        }
    }

    QString changeText = QString("<html><head/><body><p><span style=\"font-size:18pt; color:%1;\">%2 %3%</span></p></body></html>")
                             .arg(color)
                             .arg(arrow)
                             .arg(QString::number(changePercent, 'f', 1));

    ui->label_15->setText(changeText);
}

void Dashboard::updateIncomeTrend() {
    QVector<double> lastThreeMonths = getLastThreeMonthsIncomes();

    if (lastThreeMonths.size() < 3) {
        ui->label_18->setText("<html><head/><body><p><span style=\"font-size:14pt; color:#666666;\">Chưa đủ dữ liệu</span></p></body></html>");
        return;
    }

    
    double firstMonth = lastThreeMonths[0];
    double lastMonth = lastThreeMonths[2];

    QString trendText;
    QString color;

    if (firstMonth == 0 && lastMonth == 0) {
        trendText = "Không có thu nhập";
        color = "#666666";
    } else if (firstMonth == 0) {
        trendText = "Thu nhập bắt đầu tăng";
        color = "#28a745";
    } else {
        double trendPercent = ((lastMonth - firstMonth) / firstMonth) * 100.0;

        if (trendPercent > 10) {
            trendText = "Thu nhập đang tăng mạnh";
            color = "#28a745"; 
        } else if (trendPercent > 0) {
            trendText = "Thu nhập đang tăng";
            color = "#6c757d"; 
        } else if (trendPercent < -10) {
            trendText = "Thu nhập đang giảm mạnh";
            color = "#dc3545"; 
        } else if (trendPercent < 0) {
            trendText = "Thu nhập đang giảm";
            color = "#aa0b20"; 
        } else {
            trendText = "Thu nhập ổn định";
            color = "#17a2b8"; 
        }
    }

    QString html = QString("<html><head/><body><p><span style=\"font-size:14pt; color:%1;\">%2</span></p></body></html>")
                       .arg(color)
                       .arg(trendText);

    ui->label_18->setText(html);
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
            QString categoryName = categoryCombo->currentText();
            renderHtmlCard(categoryName, amount, QDateTime::currentDateTime(), description,
                           App::generateNextExpenseId(), "expenseList");
        } else {
            App::addIncome(categoryId, amount, description);
            QString categoryName = categoryCombo->currentText();
            renderHtmlCard(categoryName, amount, QDateTime::currentDateTime(), description,
                           App::generateNextIncomeId(), "incomeList");
        }

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

    
    for (const Expense& exp : filteredExpenses) {
        QString cat = exp.getCategory() ? exp.getCategory()->getName() : "Unknown Category";
        renderHtmlCard(cat, exp.getAmount(), exp.getCreatedAt(), exp.getDescription(), exp.getID(), "expenseList");
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

    
    for (const Income& inc : filteredIncomes) {
        QString cat = inc.getCategory() ? inc.getCategory()->getName() : "Unknown Category";
        renderHtmlCard(cat, inc.getAmount(), inc.getCreatedAt(), inc.getDescription(), inc.getID(), "incomeList");
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
    QFrame* card = createSavingGoalWidget(
        saving.getName(),
        saving.getTargetAmount(),
        saving.getCurrentAmount(),
        saving.getDescription(),
        saving.getID()
    );

    if (card && ui->savingListContentLayout) {
        ui->savingListContentLayout->insertWidget(ui->savingListContentLayout->count() - 1, card);
    }
}

QFrame* Dashboard::createSavingGoalWidget(const QString& goalName, double targetAmount, double currentAmount, const QString& description, const QString& goalId)
{
    QFrame* goalFrame = new QFrame();
    goalFrame->setObjectName(QString("savingGoal_%1").arg(goalId));
    goalFrame->setStyleSheet(
        "QFrame {"
        "    background-color: rgb(255, 255, 255);"
        "    border: 1px solid rgb(220, 220, 220);"
        "    border-radius: 8px;"
        "    padding: 15px;"
        "    margin: 5px;"
        "}"
        "QFrame:hover {"
        "    border: 2px solid rgb(34, 139, 34);"
        "}"
    );
    goalFrame->setFrameShape(QFrame::StyledPanel);
    goalFrame->setMinimumHeight(120);

    QVBoxLayout* mainLayout = new QVBoxLayout(goalFrame);

    
    QHBoxLayout* headerLayout = new QHBoxLayout();

    QLabel* titleLabel = new QLabel(goalName);
    titleLabel->setStyleSheet("font: 700 16pt 'Segoe UI'; color: rgb(41, 41, 41);");

    
    QHBoxLayout* actionsLayout = new QHBoxLayout();

    QPushButton* depositBtn = new QPushButton("Nạp");
    depositBtn->setObjectName(QString("deposit_%1").arg(goalId));
    depositBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: rgb(34, 139, 34);"
        "    color: white;"
        "    border: none;"
        "    padding: 5px 15px;"
        "    border-radius: 4px;"
        "    font: 600 10pt 'Segoe UI';"
        "}"
        "QPushButton:hover {"
        "    background-color: rgb(46, 125, 50);"
        "}"
    );

    QPushButton* withdrawBtn = new QPushButton("Rút");
    withdrawBtn->setObjectName(QString("withdraw_%1").arg(goalId));
    withdrawBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: rgb(255, 87, 34);"
        "    color: white;"
        "    border: none;"
        "    padding: 5px 15px;"
        "    border-radius: 4px;"
        "    font: 600 10pt 'Segoe UI';"
        "}"
        "QPushButton:hover {"
        "    background-color: rgb(244, 81, 30);"
        "}"
    );

    QPushButton* editBtn = new QPushButton("Sửa");
    editBtn->setObjectName(QString("edit_%1").arg(goalId));
    editBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: rgb(0, 123, 255);"
        "    color: white;"
        "    border: none;"
        "    padding: 5px 15px;"
        "    border-radius: 4px;"
        "    font: 600 10pt 'Segoe UI';"
        "}"
        "QPushButton:hover {"
        "    background-color: rgb(0, 86, 179);"
        "}"
    );

    QPushButton* deleteBtn = new QPushButton("Xóa");
    deleteBtn->setObjectName(QString("delete_%1").arg(goalId));
    deleteBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: rgb(220, 20, 60);"
        "    color: white;"
        "    border: none;"
        "    padding: 5px 15px;"
        "    border-radius: 4px;"
        "    font: 600 10pt 'Segoe UI';"
        "}"
        "QPushButton:hover {"
        "    background-color: rgb(190, 15, 50);"
        "}"
    );

    actionsLayout->addWidget(depositBtn);
    actionsLayout->addWidget(withdrawBtn);
    actionsLayout->addWidget(editBtn);
    actionsLayout->addWidget(deleteBtn);

    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addLayout(actionsLayout);

    
    QLabel* descLabel = new QLabel(description.isEmpty() ? "Không có mô tả" : description);
    descLabel->setStyleSheet("font: 11pt 'Segoe UI'; color: rgb(128, 128, 128); margin-top: 5px;");
    descLabel->setWordWrap(true);

    
    QHBoxLayout* amountLayout = new QHBoxLayout();

    QLabel* currentLabel = new QLabel(App::formatMoney(currentAmount));
    currentLabel->setStyleSheet("font: 700 14pt 'Segoe UI'; color: rgb(34, 139, 34);");

    QLabel* targetLabel = new QLabel("/ " + App::formatMoney(targetAmount));
    targetLabel->setStyleSheet("font: 12pt 'Segoe UI'; color: rgb(128, 128, 128);");

    double progress = calculateProgressPercentage(currentAmount, targetAmount);
    QLabel* progressLabel = new QLabel(QString("(%1%)").arg(QString::number(progress, 'f', 1)));
    progressLabel->setStyleSheet("font: 600 12pt 'Segoe UI'; color: rgb(255, 140, 0);");

    amountLayout->addWidget(currentLabel);
    amountLayout->addWidget(targetLabel);
    amountLayout->addWidget(progressLabel);
    amountLayout->addStretch();

    
    QProgressBar* progressBar = new QProgressBar();
    progressBar->setMaximum(100);
    progressBar->setValue(static_cast<int>(progress));
    progressBar->setStyleSheet(
        "QProgressBar {"
        "    border: 1px solid rgb(220, 220, 220);"
        "    border-radius: 4px;"
        "    text-align: center;"
        "    height: 20px;"
        "}"
        "QProgressBar::chunk {"
        "    background-color: rgb(34, 139, 34);"
        "    border-radius: 3px;"
        "}"
    );

    
    mainLayout->addLayout(headerLayout);
    mainLayout->addWidget(descLabel);
    mainLayout->addLayout(amountLayout);
    mainLayout->addWidget(progressBar);

    
    connect(depositBtn, &QPushButton::clicked, [this, goalId]() {
        showSavingTransactionDialog("deposit", goalId);
    });

    connect(withdrawBtn, &QPushButton::clicked, [this, goalId]() {
        showSavingTransactionDialog("withdraw", goalId);
    });

    connect(editBtn, &QPushButton::clicked, [this, goalId]() {
        showEditSavingGoalDialog(goalId);
    });

    connect(deleteBtn, &QPushButton::clicked, [this, goalId]() {
        deleteSavingGoal(goalId);
    });

    return goalFrame;
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

double Dashboard::calculateProgressPercentage(double current, double target)
{
    if (target <= 0) return 0.0;
    double progress = (current / target) * 100.0;
    return qMin(progress, 100.0); 
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

            double progress = calculateProgressPercentage(saving.getCurrentAmount(), saving.getTargetAmount());
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
                 [this](const Saving& a, const Saving& b) {
                     double progressA = calculateProgressPercentage(a.getCurrentAmount(), a.getTargetAmount());
                     double progressB = calculateProgressPercentage(b.getCurrentAmount(), b.getTargetAmount());
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

#include "dashboard.moc"



