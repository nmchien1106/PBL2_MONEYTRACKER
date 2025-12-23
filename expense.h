#ifndef EXPENSE_H
#define EXPENSE_H

#include "transaction.h"
#include <QString>
#include <QDate>
#include <QFrame>
#include <QVector>

class Expense : public Transaction {
private:

public:
    Expense(const QString& id = "", Category* category= nullptr,
           const QDateTime& createdAt= QDateTime::currentDateTime(), const QDate& updatedAt= QDate::currentDate(),
           double amount= 0.0, const QString& description= "");

    Expense(const QStringList& data);

    QString getType() const override;
    QString toString() const override;
    QString getColorCode() const override;
    QFrame* createCard(const QString& index) const override;
    QString getCardHtml() const override;

    static QStringList getHeader();
    QStringList toCsvRow() const;

    // Static calculation methods
    static double calculateMonthlyTotal(const QVector<Expense>& expenses, int year, int month);
    static QVector<double> getLastThreeMonthsTotals(const QVector<Expense>& expenses);
};

QDebug operator<<(QDebug dbg, const Expense &u);

#endif
