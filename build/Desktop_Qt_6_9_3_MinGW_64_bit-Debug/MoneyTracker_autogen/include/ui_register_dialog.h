/********************************************************************************
** Form generated from reading UI file 'register_dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTER_DIALOG_H
#define UI_REGISTER_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RegisterUI
{
public:
    QFrame *frame;
    QFrame *frame_2;
    QLabel *label_4;
    QLabel *label_5;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label;
    QTextEdit *username_input;
    QTextEdit *password_input;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *RegisterSubmit_btn;
    QLabel *label_10;
    QTextEdit *email_input;
    QPushButton *LoginSignal_btn;

    void setupUi(QDialog *RegisterUI)
    {
        if (RegisterUI->objectName().isEmpty())
            RegisterUI->setObjectName("RegisterUI");
        RegisterUI->resize(699, 392);
        frame = new QFrame(RegisterUI);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(0, 0, 701, 393));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        frame_2 = new QFrame(frame);
        frame_2->setObjectName("frame_2");
        frame_2->setGeometry(QRect(0, -10, 271, 401));
        frame_2->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 170, 255);"));
        frame_2->setFrameShape(QFrame::Shape::StyledPanel);
        frame_2->setFrameShadow(QFrame::Shadow::Raised);
        label_4 = new QLabel(frame_2);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(10, 40, 251, 41));
        QFont font;
        font.setPointSize(14);
        font.setBold(true);
        label_4->setFont(font);
        label_4->setAutoFillBackground(false);
        label_4->setStyleSheet(QString::fromUtf8("background-color: none\n"
""));
        label_4->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_4->setWordWrap(false);
        label_5 = new QLabel(frame_2);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(10, 70, 251, 41));
        label_5->setFont(font);
        label_5->setAutoFillBackground(false);
        label_5->setStyleSheet(QString::fromUtf8("background-color: none\n"
""));
        label_5->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_5->setWordWrap(false);
        layoutWidget = new QWidget(frame_2);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(30, 310, 211, 51));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label_6 = new QLabel(layoutWidget);
        label_6->setObjectName("label_6");
        label_6->setFont(font);
        label_6->setAutoFillBackground(false);
        label_6->setStyleSheet(QString::fromUtf8("background-color: none\n"
""));
        label_6->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_6->setWordWrap(false);

        verticalLayout->addWidget(label_6);

        label_7 = new QLabel(layoutWidget);
        label_7->setObjectName("label_7");
        label_7->setFont(font);
        label_7->setAutoFillBackground(false);
        label_7->setStyleSheet(QString::fromUtf8("background-color: none\n"
""));
        label_7->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_7->setWordWrap(false);

        verticalLayout->addWidget(label_7);

        label_8 = new QLabel(frame_2);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(40, 140, 91, 91));
        label_8->setPixmap(QPixmap(QString::fromUtf8(":/src/images.png")));
        label_8->setScaledContents(true);
        label_9 = new QLabel(frame_2);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(150, 140, 81, 91));
        label_9->setPixmap(QPixmap(QString::fromUtf8(":/src/images.jpg")));
        label_9->setScaledContents(true);
        label = new QLabel(frame);
        label->setObjectName("label");
        label->setGeometry(QRect(320, 40, 291, 41));
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        username_input = new QTextEdit(frame);
        username_input->setObjectName("username_input");
        username_input->setGeometry(QRect(410, 140, 231, 31));
        username_input->setStyleSheet(QString::fromUtf8("QTextEdit {\n"
"	border: 1px solid rgb(76, 76, 76);\n"
"	border-radius:  4px;\n"
"	background : none;\n"
"}"));
        password_input = new QTextEdit(frame);
        password_input->setObjectName("password_input");
        password_input->setGeometry(QRect(410, 220, 231, 31));
        password_input->setStyleSheet(QString::fromUtf8("QTextEdit {\n"
"	border: 1px solid rgb(76, 76, 76);\n"
"	border-radius:  4px;\n"
"	background : none;\n"
"}"));
        label_2 = new QLabel(frame);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(300, 150, 91, 16));
        label_2->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	letter-spacing: 2px;\n"
"}"));
        label_3 = new QLabel(frame);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(300, 230, 91, 16));
        label_3->setStyleSheet(QString::fromUtf8("letter-spacing: 2px;"));
        RegisterSubmit_btn = new QPushButton(frame);
        RegisterSubmit_btn->setObjectName("RegisterSubmit_btn");
        RegisterSubmit_btn->setGeometry(QRect(500, 320, 151, 31));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Segoe UI")});
        font1.setPointSize(9);
        font1.setWeight(QFont::Black);
        font1.setItalic(false);
        RegisterSubmit_btn->setFont(font1);
        RegisterSubmit_btn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: rgb(0, 170, 255);\n"
"	color: rgb(255, 255, 255);\n"
"	border: rgb(0, 170, 255);\n"
"	\n"
"	font: 900 9pt \"Segoe UI\";\n"
"}\n"
"\n"
"QPushButton:Hover {\n"
"	background-color: rgb(0, 150, 225);\n"
"}"));
        label_10 = new QLabel(frame);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(300, 190, 91, 16));
        label_10->setStyleSheet(QString::fromUtf8("letter-spacing: 2px;"));
        email_input = new QTextEdit(frame);
        email_input->setObjectName("email_input");
        email_input->setGeometry(QRect(410, 180, 231, 31));
        email_input->setStyleSheet(QString::fromUtf8("QTextEdit {\n"
"	border: 1px solid rgb(76, 76, 76);\n"
"	border-radius:  4px;\n"
"	background : none;\n"
"}"));
        LoginSignal_btn = new QPushButton(frame);
        LoginSignal_btn->setObjectName("LoginSignal_btn");
        LoginSignal_btn->setGeometry(QRect(320, 320, 151, 31));
        LoginSignal_btn->setFont(font1);
        LoginSignal_btn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: rgb(255, 255, 255);\n"
"	color: rgb(255, 255, 255);\n"
"	border: 1px  solid rgb(83, 83, 83)\n"
"	\n"
"	font: 900 9pt \"Segoe UI\";\n"
"}\n"
"\n"
"QPushButton:Hover {\n"
"	background-color: rgb(0, 150, 225);\n"
"}"));
        frame_2->raise();
        label->raise();
        label_2->raise();
        label_3->raise();
        RegisterSubmit_btn->raise();
        password_input->raise();
        username_input->raise();
        label_10->raise();
        email_input->raise();
        LoginSignal_btn->raise();

        retranslateUi(RegisterUI);

        QMetaObject::connectSlotsByName(RegisterUI);
    } // setupUi

    void retranslateUi(QDialog *RegisterUI)
    {
        RegisterUI->setWindowTitle(QCoreApplication::translate("RegisterUI", "Dialog", nullptr));
        label_4->setText(QCoreApplication::translate("RegisterUI", "<html><head/><body><p>H\341\273\206 TH\341\273\220NG</p></body></html>", nullptr));
        label_5->setText(QCoreApplication::translate("RegisterUI", "<html><head/><body><p><span style=\" color:#ffffff;\">QU\341\272\242N L\303\235 CHI TI\303\212U</span></p></body></html>", nullptr));
        label_6->setText(QCoreApplication::translate("RegisterUI", "<html><head/><body><p><span style=\" font-size:10pt;\">Nguy\341\273\205n Minh Chi\341\272\277n - 102240181</span></p></body></html>", nullptr));
        label_7->setText(QCoreApplication::translate("RegisterUI", "<html><head/><body><p><span style=\" font-size:10pt;\">V\303\265 V\304\203n \304\220\341\272\241t - 102240181</span></p></body></html>", nullptr));
        label_8->setText(QString());
        label_9->setText(QString());
        label->setText(QCoreApplication::translate("RegisterUI", "<html><head/><body><p align=\"center\"><span style=\" font-size:18pt; font-weight:700;\">\304\220\304\202NG K\303\235 T\303\200I KHO\341\272\242N</span></p></body></html>", nullptr));
        label_2->setText(QCoreApplication::translate("RegisterUI", "<html><head/><body><p><span style=\" font-weight:700;\">USERNAME</span></p></body></html>", nullptr));
        label_3->setText(QCoreApplication::translate("RegisterUI", "<html><head/><body><p><span style=\" font-weight:700;\">PASSWORD</span></p></body></html>", nullptr));
        RegisterSubmit_btn->setText(QCoreApplication::translate("RegisterUI", "\304\220\304\202NG K\303\235", nullptr));
        label_10->setText(QCoreApplication::translate("RegisterUI", "<html><head/><body><p><span style=\" font-weight:700;\">EMAIL</span></p></body></html>", nullptr));
        LoginSignal_btn->setText(QCoreApplication::translate("RegisterUI", "\304\220\304\202NG NH\341\272\254P", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegisterUI: public Ui_RegisterUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTER_DIALOG_H
