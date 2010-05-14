/****************************************************************************
** Meta object code from reading C++ file 'addcomment.h'
**
** Created: Fri May 7 12:46:39 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "addcomment.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'addcomment.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AddComment[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      14,   12,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      46,   11,   11,   11, 0x08,
      65,   11,   11,   11, 0x08,
      85,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_AddComment[] = {
    "AddComment\0\0,\0commentWritten(QString,QString)\0"
    "on_erase_clicked()\0on_cancel_clicked()\0"
    "on_ok_clicked()\0"
};

const QMetaObject AddComment::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_AddComment,
      qt_meta_data_AddComment, 0 }
};

const QMetaObject *AddComment::metaObject() const
{
    return &staticMetaObject;
}

void *AddComment::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AddComment))
        return static_cast<void*>(const_cast< AddComment*>(this));
    return QDialog::qt_metacast(_clname);
}

int AddComment::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: commentWritten((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: on_erase_clicked(); break;
        case 2: on_cancel_clicked(); break;
        case 3: on_ok_clicked(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void AddComment::commentWritten(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
