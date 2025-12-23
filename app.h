#ifndef APP_H
#define APP_H

#include <QObject>
#include <QApplication>
#include <QDebug>
#include <QVector>
#include "hashtable.h"
#include "linkedlist.h"

#include "filehandler.h"
#include "category.h"
#include "income.h"
#include "expense.h"
#include "saving.h"
#include "debt.h"
#include <QLocale>

class LoginUI;
class Dashboard;


class App : public QObject {
    Q_OBJECT
private:

    QApplication &m_app;
    LoginUI *m_login;
    Dashboard *m_dashboard;

    static QVector<Category> categoryList;
    static QVector<Income> incomeList;
    static QVector<Expense> expenseList;
    static QVector<Saving> savingList;
    static QVector<Debt> debtList;

    // Custom data structures for optimization
    static HashTable<QString, Category*> categoryHashTable;
    static LinkedList<QString> recentTransactionIds;  // Keep track of recent 50 transactions
public:
    App(QApplication&);
    ~App();
    void run();

    void showLogin();
    void showDashboard();

    void loadData();
    static void writeData();

    static Category* findCategoryByID(const QString&);
    static Category* findCategoryByIDFast(const QString&);  // Using HashTable O(1)
    static void buildCategoryHashTable();  // Build hash table from category list
    static void addToRecentTransactions(const QString& transactionId);
    static LinkedList<QString>& getRecentTransactions() { return recentTransactionIds; }
    static QString formatMoney(double amount);
    static double getMonthExpenseTotal();
    static double getMonthIncomeTotal();
    static double getCurrentBalance();




    static void addIncome(const QString& categoryId, double amount, const QString& description);
    static void addExpense(const QString& categoryId, double amount, const QString& description);
    static QString generateNextIncomeId();
    static QString generateNextExpenseId();
    static bool removeIncomeById(const QString& incomeId);
    static bool removeExpenseById(const QString& expenseId);

    static const QVector<Expense>& getExpenseList() { return expenseList; }
    static const QVector<Saving>& getSavingList() { return savingList; }
    static const QVector<Income>& getIncomeList() { return incomeList; }
    static const QVector<Category>& getCategoryList() { return categoryList; }
    static const QVector<Debt>& getDebtList() { return debtList; }

    // Debt management methods
    static void addDebt(const QString& categoryId, double amount, const QString& description,
                       const QString& debtorName, const QDate& dueDate, const QString& debtType, bool isPaid = false);
    static QString generateNextDebtId();
    static bool removeDebtById(const QString& debtId);
    static bool markDebtAsPaid(const QString& debtId, bool isPaid);
};

#endif
