/********************************************************************************
** Form generated from reading UI file 'login_dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_DIALOG_H
#define UI_LOGIN_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginUI
{
public:
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_4;
    QWidget *widget;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label;
    QLabel *label_2;
    QSpacerItem *verticalSpacer;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QSpacerItem *verticalSpacer_2;
    QPushButton *connectDashboard_btn;

    void setupUi(QDialog *LoginUI)
    {
        if (LoginUI->objectName().isEmpty())
            LoginUI->setObjectName("LoginUI");
        LoginUI->resize(703, 573);
        frame = new QFrame(LoginUI);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(0, 0, 701, 571));
        frame->setStyleSheet(QString::fromUtf8("background-color: rgb(33, 166, 255);"));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName("verticalLayout");
        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_4);

        widget = new QWidget(frame);
        widget->setObjectName("widget");
        widget->setMinimumSize(QSize(100, 150));
        widget->setMaximumSize(QSize(16777215, 500));
        label_6 = new QLabel(widget);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(220, 20, 111, 111));
        label_6->setPixmap(QPixmap(QString::fromUtf8(":/img/src/images.png")));
        label_6->setScaledContents(true);
        label_7 = new QLabel(widget);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(350, 20, 111, 111));
        label_7->setPixmap(QPixmap(QString::fromUtf8(":/img/src/images.jpg")));
        label_7->setScaledContents(true);

        verticalLayout->addWidget(widget);

        label = new QLabel(frame);
        label->setObjectName("label");

        verticalLayout->addWidget(label);

        label_2 = new QLabel(frame);
        label_2->setObjectName("label_2");

        verticalLayout->addWidget(label_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        label_3 = new QLabel(frame);
        label_3->setObjectName("label_3");

        verticalLayout->addWidget(label_3);

        label_4 = new QLabel(frame);
        label_4->setObjectName("label_4");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);
        label_4->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        label_4->setScaledContents(false);
        label_4->setAlignment(Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignTop);
        label_4->setWordWrap(false);

        verticalLayout->addWidget(label_4);

        label_5 = new QLabel(frame);
        label_5->setObjectName("label_5");

        verticalLayout->addWidget(label_5);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        connectDashboard_btn = new QPushButton(frame);
        connectDashboard_btn->setObjectName("connectDashboard_btn");
        connectDashboard_btn->setMinimumSize(QSize(0, 50));
        connectDashboard_btn->setStyleSheet(QString::fromUtf8("font: 700 16pt \"Segoe UI\";"));

        verticalLayout->addWidget(connectDashboard_btn);


        retranslateUi(LoginUI);

        QMetaObject::connectSlotsByName(LoginUI);
    } // setupUi

    void retranslateUi(QDialog *LoginUI)
    {
        LoginUI->setWindowTitle(QCoreApplication::translate("LoginUI", "Dialog", nullptr));
        label_6->setText(QString());
        label_7->setText(QString());
        label->setText(QCoreApplication::translate("LoginUI", "<html><head/><body><p align=\"center\"><span style=\" font-size:18pt; font-weight:700;\">PBL2 : \304\220\341\273\222 \303\201N C\306\240 S\341\273\236 L\341\272\254P TR\303\214NH</span></p></body></html>", nullptr));
        label_2->setText(QCoreApplication::translate("LoginUI", "<html><head/><body><p align=\"center\"><span style=\" font-size:18pt; font-weight:700; color:#ffffff;\">H\341\273\207 th\341\273\221ng qu\341\272\243n l\303\275 chi ti\303\252u c\303\241 nh\303\242n</span></p></body></html>", nullptr));
        label_3->setText(QCoreApplication::translate("LoginUI", "<html><head/><body><p align=\"center\"><span style=\" font-size:12pt; font-weight:700;\">Sinh vi\303\252n th\341\273\261c hi\341\273\207n:</span></p></body></html>", nullptr));
        label_4->setText(QCoreApplication::translate("LoginUI", "<html><head/><body><p align=\"center\"><span style=\" font-size:12pt; font-weight:700;\">Nguy\341\273\205n Minh Chi\341\272\277n  - 24T_DT4</span></p><p align=\"center\"><span style=\" font-size:12pt; font-weight:700;\">V\303\265 V\304\203n \304\220\341\272\241t - 24T_DT4</span></p></body></html>", nullptr));
        label_5->setText(QCoreApplication::translate("LoginUI", "<html><head/><body><p align=\"center\"><span style=\" font-size:12pt; font-weight:700;\">Gi\341\272\243ng vi\303\252n h\306\260\341\273\233ng d\341\272\253n : TS Nguy\341\273\205n N\304\203ng H\303\271ng V\303\242n</span></p></body></html>", nullptr));
        connectDashboard_btn->setText(QCoreApplication::translate("LoginUI", "TRUY C\341\272\254P", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginUI: public Ui_LoginUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_DIALOG_H
