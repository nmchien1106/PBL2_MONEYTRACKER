#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QDate>
#include <QString>
#include <QFrame>
#include "category.h"

class Transaction
{
private:
    QString id;
    Category* category;
    QDateTime createdAt;
    QDate updatedAt;
    double amount;
    QString description;
public:

    Transaction(const QString& id = "", Category* category= nullptr, const QDateTime& createdAt= QDateTime::currentDateTime(),
        const QDate& updatedAt= QDate::currentDate(),
        double amount= 0.0, const QString& description= "");

    Transaction(const QStringList& data);

    virtual ~Transaction();

    QString getID() const {return this->id;}
    Category* getCategory() const {return this->category;}
    QDateTime getCreatedAt() const {return this->createdAt;}
    QDate getUpdatedAt() const {return this->updatedAt;}
    double getAmount() const {return this->amount;}
    QString getDescription() const {return this->description;}

    void setCreatedAt(QDateTime createdAt){
        this->createdAt = createdAt;
    }

    void setUpdatedAt(QDate updatedAt){
        this->updatedAt = updatedAt;
    }

    void setID(QString id){
        this->id = id;
    }

    void setCategory(Category* category){
        this->category = category;
    }

    void setAmount(double amount){
        this->amount = amount;
    }

    void setDescription(QString description){
        this->description = description;
    }

    // Abstract methods to be implemented by derived classes
    virtual QString getType() const = 0;
    virtual QString toString() const = 0;
    virtual QFrame* createCard(const QString& index) const = 0;
    virtual QString getCardHtml() const = 0;
    virtual QString getColorCode() const = 0;
};

#endif
