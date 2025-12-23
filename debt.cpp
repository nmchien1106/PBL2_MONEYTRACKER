#include "debt.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

Debt::Debt(const QString& id, Category* category,
           const QDateTime& createdAt, const QDate& updatedAt,
           double amount, const QString& description,
           const QString& debtorName, const QDate& dueDate,
           bool isPaid, const QString& debtType)
    : Transaction(id, category, createdAt, updatedAt, amount, description),
      debtorName(debtorName), dueDate(dueDate), isPaid(isPaid), debtType(debtType)
{
}

Debt::Debt(const QStringList& data)
    : Transaction(data)
{
    if (data.size() >= 10) {
        debtorName = data[6];
        dueDate = QDate::fromString(data[7], "dd/MM/yyyy");
        isPaid = (data[8] == "1" || data[8].toLower() == "true");
        debtType = data[9];
    }
    qDebug() << "Debt: " << data;
}

QString Debt::getType() const {
    return "Debt";
}

QString Debt::toString() const {
    QString categoryName = getCategory() ? getCategory()->getName() : "No Category";
    return QString("Debt[ID: %1, Category: %2, CreatedAt: %3, UpdatedAt: %4, Amount: %5, Description: %6, DebtorName: %7, DueDate: %8, IsPaid: %9, DebtType: %10]")
        .arg(getID())
        .arg(categoryName)
        .arg(getCreatedAt().toString("dd/MM/yyyy"))
        .arg(getUpdatedAt().toString("dd/MM/yyyy"))
        .arg(getAmount())
        .arg(getDescription())
        .arg(debtorName)
        .arg(dueDate.toString("dd/MM/yyyy"))
        .arg(isPaid ? "Paid" : "Unpaid")
        .arg(debtType);
}

QString Debt::getColorCode() const {
    if (debtType == "lend") {
        return "#28a745"; // Green for lending (positive)
    } else {
        return "#ffc107"; // Yellow/Orange for borrowing (caution)
    }
}

QFrame* Debt::createCard(const QString& index) const {
    QFrame* card = new QFrame();
    card->setObjectName("debtCard_" + index);
    card->setStyleSheet(
        "QFrame#debtCard_" + index + " {"
        "background-color: white;"
        "border-radius: 8px;"
        "padding: 15px;"
        "margin: 5px;"
        "border: 1px solid #e0e0e0;"
        "}"
        "QFrame#debtCard_" + index + ":hover {"
        "border: 2px solid " + getColorCode() + ";"
        "}"
    );

    QVBoxLayout* mainLayout = new QVBoxLayout(card);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // Top row: Amount and Type badge
    QHBoxLayout* topRow = new QHBoxLayout();
    QString prefix = (debtType == "lend") ? "+" : "-";
    QLabel* amountLabel = new QLabel(QString("%1%2 VNĐ").arg(prefix).arg(getAmount(), 0, 'f', 0));
    amountLabel->setStyleSheet("color: " + getColorCode() + "; font-weight: bold; font-size: 18px;");

    QLabel* typeLabel = new QLabel(debtType == "lend" ? "CHO VAY" : "VAY NỢ");
    typeLabel->setStyleSheet(
        "background-color: " + getColorCode() + ";"
        "color: white;"
        "padding: 3px 8px;"
        "border-radius: 3px;"
        "font-size: 10px;"
        "font-weight: bold;"
    );
    topRow->addWidget(amountLabel);
    topRow->addStretch();
    topRow->addWidget(typeLabel);

    // Debtor name row
    QHBoxLayout* debtorRow = new QHBoxLayout();
    QLabel* debtorLabel = new QLabel((debtType == "lend" ? "Người vay: " : "Người cho vay: ") + debtorName);
    debtorLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #333;");
    debtorRow->addWidget(debtorLabel);

    // Due date and status row
    QHBoxLayout* dateRow = new QHBoxLayout();
    QLabel* dueDateLabel = new QLabel("Đáo hạn: " + dueDate.toString("dd/MM/yyyy"));
    dueDateLabel->setStyleSheet("color: #666; font-size: 12px;");

    QLabel* statusLabel = new QLabel(isPaid ? "✓ Đã thanh toán" : "⏳ Chưa thanh toán");
    statusLabel->setStyleSheet(
        QString("color: %1; font-size: 12px; font-weight: bold;")
        .arg(isPaid ? "#28a745" : "#dc3545")
    );
    dateRow->addWidget(dueDateLabel);
    dateRow->addStretch();
    dateRow->addWidget(statusLabel);

    // Description
    QLabel* descLabel = new QLabel(getDescription());
    descLabel->setStyleSheet("color: #666; font-size: 12px;");
    descLabel->setWordWrap(true);

    // Action buttons
    QHBoxLayout* actionRow = new QHBoxLayout();
    QPushButton* markPaidBtn = new QPushButton(isPaid ? "Đánh dấu chưa trả" : "Đánh dấu đã trả");
    QPushButton* editBtn = new QPushButton("Sửa");
    QPushButton* deleteBtn = new QPushButton("Xóa");

    markPaidBtn->setObjectName("markPaid_" + getID());
    editBtn->setObjectName("edit_" + getID());
    deleteBtn->setObjectName("delete_" + getID());

    markPaidBtn->setStyleSheet("background-color: #28a745; color: white; border: none; padding: 5px 10px; border-radius: 3px;");
    editBtn->setStyleSheet("background-color: #007bff; color: white; border: none; padding: 5px 10px; border-radius: 3px;");
    deleteBtn->setStyleSheet("background-color: #dc3545; color: white; border: none; padding: 5px 10px; border-radius: 3px;");

    actionRow->addWidget(markPaidBtn);
    actionRow->addStretch();
    actionRow->addWidget(editBtn);
    actionRow->addWidget(deleteBtn);

    mainLayout->addLayout(topRow);
    mainLayout->addLayout(debtorRow);
    mainLayout->addLayout(dateRow);
    mainLayout->addWidget(descLabel);
    mainLayout->addLayout(actionRow);

    return card;
}

QString Debt::getCardHtml() const {
    QString prefix = (debtType == "lend") ? "+" : "-";
    QString typeText = (debtType == "lend") ? "CHO VAY" : "VAY NỢ";
    QString debtorText = (debtType == "lend") ? "Người vay: " : "Người cho vay: ";
    QString statusText = isPaid ? "✓ Đã thanh toán" : "⏳ Chưa thanh toán";
    QString statusColor = isPaid ? "#28a745" : "#dc3545";

    return QString(
        "<div style='background: white; border-radius: 8px; padding: 15px; margin: 10px; border: 1px solid #e0e0e0;'>"
        "<div style='display: flex; justify-content: space-between; margin-bottom: 10px;'>"
        "<span style='color: %1; font-weight: bold; font-size: 18px;'>%2%3 VNĐ</span>"
        "<span style='background-color: %1; color: white; padding: 3px 8px; border-radius: 3px; font-size: 10px; font-weight: bold;'>%4</span>"
        "</div>"
        "<div style='font-weight: bold; font-size: 14px; color: #333; margin-bottom: 5px;'>%5%6</div>"
        "<div style='display: flex; justify-content: space-between; margin-bottom: 5px;'>"
        "<span style='color: #666; font-size: 12px;'>Đáo hạn: %7</span>"
        "<span style='color: %8; font-size: 12px; font-weight: bold;'>%9</span>"
        "</div>"
        "<div style='color: #666; font-size: 12px;'>%10</div>"
        "</div>"
    ).arg(getColorCode())
     .arg(prefix)
     .arg(getAmount(), 0, 'f', 0)
     .arg(typeText)
     .arg(debtorText)
     .arg(debtorName)
     .arg(dueDate.toString("dd/MM/yyyy"))
     .arg(statusColor)
     .arg(statusText)
     .arg(getDescription());
}

QStringList Debt::getHeader() {
    return {"id", "category", "createdAt", "updatedAt", "amount", "description", "debtorName", "dueDate", "isPaid", "debtType"};
}

QStringList Debt::toCsvRow() const {
    QString categoryName = getCategory() ? getCategory()->getID() : "";
    return {getID(), categoryName, getCreatedAt().toString("dd/MM/yyyy HH:mm"),
            getUpdatedAt().toString("dd/MM/yyyy"), QString::number(getAmount()),
            getDescription(), debtorName, dueDate.toString("dd/MM/yyyy"),
            isPaid ? "1" : "0", debtType};
}

// Static calculation methods
double Debt::calculateMonthlyTotal(const QVector<Debt>& debts, int year, int month, const QString& type) {
    double total = 0.0;
    for (const Debt& debt : debts) {
        QDate debtDate = debt.getCreatedAt().date();
        if (debtDate.year() == year && debtDate.month() == month) {
            if (type.isEmpty() || debt.getDebtType() == type) {
                total += debt.getAmount();
            }
        }
    }
    return total;
}

QVector<double> Debt::getLastThreeMonthsTotals(const QVector<Debt>& debts, const QString& type) {
    QVector<double> totals;
    QDate currentDate = QDate::currentDate();

    for (int i = 2; i >= 0; --i) {
        QDate targetDate = currentDate.addMonths(-i);
        totals.append(calculateMonthlyTotal(debts, targetDate.year(), targetDate.month(), type));
    }
    return totals;
}

double Debt::getTotalBorrowed(const QVector<Debt>& debts) {
    double total = 0.0;
    for (const Debt& debt : debts) {
        if (debt.getDebtType() == "borrow") {
            total += debt.getAmount();
        }
    }
    return total;
}

double Debt::getTotalLent(const QVector<Debt>& debts) {
    double total = 0.0;
    for (const Debt& debt : debts) {
        if (debt.getDebtType() == "lend") {
            total += debt.getAmount();
        }
    }
    return total;
}

double Debt::getTotalUnpaid(const QVector<Debt>& debts, const QString& type) {
    double total = 0.0;
    for (const Debt& debt : debts) {
        if (!debt.getIsPaid()) {
            if (type.isEmpty() || debt.getDebtType() == type) {
                total += debt.getAmount();
            }
        }
    }
    return total;
}

QDebug operator<<(QDebug dbg, const Debt& d)
{
    dbg.nospace() << d.toString();
    return dbg.space();
}
