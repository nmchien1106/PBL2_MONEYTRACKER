/****************************************************************************
** Meta object code from reading C++ file 'dashboard.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../dashboard.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dashboard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN11ChartWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto ChartWidget::qt_create_metaobjectdata<qt_meta_tag_ZN11ChartWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ChartWidget"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ChartWidget, qt_meta_tag_ZN11ChartWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ChartWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11ChartWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11ChartWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11ChartWidgetE_t>.metaTypes,
    nullptr
} };

void ChartWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ChartWidget *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *ChartWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChartWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11ChartWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ChartWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {
struct qt_meta_tag_ZN9DashboardE_t {};
} // unnamed namespace

template <> constexpr inline auto Dashboard::qt_create_metaobjectdata<qt_meta_tag_ZN9DashboardE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Dashboard",
        "on_DashBoard_btn_2_clicked",
        "",
        "on_expense_btn_2_clicked",
        "on_income_btn_2_clicked",
        "on_saving_btn_2_clicked",
        "on_debt_btn_2_clicked",
        "on_expenseSearchButton_clicked",
        "on_expenseClearButton_clicked",
        "on_incomeSearchButton_clicked",
        "on_incomeClearButton_clicked",
        "on_addExpenseButton_clicked",
        "on_addIncomeButton_clicked",
        "on_addSavingGoalBtn_clicked",
        "on_depositBtn_clicked",
        "on_withdrawBtn_clicked",
        "on_savingReportBtn_clicked",
        "on_savingSearchButton_clicked",
        "on_savingClearButton_clicked",
        "onSavingSortChanged",
        "sortType",
        "on_saving_btn_clicked",
        "searchExpense",
        "keyword",
        "clearExpenseSearch",
        "searchIncome",
        "clearIncomeSearch",
        "showAddTransactionDialog",
        "type",
        "on_expenseExpandingButton_clicked",
        "on_incomeExpandingButton_clicked",
        "handleExpenseCardAction",
        "handleIncomeCardAction",
        "editIncomeTransaction",
        "incomeId",
        "editExpenseTransaction",
        "expenseId",
        "removeIncomeTransaction",
        "removeExpenseTransaction",
        "applyExpenseFiltersToMainList",
        "applyIncomeFiltersToMainList",
        "clearAllFilters",
        "applySortToMainList",
        "sortBy",
        "ascending"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'on_DashBoard_btn_2_clicked'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_expense_btn_2_clicked'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_income_btn_2_clicked'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_saving_btn_2_clicked'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_debt_btn_2_clicked'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_expenseSearchButton_clicked'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_expenseClearButton_clicked'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_incomeSearchButton_clicked'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_incomeClearButton_clicked'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_addExpenseButton_clicked'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_addIncomeButton_clicked'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_addSavingGoalBtn_clicked'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_depositBtn_clicked'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_withdrawBtn_clicked'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_savingReportBtn_clicked'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_savingSearchButton_clicked'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_savingClearButton_clicked'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSavingSortChanged'
        QtMocHelpers::SlotData<void(const QString &)>(19, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 20 },
        }}),
        // Slot 'on_saving_btn_clicked'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'searchExpense'
        QtMocHelpers::SlotData<void(const QString &)>(22, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 23 },
        }}),
        // Slot 'clearExpenseSearch'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'searchIncome'
        QtMocHelpers::SlotData<void(const QString &)>(25, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 23 },
        }}),
        // Slot 'clearIncomeSearch'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'showAddTransactionDialog'
        QtMocHelpers::SlotData<void(const QString &)>(27, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 28 },
        }}),
        // Slot 'on_expenseExpandingButton_clicked'
        QtMocHelpers::SlotData<void()>(29, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_incomeExpandingButton_clicked'
        QtMocHelpers::SlotData<void()>(30, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'handleExpenseCardAction'
        QtMocHelpers::SlotData<void()>(31, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'handleIncomeCardAction'
        QtMocHelpers::SlotData<void()>(32, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'editIncomeTransaction'
        QtMocHelpers::SlotData<void(const QString &)>(33, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 34 },
        }}),
        // Slot 'editExpenseTransaction'
        QtMocHelpers::SlotData<void(const QString &)>(35, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 36 },
        }}),
        // Slot 'removeIncomeTransaction'
        QtMocHelpers::SlotData<void(const QString &)>(37, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 34 },
        }}),
        // Slot 'removeExpenseTransaction'
        QtMocHelpers::SlotData<void(const QString &)>(38, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 36 },
        }}),
        // Slot 'applyExpenseFiltersToMainList'
        QtMocHelpers::SlotData<void()>(39, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'applyIncomeFiltersToMainList'
        QtMocHelpers::SlotData<void()>(40, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'clearAllFilters'
        QtMocHelpers::SlotData<void()>(41, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'applySortToMainList'
        QtMocHelpers::SlotData<void(const QString &, const QString &, bool)>(42, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 28 }, { QMetaType::QString, 43 }, { QMetaType::Bool, 44 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Dashboard, qt_meta_tag_ZN9DashboardE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Dashboard::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9DashboardE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9DashboardE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN9DashboardE_t>.metaTypes,
    nullptr
} };

void Dashboard::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Dashboard *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->on_DashBoard_btn_2_clicked(); break;
        case 1: _t->on_expense_btn_2_clicked(); break;
        case 2: _t->on_income_btn_2_clicked(); break;
        case 3: _t->on_saving_btn_2_clicked(); break;
        case 4: _t->on_debt_btn_2_clicked(); break;
        case 5: _t->on_expenseSearchButton_clicked(); break;
        case 6: _t->on_expenseClearButton_clicked(); break;
        case 7: _t->on_incomeSearchButton_clicked(); break;
        case 8: _t->on_incomeClearButton_clicked(); break;
        case 9: _t->on_addExpenseButton_clicked(); break;
        case 10: _t->on_addIncomeButton_clicked(); break;
        case 11: _t->on_addSavingGoalBtn_clicked(); break;
        case 12: _t->on_depositBtn_clicked(); break;
        case 13: _t->on_withdrawBtn_clicked(); break;
        case 14: _t->on_savingReportBtn_clicked(); break;
        case 15: _t->on_savingSearchButton_clicked(); break;
        case 16: _t->on_savingClearButton_clicked(); break;
        case 17: _t->onSavingSortChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 18: _t->on_saving_btn_clicked(); break;
        case 19: _t->searchExpense((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 20: _t->clearExpenseSearch(); break;
        case 21: _t->searchIncome((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 22: _t->clearIncomeSearch(); break;
        case 23: _t->showAddTransactionDialog((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 24: _t->on_expenseExpandingButton_clicked(); break;
        case 25: _t->on_incomeExpandingButton_clicked(); break;
        case 26: _t->handleExpenseCardAction(); break;
        case 27: _t->handleIncomeCardAction(); break;
        case 28: _t->editIncomeTransaction((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 29: _t->editExpenseTransaction((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 30: _t->removeIncomeTransaction((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 31: _t->removeExpenseTransaction((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 32: _t->applyExpenseFiltersToMainList(); break;
        case 33: _t->applyIncomeFiltersToMainList(); break;
        case 34: _t->clearAllFilters(); break;
        case 35: _t->applySortToMainList((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObject *Dashboard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Dashboard::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9DashboardE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int Dashboard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 36)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 36;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 36)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 36;
    }
    return _id;
}
QT_WARNING_POP
