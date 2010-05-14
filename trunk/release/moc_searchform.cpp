/****************************************************************************
** Meta object code from reading C++ file 'searchform.h'
**
** Created: Fri May 14 11:32:15 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../searchform.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'searchform.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SearchForm[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      14,   12,   11,   11, 0x05,
      38,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      62,   54,   11,   11, 0x08,
      94,   11,   11,   11, 0x08,
     114,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SearchForm[] = {
    "SearchForm\0\0,\0Search(QRegExp,QString)\0"
    "Search(QString)\0checked\0"
    "on_regexpCheckBox_clicked(bool)\0"
    "on_cancel_clicked()\0on_submit_clicked()\0"
};

const QMetaObject SearchForm::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SearchForm,
      qt_meta_data_SearchForm, 0 }
};

const QMetaObject *SearchForm::metaObject() const
{
    return &staticMetaObject;
}

void *SearchForm::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SearchForm))
        return static_cast<void*>(const_cast< SearchForm*>(this));
    return QDialog::qt_metacast(_clname);
}

int SearchForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: Search((*reinterpret_cast< QRegExp(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: Search((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: on_regexpCheckBox_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: on_cancel_clicked(); break;
        case 4: on_submit_clicked(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void SearchForm::Search(QRegExp _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SearchForm::Search(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
