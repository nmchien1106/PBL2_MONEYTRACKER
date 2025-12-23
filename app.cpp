#include "app.h"
#include "login_dialog.h"
#include "dashboard.h"

QVector<Category> App::categoryList = {};
QVector<Income> App::incomeList = {};
QVector<Expense> App::expenseList = {};
QVector<Saving> App::savingList = {};
QVector<Debt> App::debtList = {};

// Initialize custom data structures
HashTable<QString, Category*> App::categoryHashTable;
LinkedList<QString> App::recentTransactionIds;


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
    debtList = fh.readList<Debt>("data/deptData.csv");

    // Build hash table for fast category lookup
    buildCategoryHashTable();

    qDebug() << "Loaded data - Categories:" << categoryList.size() << "Income:" << incomeList.size() << "Expense:" << expenseList.size() << "Savings:" << savingList.size() << "Debts:" << debtList.size();

    m_dashboard->renderCards();
    m_dashboard->updateMonthlyExpenseTotal();
    m_dashboard->updateMonthlyIncomeTotal();
    m_dashboard->updateDashboardOverview();
    
    // Load debt and saving data on startup
    m_dashboard->renderDebtCards();
    m_dashboard->updateDebtOverview();
    m_dashboard->refreshSavingGoals();
    m_dashboard->updateSavingStatistics();
    
    return;
}

void App::writeData()
{
    FileHandler fh;
    fh.writeList<Category>("data/categoryData.csv", App::categoryList);
    fh.writeList<Income>("data/incomeData.csv", App::incomeList);
    fh.writeList<Expense>("data/expenseData.csv", App::expenseList);
    fh.writeList<Saving>("data/saveData.csv", App::savingList);
    fh.writeList<Debt>("data/deptData.csv", App::debtList);

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

// Optimized O(1) lookup using HashTable
Category *App::findCategoryByIDFast(const QString &id)
{
    Category* result = nullptr;
    categoryHashTable.find(id, result);  // find() returns bool and sets result
    return result;  // Returns nullptr if not found
}

// Build hash table from category list for fast lookup
void App::buildCategoryHashTable()
{
    qDebug() << "Building category hash table...";
    for (auto &category : categoryList)
    {
        categoryHashTable.insert(category.getID(), &category);
    }
    qDebug() << "Hash table built with" << categoryList.size() << "categories";
}

// Add transaction ID to recent transactions (keep only last 50)
void App::addToRecentTransactions(const QString& transactionId)
{
    // Add to front of list
    recentTransactionIds.prepend(transactionId);

    // Keep only last 50 transactions
    while (recentTransactionIds.getSize() > 50) {
        recentTransactionIds.removeAt(recentTransactionIds.getSize() - 1);
    }

    qDebug() << "Added transaction" << transactionId << "to recent list. Total recent:" << recentTransactionIds.getSize();
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
    Category* category = findCategoryByIDFast(categoryId);  // Use optimized O(1) lookup
    if (!category) {
        category = findCategoryByID(categoryId);  // Fallback to linear search
    }

    if (category) {
        Income newIncome(id, category, QDateTime::currentDateTime(),
                         QDate::currentDate(), amount, description);
        incomeList.append(newIncome);
        addToRecentTransactions(id);  // Track recent transaction
    }
}

void App::addExpense(const QString& categoryId, double amount, const QString& description) {
    QString id = generateNextExpenseId();
    Category* category = findCategoryByIDFast(categoryId);  // Use optimized O(1) lookup
    if (!category) {
        category = findCategoryByID(categoryId);  // Fallback to linear search
    }

    if (category) {
        Expense newExpense(id, category, QDateTime::currentDateTime(),
                           QDate::currentDate(), amount, description);
        expenseList.append(newExpense);
        addToRecentTransactions(id);  // Track recent transaction
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

// Debt management methods
void App::addDebt(const QString& categoryId, double amount, const QString& description,
                 const QString& debtorName, const QDate& dueDate, const QString& debtType, bool isPaid) {
    QString newId = generateNextDebtId();
    Category* category = findCategoryByIDFast(categoryId);  // Use optimized lookup
    if (!category) {
        category = findCategoryByID(categoryId);  // Fallback to linear search
    }
    Debt newDebt(newId, category, QDateTime::currentDateTime(), QDate::currentDate(),
                 amount, description, debtorName, dueDate, isPaid, debtType);
    debtList.append(newDebt);
    addToRecentTransactions(newId);  // Track recent transaction
    qDebug() << "Added debt with ID:" << newId << "isPaid:" << isPaid;
}

QString App::generateNextDebtId() {
    int maxId = 0;
    for (const Debt& debt : debtList) {
        QString idStr = debt.getID();
        if (idStr.startsWith("DT")) {
            bool ok;
            int id = idStr.mid(2).toInt(&ok);
            if (ok && id > maxId) {
                maxId = id;
            }
        }
    }
    return QString("DT%1").arg(maxId + 1, 4, 10, QChar('0'));
}

bool App::removeDebtById(const QString& debtId) {
    for (int i = 0; i < debtList.size(); i++) {
        if (debtList[i].getID() == debtId) {
            debtList.removeAt(i);
            qDebug() << "Removed debt with ID:" << debtId;
            return true;
        }
    }
    qDebug() << "Debt with ID not found:" << debtId;
    return false;
}

bool App::markDebtAsPaid(const QString& debtId, bool isPaid) {
    for (int i = 0; i < debtList.size(); i++) {
        if (debtList[i].getID() == debtId) {
            debtList[i].setIsPaid(isPaid);
            qDebug() << "Marked debt as" << (isPaid ? "paid" : "unpaid") << "- ID:" << debtId;
            return true;
        }
    }
    qDebug() << "Debt with ID not found:" << debtId;
    return false;
}

