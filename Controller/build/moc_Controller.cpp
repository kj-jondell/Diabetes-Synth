/****************************************************************************
** Meta object code from reading C++ file 'Controller.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../src/Controller.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Controller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SynthController_t {
    QByteArrayData data[13];
    char stringdata0[134];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SynthController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SynthController_t qt_meta_stringdata_SynthController = {
    {
QT_MOC_LITERAL(0, 0, 15), // "SynthController"
QT_MOC_LITERAL(1, 16, 8), // "fileOpen"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 10), // "sendNoteOn"
QT_MOC_LITERAL(4, 37, 11), // "sendNoteOff"
QT_MOC_LITERAL(5, 49, 7), // "parseCC"
QT_MOC_LITERAL(6, 57, 12), // "startScSynth"
QT_MOC_LITERAL(7, 70, 12), // "valueChanged"
QT_MOC_LITERAL(8, 83, 3), // "idx"
QT_MOC_LITERAL(9, 87, 12), // "changeTuning"
QT_MOC_LITERAL(10, 100, 7), // "newText"
QT_MOC_LITERAL(11, 108, 13), // "openConverter"
QT_MOC_LITERAL(12, 122, 11) // "openProject"

    },
    "SynthController\0fileOpen\0\0sendNoteOn\0"
    "sendNoteOff\0parseCC\0startScSynth\0"
    "valueChanged\0idx\0changeTuning\0newText\0"
    "openConverter\0openProject"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SynthController[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x0a /* Public */,
       3,    2,   62,    2, 0x0a /* Public */,
       4,    2,   67,    2, 0x0a /* Public */,
       5,    2,   72,    2, 0x0a /* Public */,
       6,    0,   77,    2, 0x08 /* Private */,
       7,    1,   78,    2, 0x08 /* Private */,
       9,    1,   81,    2, 0x08 /* Private */,
      11,    0,   84,    2, 0x08 /* Private */,
      12,    0,   85,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SynthController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SynthController *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->fileOpen((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->sendNoteOn((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->sendNoteOff((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->parseCC((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->startScSynth(); break;
        case 5: _t->valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->changeTuning((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->openConverter(); break;
        case 8: _t->openProject(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SynthController::staticMetaObject = { {
    QMetaObject::SuperData::link<Controller::staticMetaObject>(),
    qt_meta_stringdata_SynthController.data,
    qt_meta_data_SynthController,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SynthController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SynthController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SynthController.stringdata0))
        return static_cast<void*>(this);
    return Controller::qt_metacast(_clname);
}

int SynthController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Controller::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
