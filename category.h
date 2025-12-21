#ifndef CATEGORY_H
#define CATEGORY_H

#include <QString>
#include <QStringList>
#include <QDate>

class Category
{
private:
    QString id;
    QDate createdAt;
    QString name;
    QString description;
    QString type;
public:

    Category(const QString& id = "", const QDate& createdAt= QDate::currentDate(),
             const QString& name= "", const QString& description= "", const QString& type= "");
    Category(const QStringList&);

    ~Category();

    QString getID() const {return this->id;}
    QDate getCreatedAt() const {return this->createdAt;}
    QString getName() const {return this->name;}
    QString getDescription() const {return this->description;}
    QString getType() const {return this->type;}

    void setName(QString name){
        this->name = name;
    }

    void setDescription(QString description){
        this->description = description;
    }

    void setType(QString type){
        this->type = type;
    }


    QString toString() const {
        return QString("Category[ID: %1, CreatedAt: %2, Name: %3, Description: %4, Type: %5]")
            .arg(id)
            .arg(createdAt.toString("dd/MM/yyyy"))
            .arg(name)
            .arg(description)
            .arg(type);
    }

    static QStringList getHeader(){
        return {"id", "createdAt", "name", "description", "type"};
    }

    QStringList toCsvRow() const {
        return {id, createdAt.toString("dd/MM/yyyy"), name, description, type};
    }
};

inline QDebug operator<<(QDebug dbg, const Category &u)
{
    dbg.nospace() << u.toString();
    return dbg.space();
}

inline QDebug operator<<(QDebug dbg, const Category* u)
{
    if (u)
        dbg.nospace() << u->toString();
    else
        dbg.nospace() << "nullptr";
    return dbg.space();
}


#endif 
