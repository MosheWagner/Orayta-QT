/****************************************************************************
** Meta object code from reading C++ file 'bookfind.h'
**
** Created: Fri May 21 14:02:35 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../bookfind.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bookfind.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_bookfind[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      13,   10,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      27,    9,    9,    9, 0x08,
      51,    9,    9,    9, 0x08,
      85,   77,    9,    9, 0x08,
     116,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_bookfind[] = {
    "bookfind\0\0id\0openBook(int)\0"
    "on_pushButton_clicked()\0"
    "on_pushButton_2_clicked()\0checked\0"
    "on_radioButton_2_toggled(bool)\0"
    "on_lineEdit_textEdited(QString)\0"
};

const QMetaObject bookfind::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_bookfind,
      qt_meta_data_bookfind, 0 }
};

const QMetaObject *bookfind::metaObject() const
{
    return &staticMetaObject;
}

void *bookfind::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_bookfind))
        return static_cast<void*>(const_cast< bookfind*>(this));
    return QDialog::qt_metacast(_clname);
}

int bookfind::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: openBook((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: on_pushButton_clicked(); break;
        case 2: on_pushButton_2_clicked(); break;
        case 3: on_radioButton_2_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: on_lineEdit_textEdited((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void bookfind::openBook(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
