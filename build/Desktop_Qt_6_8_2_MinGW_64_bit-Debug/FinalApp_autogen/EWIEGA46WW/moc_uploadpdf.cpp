/****************************************************************************
** Meta object code from reading C++ file 'uploadpdf.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../uploadpdf.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'uploadpdf.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.2. It"
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
struct qt_meta_tag_ZN9UploadPdfE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN9UploadPdfE = QtMocHelpers::stringData(
    "UploadPdf",
    "backButtonClicked",
    "",
    "selectFile",
    "resetScreen",
    "openPDF",
    "uploadFile",
    "insertFileMetadata",
    "fileUrl",
    "loadCourses"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN9UploadPdfE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   56,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       3,    0,   57,    2, 0x08,    2 /* Private */,
       4,    0,   58,    2, 0x08,    3 /* Private */,
       5,    0,   59,    2, 0x08,    4 /* Private */,
       6,    0,   60,    2, 0x08,    5 /* Private */,
       7,    1,   61,    2, 0x08,    6 /* Private */,
       9,    0,   64,    2, 0x08,    8 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject UploadPdf::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ZN9UploadPdfE.offsetsAndSizes,
    qt_meta_data_ZN9UploadPdfE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN9UploadPdfE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<UploadPdf, std::true_type>,
        // method 'backButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'selectFile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'resetScreen'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'openPDF'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'uploadFile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'insertFileMetadata'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'loadCourses'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void UploadPdf::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<UploadPdf *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->backButtonClicked(); break;
        case 1: _t->selectFile(); break;
        case 2: _t->resetScreen(); break;
        case 3: _t->openPDF(); break;
        case 4: _t->uploadFile(); break;
        case 5: _t->insertFileMetadata((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->loadCourses(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (UploadPdf::*)();
            if (_q_method_type _q_method = &UploadPdf::backButtonClicked; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *UploadPdf::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UploadPdf::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN9UploadPdfE.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int UploadPdf::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void UploadPdf::backButtonClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
