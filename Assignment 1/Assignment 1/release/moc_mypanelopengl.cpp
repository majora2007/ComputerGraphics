/****************************************************************************
** Meta object code from reading C++ file 'mypanelopengl.h'
**
** Created: Mon Sep 10 16:33:32 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mypanelopengl.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mypanelopengl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MyPanelOpenGL[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   15,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      41,   39,   14,   14, 0x0a,
      73,   71,   14,   14, 0x0a,
     104,  102,   14,   14, 0x0a,
     133,  131,   14,   14, 0x0a,
     164,  160,   14,   14, 0x0a,
     191,  185,   14,   14, 0x0a,
     227,  221,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MyPanelOpenGL[] = {
    "MyPanelOpenGL\0\0m\0drawModeChanged(Mode)\0"
    "h\0changeRectangleHeight(double)\0w\0"
    "changeRectangleWidth(double)\0r\0"
    "changeCircleRadius(double)\0c\0"
    "changeCircleVerticies(int)\0str\0"
    "setDrawMode(QString)\0event\0"
    "mousePressEvent(QMouseEvent*)\0index\0"
    "setCursorTriangle(int)\0"
};

void MyPanelOpenGL::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MyPanelOpenGL *_t = static_cast<MyPanelOpenGL *>(_o);
        switch (_id) {
        case 0: _t->drawModeChanged((*reinterpret_cast< Mode(*)>(_a[1]))); break;
        case 1: _t->changeRectangleHeight((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->changeRectangleWidth((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->changeCircleRadius((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->changeCircleVerticies((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->setDrawMode((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->mousePressEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 7: _t->setCursorTriangle((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MyPanelOpenGL::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MyPanelOpenGL::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_MyPanelOpenGL,
      qt_meta_data_MyPanelOpenGL, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MyPanelOpenGL::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MyPanelOpenGL::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MyPanelOpenGL::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MyPanelOpenGL))
        return static_cast<void*>(const_cast< MyPanelOpenGL*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int MyPanelOpenGL::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void MyPanelOpenGL::drawModeChanged(Mode _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
