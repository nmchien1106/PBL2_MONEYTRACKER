#ifndef SAVING_H
#define SAVING_H

#include "transaction.h"
#include <QString>
#include <QDate>

class Saving : public Transaction
{
private:
    QDate endDate;
    double amountTarget;
    QString name;

public:
    Saving(
        const QString &id = "", const QDateTime &createdAt = QDateTime::currentDateTime(),
        const QDate &updatedAt = QDate::currentDate(), double amount = 0.0, const QString &description = "",
        const QString &name = "", const QDate &endDate = QDate::currentDate(), const double &amountTarget = 0.0)
        : Transaction(id, nullptr, createdAt, updatedAt, amount, description)
    {
        this->amountTarget = amountTarget;
        this->endDate = endDate;
        this->name = name;
    }

    Saving(const QStringList &data)
    {
        qDebug() << "Saving: " << data;
        if (data.size() >= 9)
        {
            setID(data[0]);
            setCategory(nullptr);
            setCreatedAt(QDateTime::fromString(data[2], "dd/MM/yyyy HH:mm"));
            setUpdatedAt(QDate::fromString(data[3], "dd/MM/yyyy"));
            setAmount(data[4].toDouble());
            setDescription(data[5]);
            this->name = data[6];
            this->endDate = QDate::fromString(data[7], "dd/MM/yyyy");
            this->amountTarget = data[8].toDouble();
        }
    }

    QString getType() const override
    {
        return "Saving";
    }

    QDate getEndDate() const
    {
        return this->endDate;
    }

    double getAmountTarget() const
    {
        return this->amountTarget;
    }

    QString getName() const
    {
        return this->name;
    }

    void setName(const QString& newName)
    {
        this->name = newName;
    }

    void setAmountTarget(double target)
    {
        this->amountTarget = target;
    }

    void setEndDate(const QDate& date)
    {
        this->endDate = date;
    }

    double getCurrentAmount() const
    {
        return getAmount();
    }

    void setCurrentAmount(double amount)
    {
        setAmount(amount);
    }

    double getTargetAmount() const
    {
        return getAmountTarget();
    }

    void setTargetAmount(double target)
    {
        setAmountTarget(target);
    }

    QString toString() const override
    {
        QString categoryName = getCategory() ? getCategory()->getName() : "No Category";
        return QString("Saving[ID: %1, CreatedAt: %2, UpdatedAt: %3, Amount: %4, Description: %5, Name: %6, EndDate: %7, AmountTarget: %8]")
            .arg(getID())
            .arg(getCreatedAt().toString("dd/MM/yyyy HH:mm"))
            .arg(getUpdatedAt().toString("dd/MM/yyyy"))
            .arg(getAmount())
            .arg(getDescription())
            .arg(name)
            .arg(endDate.toString("dd/MM/yyyy"))
            .arg(amountTarget);
    }

    static QStringList getHeader()
    {
        return {"id", "category", "createdAt", "updatedAt", "amount", "description", "name", "endDate", "amountTarget"};
    }

    QStringList toCsvRow() const
    {
        return {
            getID(), "", getCreatedAt().toString("dd/MM/yyyy HH:mm"),
            getUpdatedAt().toString("dd/MM/yyyy"), QString::number(getAmount()), getDescription(), name, endDate.toString("dd/MM/yyyy"), QString::number(amountTarget)};
    }
};

inline QDebug operator<<(QDebug dbg, const Saving &u)
{
    dbg.nospace() << u.toString();
    return dbg.space();
}

#endif 
