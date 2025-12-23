#ifndef SAVING_H
#define SAVING_H

#include "transaction.h"
#include <QString>
#include <QDate>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>

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

    QString getColorCode() const override {
        return "#17a2b8"; // Blue for savings
    }

    double calculateProgress() const {
        if (amountTarget <= 0) return 0;
        return (getAmount() / amountTarget) * 100.0;
    }

    QFrame* createCard(const QString& index) const override {
        QFrame* card = new QFrame();
        card->setObjectName("savingCard_" + index);
        card->setStyleSheet(
            "QFrame#savingCard_" + index + " {"
            "background-color: white;"
            "border-radius: 8px;"
            "padding: 15px;"
            "margin: 5px;"
            "border: 1px solid #e0e0e0;"
            "}"
            "QFrame#savingCard_" + index + ":hover {"
            "border: 2px solid " + getColorCode() + ";"
            "}"
        );

        QVBoxLayout* mainLayout = new QVBoxLayout(card);
        mainLayout->setContentsMargins(10, 10, 10, 10);

        // Goal name
        QLabel* nameLabel = new QLabel(name);
        nameLabel->setStyleSheet("font-weight: bold; font-size: 16px; color: #333;");

        // Amount info
        QHBoxLayout* amountRow = new QHBoxLayout();
        QLabel* currentLabel = new QLabel(QString("%1 VNĐ").arg(getAmount(), 0, 'f', 0));
        currentLabel->setStyleSheet("color: " + getColorCode() + "; font-weight: bold; font-size: 14px;");
        QLabel* targetLabel = new QLabel(QString("/ %1 VNĐ").arg(amountTarget, 0, 'f', 0));
        targetLabel->setStyleSheet("color: #666; font-size: 14px;");
        amountRow->addWidget(currentLabel);
        amountRow->addWidget(targetLabel);
        amountRow->addStretch();

        // Progress bar
        QProgressBar* progressBar = new QProgressBar();
        progressBar->setValue(static_cast<int>(calculateProgress()));
        progressBar->setStyleSheet(
            "QProgressBar {"
            "border: 1px solid #ccc;"
            "border-radius: 5px;"
            "text-align: center;"
            "height: 20px;"
            "}"
            "QProgressBar::chunk {"
            "background-color: " + getColorCode() + ";"
            "border-radius: 5px;"
            "}"
        );

        // End date
        QLabel* dateLabel = new QLabel(QString("Đến: %1").arg(endDate.toString("dd/MM/yyyy")));
        dateLabel->setStyleSheet("color: #666; font-size: 12px;");

        // Description
        QLabel* descLabel = new QLabel(getDescription());
        descLabel->setStyleSheet("color: #666; font-size: 12px;");
        descLabel->setWordWrap(true);

        // Action buttons
        QHBoxLayout* actionRow = new QHBoxLayout();
        QPushButton* depositBtn = new QPushButton("Nạp");
        QPushButton* withdrawBtn = new QPushButton("Rút");
        QPushButton* editBtn = new QPushButton("Sửa");
        QPushButton* deleteBtn = new QPushButton("Xóa");

        depositBtn->setObjectName("deposit_" + getID());
        withdrawBtn->setObjectName("withdraw_" + getID());
        editBtn->setObjectName("edit_" + getID());
        deleteBtn->setObjectName("delete_" + getID());

        depositBtn->setStyleSheet("background-color: #28a745; color: white; border: none; padding: 5px 10px; border-radius: 3px;");
        withdrawBtn->setStyleSheet("background-color: #ffc107; color: white; border: none; padding: 5px 10px; border-radius: 3px;");
        editBtn->setStyleSheet("background-color: #007bff; color: white; border: none; padding: 5px 10px; border-radius: 3px;");
        deleteBtn->setStyleSheet("background-color: #dc3545; color: white; border: none; padding: 5px 10px; border-radius: 3px;");

        actionRow->addStretch();
        actionRow->addWidget(depositBtn);
        actionRow->addWidget(withdrawBtn);
        actionRow->addWidget(editBtn);
        actionRow->addWidget(deleteBtn);

        mainLayout->addWidget(nameLabel);
        mainLayout->addLayout(amountRow);
        mainLayout->addWidget(progressBar);
        mainLayout->addWidget(dateLabel);
        mainLayout->addWidget(descLabel);
        mainLayout->addLayout(actionRow);

        return card;
    }

    QString getCardHtml() const override {
        double progress = calculateProgress();
        return QString(
            "<div style='background: white; border-radius: 8px; padding: 15px; margin: 10px; border: 1px solid #e0e0e0;'>"
            "<div style='font-weight: bold; font-size: 16px; color: #333; margin-bottom: 10px;'>%1</div>"
            "<div style='color: %2; font-weight: bold; font-size: 14px;'>%3 VNĐ <span style='color: #666;'>/ %4 VNĐ</span></div>"
            "<div style='background: #e0e0e0; border-radius: 5px; height: 20px; margin: 10px 0;'>"
            "<div style='background: %2; width: %5%; height: 100%; border-radius: 5px;'></div>"
            "</div>"
            "<div style='color: #666; font-size: 12px;'>Đến: %6</div>"
            "<div style='color: #666; font-size: 12px; margin-top: 5px;'>%7</div>"
            "</div>"
        ).arg(name)
         .arg(getColorCode())
         .arg(getAmount(), 0, 'f', 0)
         .arg(amountTarget, 0, 'f', 0)
         .arg(progress, 0, 'f', 1)
         .arg(endDate.toString("dd/MM/yyyy"))
         .arg(getDescription());
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
