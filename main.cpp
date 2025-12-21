#include <QApplication>

#include <QMessageBox>
#include <exception>
#include <iostream>
#include "dashboard.h"
#include "login_dialog.h"
#include "app.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    try {
        App app(a);
        app.run();
        return a.exec();
    }
    catch (const std::exception &e) {

        QMessageBox::critical(nullptr, "Lỗi nghiêm trọng", QString("Đã xảy ra lỗi:\n%1").arg(e.what()));
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    catch (...) {

        QMessageBox::critical(nullptr, "Lỗi không xác định", "Đã xảy ra lỗi không rõ nguyên nhân!");
        std::cerr << "Unknown exception caught." << std::endl;
    }

    return 0;
}
