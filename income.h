#ifndef INCOME_H
#define INCOME_H

#include "transaction.h"

class Income : public Transaction
{
public:
    Income(const QString& id = "", Category* category= nullptr,
           const QDateTime& createdAt= QDateTime::currentDateTime(), const QDate& updatedAt= QDate::currentDate(),
           double amount= 0.0, const QString& description= "")
        : Transaction(id, category, createdAt, updatedAt, amount, description)
    {
    }

    Income(const QStringList& data)
        : Transaction(data)
    {
        qDebug() << "Income: " << data;
    }

    QString getType() const override {
        return "Income";
    }

    QString toString() const override {
        QString categoryName = getCategory() ? getCategory()->getName() : "No Category";
        return QString("Income[ID: %1, Category: %2, CreatedAt: %3, UpdatedAt: %4, Amount: %5, Description: %6]")
            .arg(getID())
            .arg(categoryName)
            .arg(getCreatedAt().toString("dd/MM/yyyy"))
            .arg(getUpdatedAt().toString("dd/MM/yyyy"))
            .arg(getAmount())
            .arg(getDescription());
    }

    static QStringList getHeader() {
        return {"id",  "category", "createdAt", "updatedAt", "amount", "description"};
    }

    QStringList toCsvRow() const {
        QString categoryName = getCategory() ? getCategory()->getID() : "";
        return {getID(), categoryName, getCreatedAt().toString("dd/MM/yyyy HH:mm"),
                getUpdatedAt().toString("dd/MM/yyyy"), QString::number(getAmount()), getDescription()};
    }
};

inline QDebug operator<<(QDebug dbg, const Income &u)
{
    dbg.nospace()  << u.toString();
    return dbg.space();
}

#endif 
