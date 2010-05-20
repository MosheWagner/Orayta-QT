/****************************************************************************
** Meta object code from reading C++ file 'khtmlwidget.h'
**
** Created: Thu May 20 14:12:26 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../khtmlwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'khtmlwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_KHTMLWidget[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      37,   31,   12,   12, 0x08,
      83,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_KHTMLWidget[] = {
    "KHTMLWidget\0\0LinkClicked(QUrl)\0event\0"
    "khtmlMousePressEvent(khtml::MousePressEvent*)\0"
    "copyText()\0"
};

const QMetaObject KHTMLWidget::staticMetaObject = {
    { &KHTMLPart::staticMetaObject, qt_meta_stringdata_KHTMLWidget,
      qt_meta_data_KHTMLWidget, 0 }
};

const QMetaObject *KHTMLWidget::metaObject() const
{
    return &staticMetaObject;
}

void *KHTMLWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KHTMLWidget))
        return static_cast<void*>(const_cast< KHTMLWidget*>(this));
    return KHTMLPart::qt_metacast(_clname);
}

int KHTMLWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = KHTMLPart::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: LinkClicked((*reinterpret_cast< QUrl(*)>(_a[1]))); break;
        case 1: khtmlMousePressEvent((*reinterpret_cast< khtml::MousePressEvent*(*)>(_a[1]))); break;
        case 2: copyText(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void KHTMLWidget::LinkClicked(QUrl _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
