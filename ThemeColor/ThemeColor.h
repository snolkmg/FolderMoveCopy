#ifndef THEMECOLOR_H
#define THEMECOLOR_H

#include <QObject>
#include <QColor>
#include <QDebug>
#include "dwmapi.h"

class ThemeColor : public QObject
{
    Q_OBJECT
public:
    explicit ThemeColor(QObject *parent = 0);
    static QColor windowsThemeColor();
    static QString windowsThemeName(bool alpha = true);
};

#endif // THEMECOLOR_H
