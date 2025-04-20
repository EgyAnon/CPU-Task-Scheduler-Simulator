/****************************************************************************
** Meta object code from reading C++ file 'simulator.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../OS - Project/CPU-Task-Scheduler-Simulator/simulator.hpp"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'simulator.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.0. It"
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
struct qt_meta_tag_ZN9SimulatorE_t {};
} // unnamed namespace

template <> constexpr inline auto Simulator::qt_create_metaobjectdata<qt_meta_tag_ZN9SimulatorE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Simulator",
        "signal_processBurstTimeUpdated",
        "",
        "size_t",
        "pid",
        "newTime",
        "signal_simulationFinished",
        "std::tuple<double,double,double>",
        "result",
        "signal_updateSimulationTime",
        "signal_LiveGanttChart",
        "std::tuple<size_t,size_t,size_t>",
        "log"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'signal_processBurstTimeUpdated'
        QtMocHelpers::SignalData<void(size_t, double)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 }, { QMetaType::Double, 5 },
        }}),
        // Signal 'signal_simulationFinished'
        QtMocHelpers::SignalData<void(std::tuple<double,double,double>)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 7, 8 },
        }}),
        // Signal 'signal_updateSimulationTime'
        QtMocHelpers::SignalData<void(double)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 2 },
        }}),
        // Signal 'signal_LiveGanttChart'
        QtMocHelpers::SignalData<void(std::tuple<size_t,size_t,size_t>)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 11, 12 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Simulator, qt_meta_tag_ZN9SimulatorE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Simulator::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9SimulatorE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9SimulatorE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN9SimulatorE_t>.metaTypes,
    nullptr
} };

void Simulator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Simulator *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->signal_processBurstTimeUpdated((*reinterpret_cast< std::add_pointer_t<size_t>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2]))); break;
        case 1: _t->signal_simulationFinished((*reinterpret_cast< std::add_pointer_t<std::tuple<double,double,double>>>(_a[1]))); break;
        case 2: _t->signal_updateSimulationTime((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 3: _t->signal_LiveGanttChart((*reinterpret_cast< std::add_pointer_t<std::tuple<size_t,size_t,size_t>>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Simulator::*)(size_t , double )>(_a, &Simulator::signal_processBurstTimeUpdated, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Simulator::*)(std::tuple<double,double,double> )>(_a, &Simulator::signal_simulationFinished, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Simulator::*)(double )>(_a, &Simulator::signal_updateSimulationTime, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (Simulator::*)(std::tuple<size_t,size_t,size_t> )>(_a, &Simulator::signal_LiveGanttChart, 3))
            return;
    }
}

const QMetaObject *Simulator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Simulator::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9SimulatorE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Simulator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Simulator::signal_processBurstTimeUpdated(size_t _t1, double _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}

// SIGNAL 1
void Simulator::signal_simulationFinished(std::tuple<double,double,double> _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void Simulator::signal_updateSimulationTime(double _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void Simulator::signal_LiveGanttChart(std::tuple<size_t,size_t,size_t> _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}
QT_WARNING_POP
