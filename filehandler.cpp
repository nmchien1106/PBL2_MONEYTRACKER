#include "filehandler.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

FileHandler::FileHandler(QObject *parent)
    : QObject(parent){}

QStringList FileHandler::readRawLines(const QString &filePath)
{
    QFile file(filePath);
    QStringList lines;

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Không thể mở file:" << filePath;
        return lines;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        lines.append(in.readLine());
    }
    return lines;
}

QStringList FileHandler::splitCsvLine(const QString &line)
{
    return line.split(",");
}

template<typename T>
QVector<T> FileHandler::readList(const QString &filePath)
{
    QVector<T> list;
    QStringList lines = readRawLines(filePath);

    for (int i = 1; i < lines.size(); i++) {
        QStringList cols = splitCsvLine(lines[i]);
        if (cols.isEmpty()) continue;

        list.append(T(cols));
    }

    return list;
}

bool FileHandler::writeRawLines(const QString &filePath, const QStringList &lines)
{
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qWarning() << "Không thể mở file để ghi:" << filePath;
        return false;
    }

    QTextStream out(&file);
    for (const QString &line : lines) {
        out << line << "\n";
    }

    return true;
}

template<typename T>
bool FileHandler::writeList(const QString &filePath, const QVector<T> &list)
{
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qWarning() << "Không thể mở file để ghi:" << filePath;
        return false;
    }



    QTextStream out(&file);
    out << T::getHeader().join(",") << "\n";
    for (const T &item : list) {
        out << item.toCsvRow().join(",") << "\n";
    }

    return true;
}
template QList<Category> FileHandler::readList(const QString &filePath);
template bool FileHandler::writeList(const QString& filePath, const QList<Category> &list);
template QList<Income> FileHandler::readList(const QString &filePath);
template bool FileHandler::writeList(const QString& filePath, const QList<Income> &list);
template QList<Saving> FileHandler::readList(const QString &filePath);
template bool FileHandler::writeList(const QString& filePath, const QList<Saving> &list);
template QList<Expense> FileHandler::readList(const QString &filePath);
template bool FileHandler::writeList(const QString& filePath, const QList<Expense> &list);
template QList<Debt> FileHandler::readList(const QString &filePath);
template bool FileHandler::writeList(const QString& filePath, const QList<Debt> &list);
