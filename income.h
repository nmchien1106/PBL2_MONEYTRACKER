#ifndef INCOME_H
#define INCOME_H

#include "transaction.h"
#include <QString>
#include <QDate>
#include <QFrame>
#include <QVector>

class Income : public Transaction
{
public:
    Income(const QString& id = "", Category* category= nullptr,
           const QDateTime& createdAt= QDateTime::currentDateTime(), const QDate& updatedAt= QDate::currentDate(),
           double amount= 0.0, const QString& description= "");

    Income(const QStringList& data);

    QString getType() const override;
    QString toString() const override;
    QString getColorCode() const override;
    QFrame* createCard(const QString& index) const override;
    QString getCardHtml() const override;

    static QStringList getHeader();
    QStringList toCsvRow() const;

    // Static calculation methods
    static double calculateMonthlyTotal(const QVector<Income>& incomes, int year, int month);
    static QVector<double> getLastThreeMonthsTotals(const QVector<Income>& incomes);
};

QDebug operator<<(QDebug dbg, const Income &u);

#endif
