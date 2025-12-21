#include "app.h"
#include "login_dialog.h"
#include "dashboard.h"

QVector<Category> App::categoryList = {};
QVector<Income> App::incomeList = {};
QVector<Expense> App::expenseList = {};
QVector<Saving> App::savingList = {};


App::App(QApplication &app) : m_app(app), QObject(nullptr)
{
    this->m_login = new LoginUI();
    this->m_dashboard = new Dashboard();

    connect(m_login, &LoginUI::LoginSuccess, this, &App::showDashboard);
}

App::~App()
{
    writeData();
    delete this->m_login;
}

void App::run()
{
    m_login->show();
    loadData();
    return;
}

void App::showLogin()
{
    m_login->show();
}

void App::showDashboard()
{
    m_dashboard->show();
    m_login->close();
}

void App::loadData()
{
    FileHandler fh;
    qDebug() << "Loading data from files...";
    categoryList = fh.readList<Category>("data/categoryData.csv");
    incomeList = fh.readList<Income>("data/incomeData.csv");
    savingList = fh.readList<Saving>("data/saveData.csv");
    expenseList = fh.readList<Expense>("data/expenseData.csv");

    qDebug() << "Loaded data - Categories:" << categoryList.size() << "Income:" << incomeList.size() << "Expense:" << expenseList.size() << "Savings:" << savingList.size();

    m_dashboard->renderCards();
    m_dashboard->updateMonthlyExpenseTotal();
    m_dashboard->updateMonthlyIncomeTotal();
    m_dashboard->updateDashboardOverview();
    return;
}

void App::writeData()
{
    FileHandler fh;
    fh.writeList<Category>("data/categoryData.csv", App::categoryList);
    fh.writeList<Income>("data/incomeData.csv", App::incomeList);
    fh.writeList<Expense>("data/expenseData.csv", App::expenseList);
    fh.writeList<Saving>("data/saveData.csv", App::savingList);

    return;
}

Category *App::findCategoryByID(const QString &id)
{
    for (auto &c : categoryList)
    {
        if (c.getID() == id)
            return &c;
    }
    return nullptr;
}

QString App::formatMoney(double amount)
{
    QLocale locale(QLocale::Vietnamese, QLocale::Vietnam);
    return locale.toString(static_cast<qlonglong>(amount)) + " VNĐ";
}

double App::getMonthExpenseTotal()
{

    double total = 0.0;
    QDate currentDate = QDate::currentDate();
    int currentMonth = currentDate.month();
    int currentYear = currentDate.year();



    for (const Expense &expense : App::getExpenseList()) {
        QDate expenseDate = expense.getCreatedAt().date();

        if (expenseDate.month() == currentMonth && expenseDate.year() == currentYear) {
            total += expense.getAmount();

        }
    }

    return total;
}

double App::getMonthIncomeTotal()
{

    double total = 0.0;
    QDate currentDate = QDate::currentDate();
    int currentMonth = currentDate.month();
    int currentYear = currentDate.year();


    for (const Income &income : App::getIncomeList()) {
        QDate incomeDate = income.getCreatedAt().date();

        if (incomeDate.month() == currentMonth && incomeDate.year() == currentYear) {
            total += income.getAmount();
        }
    }

    return total;
}


void App::addIncome(const QString& categoryId, double amount, const QString& description) {
    QString id = generateNextIncomeId();
    Category* category = findCategoryByID(categoryId);

    if (category) {
        Income newIncome(id, category, QDateTime::currentDateTime(),
                         QDate::currentDate(), amount, description);
        incomeList.append(newIncome);
    }
}

void App::addExpense(const QString& categoryId, double amount, const QString& description) {
    QString id = generateNextExpenseId();
    Category* category = findCategoryByID(categoryId);

    if (category) {
        Expense newExpense(id, category, QDateTime::currentDateTime(),
                           QDate::currentDate(), amount, description);
        expenseList.append(newExpense);
    }
}

QString App::generateNextIncomeId() {
    int maxId = 0;
    for (const Income& income : incomeList) {
        QString idStr = income.getID();
        if (idStr.startsWith("IN")) {
            bool ok;
            int id = idStr.mid(2).toInt(&ok);
            if (ok && id > maxId) {
                maxId = id;
            }
        }
    }
    return QString("IN%1").arg(maxId + 1, 4, 10, QChar('0'));
}

QString App::generateNextExpenseId() {
    int maxId = 0;
    for (const Expense& expense : expenseList) {
        QString idStr = expense.getID();
        if (idStr.startsWith("EX")) {
            bool ok;
            int id = idStr.mid(2).toInt(&ok);
            if (ok && id > maxId) {
                maxId = id;
            }
        }
    }
    return QString("EX%1").arg(maxId + 1, 4, 10, QChar('0'));
}

double App::getCurrentBalance() {
    double totalIncome = 0.0;
    double totalExpense = 0.0;

    for (const Income& income : incomeList) {
        totalIncome += income.getAmount();
    }

    for (const Expense& expense : expenseList) {
        totalExpense += expense.getAmount();
    }

    qDebug() << "Total Income:" << totalIncome << "Total Expense:" << totalExpense;
    qDebug() << "Income list size:" << incomeList.size() << "Expense list size:" << expenseList.size();

    return totalIncome - totalExpense;
}

bool App::removeIncomeById(const QString& incomeId) {
    for (int i = 0; i < incomeList.size(); i++) {
        if (incomeList[i].getID() == incomeId) {
            incomeList.removeAt(i);
            qDebug() << "Removed income with ID:" << incomeId;
            return true;
        }
    }
    qDebug() << "Income with ID not found:" << incomeId;
    return false;
}

bool App::removeExpenseById(const QString& expenseId) {
    for (int i = 0; i < expenseList.size(); i++) {
        if (expenseList[i].getID() == expenseId) {
            expenseList.removeAt(i);
            qDebug() << "Removed expense with ID:" << expenseId;
            return true;
        }
    }
    qDebug() << "Expense with ID not found:" << expenseId;
    return false;
}


