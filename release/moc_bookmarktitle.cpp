/****************************************************************************
** Meta object code from reading C++ file 'bookmarktitle.h'
**
** Created: Fri May 21 14:02:23 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../bookmarktitle.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bookmarktitle.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BookMarkTitle[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      17,   15,   14,   14, 0x05,
      37,   15,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      59,   14,   14,   14, 0x08,
      75,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_BookMarkTitle[] = {
    "BookMarkTitle\0\0,\0OK(QString,QString)\0"
    "RenameOK(int,QString)\0on_ok_clicked()\0"
    "on_cancel_clicked()\0"
};

const QMetaObject BookMarkTitle::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_BookMarkTitle,
      qt_meta_data_BookMarkTitle, 0 }
};

const QMetaObject *BookMarkTitle::metaObject() const
{
    return &staticMetaObject;
}

void *BookMarkTitle::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BookMarkTitle))
        return static_cast<void*>(const_cast< BookMarkTitle*>(this));
    return QDialog::qt_metacast(_clname);
}

int BookMarkTitle::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: OK((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: RenameOK((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: on_ok_clicked(); break;
        case 3: on_cancel_clicked(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void BookMarkTitle::OK(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BookMarkTitle::RenameOK(int _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
