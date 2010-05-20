/****************************************************************************
** Meta object code from reading C++ file 'mywebview.h'
**
** Created: Thu May 20 14:12:17 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mywebview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mywebview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_myWebView[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      36,   29,   10,   10, 0x0a,
      71,   65,   10,   10, 0x08,
     101,   65,   10,   10, 0x08,
     142,  133,   10,   10, 0x08,
     168,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_myWebView[] = {
    "myWebView\0\0LinkClicked(QUrl)\0factor\0"
    "setTextSizeMultiplier(qreal)\0event\0"
    "mousePressEvent(QMouseEvent*)\0"
    "mouseReleaseEvent(QMouseEvent*)\0"
    "keyEvent\0keyPressEvent(QKeyEvent*)\0"
    "rememberActiveLink(QString)\0"
};

const QMetaObject myWebView::staticMetaObject = {
    { &QWebView::staticMetaObject, qt_meta_stringdata_myWebView,
      qt_meta_data_myWebView, 0 }
};

const QMetaObject *myWebView::metaObject() const
{
    return &staticMetaObject;
}

void *myWebView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_myWebView))
        return static_cast<void*>(const_cast< myWebView*>(this));
    return QWebView::qt_metacast(_clname);
}

int myWebView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWebView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: LinkClicked((*reinterpret_cast< QUrl(*)>(_a[1]))); break;
        case 1: setTextSizeMultiplier((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 2: mousePressEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 3: mouseReleaseEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 4: keyPressEvent((*reinterpret_cast< QKeyEvent*(*)>(_a[1]))); break;
        case 5: rememberActiveLink((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void myWebView::LinkClicked(QUrl _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
