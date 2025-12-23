/********************************************************************************
** Form generated from reading UI file 'add_transaction_dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADD_TRANSACTION_DIALOG_H
#define UI_ADD_TRANSACTION_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AddTransactionDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QFrame *separatorLine;
    QVBoxLayout *categoryLayout;
    QHBoxLayout *categoryLabelLayout;
    QLabel *categoryLabel;
    QSpacerItem *horizontalSpacer;
    QPushButton *manageCategoryButton;
    QComboBox *categoryComboBox;
    QVBoxLayout *amountLayout;
    QLabel *amountLabel;
    QLineEdit *amountLineEdit;
    QVBoxLayout *dateTimeLayout;
    QLabel *dateTimeLabel;
    QDateTimeEdit *dateTimeEdit;
    QVBoxLayout *descriptionLayout;
    QLabel *descriptionLabel;
    QTextEdit *descriptionTextEdit;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *buttonLayout;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *cancelButton;
    QPushButton *submitButton;

    void setupUi(QDialog *AddTransactionDialog)
    {
        if (AddTransactionDialog->objectName().isEmpty())
            AddTransactionDialog->setObjectName("AddTransactionDialog");
        AddTransactionDialog->resize(550, 650);
        AddTransactionDialog->setMinimumSize(QSize(550, 650));
        AddTransactionDialog->setMaximumSize(QSize(550, 650));
        AddTransactionDialog->setStyleSheet(QString::fromUtf8("QDialog {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                                stop:0 #f8f9fa, stop:1 #e9ecef);\n"
"}\n"
"\n"
"QLabel {\n"
"    font-size: 13px;\n"
"    font-weight: 500;\n"
"    color: #495057;\n"
"}\n"
"\n"
"QLabel#titleLabel {\n"
"    font-size: 24px;\n"
"    font-weight: 700;\n"
"    color: #212529;\n"
"}\n"
"\n"
"QLineEdit, QTextEdit, QComboBox, QDateTimeEdit {\n"
"    padding: 10px;\n"
"    border: 2px solid #ced4da;\n"
"    border-radius: 8px;\n"
"    background: white;\n"
"    font-size: 13px;\n"
"    color: #212529;\n"
"}\n"
"\n"
"QLineEdit:focus, QTextEdit:focus, QComboBox:focus, QDateTimeEdit:focus {\n"
"    border-color: #0d6efd;\n"
"    outline: none;\n"
"}\n"
"\n"
"QComboBox::drop-down {\n"
"    border: none;\n"
"    width: 30px;\n"
"}\n"
"\n"
"QComboBox::down-arrow {\n"
"    image: url(:/icons/down-arrow.png);\n"
"    width: 12px;\n"
"    height: 12px;\n"
"}\n"
"\n"
"QDateTimeEdit::drop-down {\n"
"    border: none;\n"
"    width: 30px;\n"
"}\n"
"\n"
"QD"
                        "ateTimeEdit::down-arrow {\n"
"    image: url(:/icons/calendar.png);\n"
"    width: 14px;\n"
"    height: 14px;\n"
"}\n"
"\n"
"QPushButton {\n"
"    padding: 12px 24px;\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    font-size: 14px;\n"
"    font-weight: 600;\n"
"    min-width: 120px;\n"
"}\n"
"\n"
"QPushButton#submitButton {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"                                stop:0 #0d6efd, stop:1 #0a58ca);\n"
"    color: white;\n"
"}\n"
"\n"
"QPushButton#submitButton:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"                                stop:0 #0a58ca, stop:1 #084298);\n"
"}\n"
"\n"
"QPushButton#cancelButton {\n"
"    background: #6c757d;\n"
"    color: white;\n"
"}\n"
"\n"
"QPushButton#cancelButton:hover {\n"
"    background: #5a6268;\n"
"}\n"
"\n"
"QPushButton#manageCategoryButton {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"                                stop:0 #198754, stop:1 #146c43);\n"
"    col"
                        "or: white;\n"
"    padding: 8px 16px;\n"
"    min-width: 100px;\n"
"}\n"
"\n"
"QPushButton#manageCategoryButton:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"                                stop:0 #146c43, stop:1 #0d5132);\n"
"}\n"
"\n"
"QFrame#separatorLine {\n"
"    background-color: #dee2e6;\n"
"    max-height: 1px;\n"
"}"));
        verticalLayout = new QVBoxLayout(AddTransactionDialog);
        verticalLayout->setSpacing(20);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(30, 30, 30, 30);
        titleLabel = new QLabel(AddTransactionDialog);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(titleLabel);

        separatorLine = new QFrame(AddTransactionDialog);
        separatorLine->setObjectName("separatorLine");
        separatorLine->setFrameShape(QFrame::HLine);
        separatorLine->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(separatorLine);

        categoryLayout = new QVBoxLayout();
        categoryLayout->setSpacing(8);
        categoryLayout->setObjectName("categoryLayout");
        categoryLabelLayout = new QHBoxLayout();
        categoryLabelLayout->setObjectName("categoryLabelLayout");
        categoryLabel = new QLabel(AddTransactionDialog);
        categoryLabel->setObjectName("categoryLabel");

        categoryLabelLayout->addWidget(categoryLabel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        categoryLabelLayout->addItem(horizontalSpacer);

        manageCategoryButton = new QPushButton(AddTransactionDialog);
        manageCategoryButton->setObjectName("manageCategoryButton");

        categoryLabelLayout->addWidget(manageCategoryButton);


        categoryLayout->addLayout(categoryLabelLayout);

        categoryComboBox = new QComboBox(AddTransactionDialog);
        categoryComboBox->setObjectName("categoryComboBox");
        categoryComboBox->setMinimumSize(QSize(0, 45));

        categoryLayout->addWidget(categoryComboBox);


        verticalLayout->addLayout(categoryLayout);

        amountLayout = new QVBoxLayout();
        amountLayout->setSpacing(8);
        amountLayout->setObjectName("amountLayout");
        amountLabel = new QLabel(AddTransactionDialog);
        amountLabel->setObjectName("amountLabel");

        amountLayout->addWidget(amountLabel);

        amountLineEdit = new QLineEdit(AddTransactionDialog);
        amountLineEdit->setObjectName("amountLineEdit");
        amountLineEdit->setMinimumSize(QSize(0, 45));

        amountLayout->addWidget(amountLineEdit);


        verticalLayout->addLayout(amountLayout);

        dateTimeLayout = new QVBoxLayout();
        dateTimeLayout->setSpacing(8);
        dateTimeLayout->setObjectName("dateTimeLayout");
        dateTimeLabel = new QLabel(AddTransactionDialog);
        dateTimeLabel->setObjectName("dateTimeLabel");

        dateTimeLayout->addWidget(dateTimeLabel);

        dateTimeEdit = new QDateTimeEdit(AddTransactionDialog);
        dateTimeEdit->setObjectName("dateTimeEdit");
        dateTimeEdit->setMinimumSize(QSize(0, 45));
        dateTimeEdit->setCalendarPopup(true);

        dateTimeLayout->addWidget(dateTimeEdit);


        verticalLayout->addLayout(dateTimeLayout);

        descriptionLayout = new QVBoxLayout();
        descriptionLayout->setSpacing(8);
        descriptionLayout->setObjectName("descriptionLayout");
        descriptionLabel = new QLabel(AddTransactionDialog);
        descriptionLabel->setObjectName("descriptionLabel");

        descriptionLayout->addWidget(descriptionLabel);

        descriptionTextEdit = new QTextEdit(AddTransactionDialog);
        descriptionTextEdit->setObjectName("descriptionTextEdit");
        descriptionTextEdit->setMinimumSize(QSize(0, 100));
        descriptionTextEdit->setMaximumSize(QSize(16777215, 100));

        descriptionLayout->addWidget(descriptionTextEdit);


        verticalLayout->addLayout(descriptionLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(15);
        buttonLayout->setObjectName("buttonLayout");
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonLayout->addItem(horizontalSpacer_2);

        cancelButton = new QPushButton(AddTransactionDialog);
        cancelButton->setObjectName("cancelButton");

        buttonLayout->addWidget(cancelButton);

        submitButton = new QPushButton(AddTransactionDialog);
        submitButton->setObjectName("submitButton");

        buttonLayout->addWidget(submitButton);


        verticalLayout->addLayout(buttonLayout);


        retranslateUi(AddTransactionDialog);

        QMetaObject::connectSlotsByName(AddTransactionDialog);
    } // setupUi

    void retranslateUi(QDialog *AddTransactionDialog)
    {
        AddTransactionDialog->setWindowTitle(QCoreApplication::translate("AddTransactionDialog", "Th\303\252m Giao D\341\273\213ch", nullptr));
        titleLabel->setText(QCoreApplication::translate("AddTransactionDialog", "Th\303\252m Giao D\341\273\213ch M\341\273\233i", nullptr));
        categoryLabel->setText(QCoreApplication::translate("AddTransactionDialog", "Danh m\341\273\245c *", nullptr));
        manageCategoryButton->setText(QCoreApplication::translate("AddTransactionDialog", "Qu\341\272\243n l\303\275", nullptr));
        amountLabel->setText(QCoreApplication::translate("AddTransactionDialog", "S\341\273\221 ti\341\273\201n (VN\304\220) *", nullptr));
        amountLineEdit->setPlaceholderText(QCoreApplication::translate("AddTransactionDialog", "Nh\341\272\255p s\341\273\221 ti\341\273\201n...", nullptr));
        dateTimeLabel->setText(QCoreApplication::translate("AddTransactionDialog", "Th\341\273\235i gian *", nullptr));
        dateTimeEdit->setDisplayFormat(QCoreApplication::translate("AddTransactionDialog", "dd/MM/yyyy HH:mm", nullptr));
        descriptionLabel->setText(QCoreApplication::translate("AddTransactionDialog", "M\303\264 t\341\272\243", nullptr));
        descriptionTextEdit->setPlaceholderText(QCoreApplication::translate("AddTransactionDialog", "Nh\341\272\255p m\303\264 t\341\272\243 chi ti\341\272\277t (t\303\271y ch\341\273\215n)...", nullptr));
        cancelButton->setText(QCoreApplication::translate("AddTransactionDialog", "H\341\273\247y", nullptr));
        submitButton->setText(QCoreApplication::translate("AddTransactionDialog", "Th\303\252m", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AddTransactionDialog: public Ui_AddTransactionDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADD_TRANSACTION_DIALOG_H
