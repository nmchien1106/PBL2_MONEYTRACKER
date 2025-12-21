#include "transaction.h"
#include "app.h"

Transaction::Transaction(const QString& id, Category* category,
                        const QDateTime& createdAt, const QDate& updatedAt,
                        double amount, const QString& description)
    : id(id), category(category), createdAt(createdAt),
      updatedAt(updatedAt), amount(amount), description(description)
{
}

Transaction::Transaction(const QStringList& data)
{
    if(data.size() != 6){
        this->id = "";
        this->category = nullptr;
        this->createdAt = QDateTime::currentDateTime();
        this->updatedAt = QDate::currentDate();
        this->amount = 0.0;
        this->description = "";
        return;
    }

    this->id = data[0];
    this->category = App::findCategoryByID(data[1].trimmed());
    this->createdAt = QDateTime::fromString(data[2], "dd/MM/yyyy HH:mm");
    this->updatedAt = QDate::fromString(data[3], "dd/MM/yyyy");
    this->amount = data[4].toDouble();
    this->description = data[5];
}

Transaction::~Transaction()
{
}
