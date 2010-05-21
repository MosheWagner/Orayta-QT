/****************************************************************************
** Meta object code from reading C++ file 'errorreport.h'
**
** Created: Fri May 21 14:02:30 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../errorreport.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'errorreport.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_errorReport[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,
      33,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_errorReport[] = {
    "errorReport\0\0on_accept_clicked()\0"
    "on_cancel_clicked()\0"
};

const QMetaObject errorReport::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_errorReport,
      qt_meta_data_errorReport, 0 }
};

const QMetaObject *errorReport::metaObject() const
{
    return &staticMetaObject;
}

void *errorReport::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_errorReport))
        return static_cast<void*>(const_cast< errorReport*>(this));
    return QDialog::qt_metacast(_clname);
}

int errorReport::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_accept_clicked(); break;
        case 1: on_cancel_clicked(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
