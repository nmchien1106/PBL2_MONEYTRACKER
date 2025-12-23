#ifndef DEBT_H
#define DEBT_H

#include "transaction.h"
#include <QString>
#include <QDate>
#include <QFrame>
#include <QVector>

class Debt : public Transaction {
private:
    QString debtorName;      // Tên người nợ/cho vay
    QDate dueDate;          // Ngày đáo hạn
    bool isPaid;            // Trạng thái thanh toán
    QString debtType;       // "borrow" (vay) hoặc "lend" (cho vay)

public:
    Debt(const QString& id = "", Category* category = nullptr,
         const QDateTime& createdAt = QDateTime::currentDateTime(),
         const QDate& updatedAt = QDate::currentDate(),
         double amount = 0.0, const QString& description = "",
         const QString& debtorName = "", const QDate& dueDate = QDate::currentDate(),
         bool isPaid = false, const QString& debtType = "borrow");

    Debt(const QStringList& data);

    // Getters
    QString getDebtorName() const { return debtorName; }
    QDate getDueDate() const { return dueDate; }
    bool getIsPaid() const { return isPaid; }
    QString getDebtType() const { return debtType; }

    // Setters
    void setDebtorName(const QString& name) { debtorName = name; }
    void setDueDate(const QDate& date) { dueDate = date; }
    void setIsPaid(bool paid) { isPaid = paid; }
    void setDebtType(const QString& type) { debtType = type; }

    // Override methods from Transaction
    QString getType() const override;
    QString toString() const override;
    QString getColorCode() const override;
    QFrame* createCard(const QString& index) const override;
    QString getCardHtml() const override;

    // CSV operations
    static QStringList getHeader();
    QStringList toCsvRow() const;

    // Static calculation methods
    static double calculateMonthlyTotal(const QVector<Debt>& debts, int year, int month, const QString& type = "");
    static QVector<double> getLastThreeMonthsTotals(const QVector<Debt>& debts, const QString& type = "");
    static double getTotalBorrowed(const QVector<Debt>& debts);
    static double getTotalLent(const QVector<Debt>& debts);
    static double getTotalUnpaid(const QVector<Debt>& debts, const QString& type = "");
};

QDebug operator<<(QDebug dbg, const Debt& d);

#endif
