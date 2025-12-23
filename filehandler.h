#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QStringList>
#include <QFile>

#include "category.h"
#include "income.h"
#include "expense.h"
#include "saving.h"
#include "debt.h"

class FileHandler : public QObject
{
    Q_OBJECT
public:
    explicit FileHandler(QObject *parent = nullptr);

    QStringList readRawLines(const QString &filePath);

    template<typename T>
    QVector<T> readList(const QString &filePath);

    bool writeRawLines(const QString &filePath, const QStringList &lines);

    template<typename T>
    bool writeList(const QString &filePath, const QVector<T> &list);

private:
    QStringList splitCsvLine(const QString &line);
};


#endif
