#include "expense.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

Expense::Expense(const QString& id, Category* category,
       const QDateTime& createdAt, const QDate& updatedAt,
       double amount, const QString& description)
    : Transaction(id, category, createdAt, updatedAt, amount, description)
{
}

Expense::Expense(const QStringList& data)
    : Transaction(data)
{
    qDebug() << "Expense: " << data;
}

QString Expense::getType() const {
    return "Expense";
}

QString Expense::toString() const {
    QString categoryName = getCategory() ? getCategory()->getName() : "No Category";
    return QString("Expense[ID: %1, Category: %2, CreatedAt: %3, UpdatedAt: %4, Amount: %5, Description: %6]")
        .arg(getID())
        .arg(categoryName)
        .arg(getCreatedAt().toString("dd/MM/yyyy"))
        .arg(getUpdatedAt().toString("dd/MM/yyyy"))
        .arg(getAmount())
        .arg(getDescription());
}

QString Expense::getColorCode() const {
    return "#dc3545"; // Red for expense
}

QFrame* Expense::createCard(const QString& index) const {
    QFrame* card = new QFrame();
    card->setObjectName("expenseCard_" + index);
    card->setStyleSheet(
        "QFrame#expenseCard_" + index + " {"
        "background-color: white;"
        "border-radius: 8px;"
        "padding: 15px;"
        "margin: 5px;"
        "border: 1px solid #e0e0e0;"
        "}"
        "QFrame#expenseCard_" + index + ":hover {"
        "border: 2px solid " + getColorCode() + ";"
        "}"
    );

    QVBoxLayout* mainLayout = new QVBoxLayout(card);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // Amount and date row
    QHBoxLayout* topRow = new QHBoxLayout();
    QLabel* amountLabel = new QLabel(QString("-%1 VNĐ").arg(getAmount(), 0, 'f', 0));
    amountLabel->setStyleSheet("color: " + getColorCode() + "; font-weight: bold; font-size: 18px;");
    QLabel* dateLabel = new QLabel(getCreatedAt().toString("dd/MM/yyyy HH:mm"));
    dateLabel->setStyleSheet("color: #666; font-size: 12px;");
    topRow->addWidget(amountLabel);
    topRow->addStretch();
    topRow->addWidget(dateLabel);

    // Category
    QLabel* categoryLabel = new QLabel(getCategory() ? getCategory()->getName() : "No Category");
    categoryLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #333;");

    // Description
    QLabel* descLabel = new QLabel(getDescription());
    descLabel->setStyleSheet("color: #666; font-size: 12px;");
    descLabel->setWordWrap(true);

    // Action buttons
    QHBoxLayout* actionRow = new QHBoxLayout();
    QPushButton* editBtn = new QPushButton("Sửa");
    QPushButton* deleteBtn = new QPushButton("Xóa");
    editBtn->setObjectName("edit_" + getID());
    deleteBtn->setObjectName("delete_" + getID());
    editBtn->setStyleSheet("background-color: #007bff; color: white; border: none; padding: 5px 10px; border-radius: 3px;");
    deleteBtn->setStyleSheet("background-color: #dc3545; color: white; border: none; padding: 5px 10px; border-radius: 3px;");
    actionRow->addStretch();
    actionRow->addWidget(editBtn);
    actionRow->addWidget(deleteBtn);

    mainLayout->addLayout(topRow);
    mainLayout->addWidget(categoryLabel);
    mainLayout->addWidget(descLabel);
    mainLayout->addLayout(actionRow);

    return card;
}

QString Expense::getCardHtml() const {
    QString categoryName = getCategory() ? getCategory()->getName() : "No Category";
    return QString(
        "<div style='background: white; border-radius: 8px; padding: 15px; margin: 10px; border: 1px solid #e0e0e0;'>"
        "<div style='display: flex; justify-content: space-between; margin-bottom: 10px;'>"
        "<span style='color: %1; font-weight: bold; font-size: 18px;'>-%2 VNĐ</span>"
        "<span style='color: #666; font-size: 12px;'>%3</span>"
        "</div>"
        "<div style='font-weight: bold; font-size: 14px; color: #333; margin-bottom: 5px;'>%4</div>"
        "<div style='color: #666; font-size: 12px;'>%5</div>"
        "</div>"
    ).arg(getColorCode())
     .arg(getAmount(), 0, 'f', 0)
     .arg(getCreatedAt().toString("dd/MM/yyyy HH:mm"))
     .arg(categoryName)
     .arg(getDescription());
}

QStringList Expense::getHeader() {
    return {"id", "category", "createdAt", "updatedAt", "amount", "description"};
}

QStringList Expense::toCsvRow() const {
    QString categoryName = getCategory() ? getCategory()->getID() : "";
    return {getID(), categoryName, getCreatedAt().toString("dd/MM/yyyy HH:mm"),
            getUpdatedAt().toString("dd/MM/yyyy"), QString::number(getAmount()), getDescription()};
}

// Static calculation methods
double Expense::calculateMonthlyTotal(const QVector<Expense>& expenses, int year, int month) {
    double total = 0.0;
    for (const Expense& expense : expenses) {
        QDate expenseDate = expense.getCreatedAt().date();
        if (expenseDate.year() == year && expenseDate.month() == month) {
            total += expense.getAmount();
        }
    }
    return total;
}

QVector<double> Expense::getLastThreeMonthsTotals(const QVector<Expense>& expenses) {
    QVector<double> totals;
    QDate currentDate = QDate::currentDate();

    for (int i = 2; i >= 0; --i) {
        QDate targetDate = currentDate.addMonths(-i);
        totals.append(calculateMonthlyTotal(expenses, targetDate.year(), targetDate.month()));
    }
    return totals;
}

QDebug operator<<(QDebug dbg, const Expense &u)
{
    dbg.nospace()  << u.toString();
    return dbg.space();
}
