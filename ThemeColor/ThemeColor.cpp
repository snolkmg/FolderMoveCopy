#include "ThemeColor.h"

ThemeColor::ThemeColor(QObject *parent) :
    QObject(parent)
{

}

// 获取当前系统主题色
QColor ThemeColor::windowsThemeColor()
{
    DWORD crColorization;
    BOOL fOpaqueBlend;
    QColor color;
    HRESULT result = DwmGetColorizationColor(&crColorization, &fOpaqueBlend);
    if (result == S_OK)
    {
        int r, g, b, a;
        a = (crColorization >> 24) % 256;
        r = (crColorization >> 16) % 256;
        g = (crColorization >> 8) % 256;
        b = crColorization % 256;
        color.setRgba(qRgba(r, g, b, a));
    }
    return color;
}

QString ThemeColor::windowsThemeName(bool alpha)
{
    QColor color = windowsThemeColor();
    QString name = alpha ? color.name(QColor::HexArgb)
                         : color.name(QColor::HexRgb);
    return name;
}
