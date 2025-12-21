#include "category.h"

Category::Category(const QString &id, const QDate &createdAt, const QString &name, const QString &description, const QString &type)
    : id(id), createdAt(createdAt), name(name), description(description), type(type)
{
}

Category::Category(const QStringList &data)
{
    qDebug() << "Category: " << data;
    if (data.size() >= 5 && !data[0].trimmed().isEmpty()) {
        this->id = data[0].trimmed();
        this->createdAt = QDate::fromString(data[1].trimmed(), "dd/MM/yyyy");
        this->name = data[2].trimmed();
        this->description = data[3].trimmed();
        this->type = data[4].trimmed();
    } else {
        this->id = "";
        this->createdAt = QDate::currentDate();
        this->name = "";
        this->description = "";
        this->type = "";
    }
}

Category::~Category()
{
}
