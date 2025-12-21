#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QMainWindow>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QIcon>
#include <QLineEdit>
#include <QVector>
#include <QPair>
#include <QDialog>
#include <QComboBox>
#include <QSpinBox>
#include <QTextEdit>
#include <QDateEdit>
#include <QPainter>
#include <QPaintEvent>
#include <QWidget>
#include <QTimer>
#include <QProgressBar>
#include "transaction.h"
#include "saving.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Dashboard;
}
QT_END_NAMESPACE


class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChartWidget(QWidget *parent = nullptr);
    void setData(double totalIncome, double totalExpense);
    void updateChart();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    double m_totalIncome;
    double m_totalExpense;
    void drawPieChart(QPainter* painter, const QRect& rect);
    void drawLegend(QPainter* painter, const QRect& rect);
};

class Dashboard : public QMainWindow
{
    Q_OBJECT

public:
    Dashboard(QWidget *parent = nullptr);
    ~Dashboard();

    Ui::Dashboard *ui;

    static QVector<int> computeLPS(const QString& pattern);
    static bool KMPSearch(const QString& text, const QString& pattern);

    QFrame* createCategoryCard(const QString& amount, const QString& time, const QString& title, const QString& description, QString index);
    void renderHtmlCard(const QString &categoryName,double amount,const QDateTime &time,const QString &description,QString type, const QString& parent);
    void renderCards();
    void updateMonthlyExpenseTotal();
    void updateMonthlyIncomeTotal();
    void refreshAllDataViews();
    void clearScrollAreaSafely(QScrollArea* scrollArea);
    void updateExpenseChange();
    void updateExpenseTrend();
    void updateIncomeChange();
    void updateIncomeTrend();
    void updateDashboardOverview();

    QString extractCardText(QFrame* card);
    void ensureCardsAtTop(QScrollArea* scrollArea);
    double calculateMonthlyExpense(int year, int month);
    QVector<double> getLastThreeMonthsExpenses();
    double calculateMonthlyIncome(int year, int month);
    QVector<double> getLastThreeMonthsIncomes();


    void setupChart();
    void updateChart();
    double getTotalIncomeForCurrentMonth();
    double getTotalExpenseForCurrentMonth();


    void updateSavingStatistics();
    void refreshSavingGoals();
    void createSavingGoalCard(const Saving& saving);
    QFrame* createSavingGoalWidget(const QString& goalName, double targetAmount, double currentAmount, const QString& description, const QString& goalId);
    void clearSavingGoalsDisplay();
    double calculateProgressPercentage(double current, double target);
    void showAddSavingGoalDialog();
    void showSavingTransactionDialog(const QString& type, const QString& goalId);
    void showEditSavingGoalDialog(const QString& goalId);
    void showSavingReportDialog();
    void searchSavingGoals(const QString& keyword);
    void applySavingSorting(const QString& sortType);
    void deleteSavingGoal(const QString& goalId);
    QString generateNextSavingId();

private slots:
    void on_DashBoard_btn_2_clicked();
    void on_expense_btn_2_clicked();
    void on_income_btn_2_clicked();
    void on_saving_btn_2_clicked();
    void on_debt_btn_2_clicked();
    void on_expenseSearchButton_clicked();
    void on_expenseClearButton_clicked();
    void on_incomeSearchButton_clicked();
    void on_incomeClearButton_clicked();
    void on_addExpenseButton_clicked();
    void on_addIncomeButton_clicked();

    void on_addSavingGoalBtn_clicked();
    void on_depositBtn_clicked();
    void on_withdrawBtn_clicked();
    void on_savingReportBtn_clicked();
    void on_savingSearchButton_clicked();
    void on_savingClearButton_clicked();
    void onSavingSortChanged(const QString& sortType);
    void on_saving_btn_clicked();
    void searchExpense(const QString& keyword);
    void clearExpenseSearch();
    void searchIncome(const QString& keyword);
    void clearIncomeSearch();
    void showAddTransactionDialog(const QString& type);

    void on_expenseExpandingButton_clicked();
    void on_incomeExpandingButton_clicked();
    void handleExpenseCardAction();
    void handleIncomeCardAction();

    void editIncomeTransaction(const QString& incomeId);
    void editExpenseTransaction(const QString& expenseId);
    void removeIncomeTransaction(const QString& incomeId);
    void removeExpenseTransaction(const QString& expenseId);

    void applyExpenseFiltersToMainList();
    void applyIncomeFiltersToMainList();
    void clearAllFilters();
    void applySortToMainList(const QString& type, const QString& sortBy, bool ascending);

private:
    QVector<QPair<QString, QFrame*>> originalExpenseCards;
    QVector<QPair<QString, QFrame*>> originalIncomeCards;
    bool searchModeActive = false;
    bool incomeSearchModeActive = false;

    struct FilterState {
        bool active = false;
        QString sortBy = "date";
        bool ascending = false;
        QString categoryFilter = "all";
        QDate startDate;
        QDate endDate;
        double minAmount = 0.0;
        double maxAmount = 999999.0;
        bool dateFilterActive = false;
        bool amountFilterActive = false;
    };

    FilterState expenseFilterState;
    FilterState incomeFilterState;

    ChartWidget* m_chartWidget;
    QVector<QPair<QString, QFrame*>> originalSavingCards;
    bool savingSearchModeActive = false;
};
#endif
