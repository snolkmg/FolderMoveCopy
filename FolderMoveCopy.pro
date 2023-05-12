QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

CONFIG += utf8_source

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    MoveCopyWorkder.cpp \
    main.cpp \
    FolderMoveCopy.cpp

HEADERS += \
    FolderMoveCopy.h \
    MoveCopyWorkder.h

FORMS += \
    FolderMoveCopy.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

include($$PWD/SingleApplication/SingleApplication.pri)
DEFINES += QAPPLICATION_CLASS=QApplication
win32 {
    msvc:LIBS += User32.lib
    gcc:LIBS += -luser32
}

include($$PWD/ThemeColor/ThemeColor.pri)
include($$PWD/CFileDialog/CFileDialog.pri)
include($$PWD/CodeEditor/CodeEditor.pri)
include($$PWD/CommonLite/CommonLite.pri)
include($$PWD/MakePath/MakePath.pri)
include($$PWD/SaveLog/SaveLog.pri)

RC_ICONS += icons/logo.ico
