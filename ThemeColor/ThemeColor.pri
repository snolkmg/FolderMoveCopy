win32 {
    msvc:LIBS += dwmapi.lib
    gcc:LIBS += -ldwmapi
}

INCLUDEPATH += ThemeColor

SOURCES += $$PWD/ThemeColor.cpp

HEADERS += $$PWD/ThemeColor.h
