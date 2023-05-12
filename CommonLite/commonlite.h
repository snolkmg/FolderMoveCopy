#ifndef COMMONLITE
#define COMMONLITE

#include <qobject.h>
#include <QString>
#include <QStringList>
#include <QList>
#include <QVector>
#include <QFileDevice>
#include <QFile>
#include "cmath"

class CommonLite
{
public:
    CommonLite(){}

    static QString readFile(QString filePath) {
        QFile file(filePath);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QString allText = QString(file.readAll());
        file.close();
        return allText;
    }

    // 秒转*d *h *m *s
    static QString timeFormat(int seconds)
    {
        QString strValue;
        QString strSpacing(" ");
        if (seconds <= 0)
        {
            strValue = QString("%1 秒").arg(0);
        }
        else if (seconds < 60)
        {
            strValue = QString("%1 秒").arg(seconds);
        }
        else if (seconds < 60 * 60)
        {
            int nMinute = seconds / 60;
            int nSecond = seconds - nMinute * 60;

            strValue = QString("%1 分").arg(nMinute);

            if (nSecond > 0)
                strValue += strSpacing + QString("%1 秒").arg(nSecond);
        }
        else if (seconds < 60 * 60 * 24)
        {
            int nHour = seconds / (60 * 60);
            int nMinute = (seconds - nHour * 60 * 60) / 60;
            int nSecond = seconds - nHour * 60 * 60 - nMinute * 60;

            strValue = QString("%1 小时").arg(nHour);

            if (nMinute > 0)
                strValue += strSpacing + QString("%1 分").arg(nMinute);

            if (nSecond > 0)
                strValue += strSpacing + QString("%1 秒").arg(nSecond);
        }
        else
        {
            int nDay = seconds / (60 * 60 * 24);
            int nHour = (seconds - nDay * 60 * 60 * 24) / (60 * 60);
            int nMinute = (seconds - nDay * 60 * 60 * 24 - nHour * 60 * 60) / 60;
            int nSecond = seconds - nDay * 60 * 60 * 24 - nHour * 60 * 60 - nMinute * 60;

            strValue = QString("%1 天").arg(nDay);

            if (nHour > 0)
                strValue += strSpacing + QString("%1 小时").arg(nHour);

            if (nMinute > 0)
                strValue += strSpacing + QString("%1 分").arg(nMinute);

            if (nSecond > 0)
                strValue += strSpacing + QString("%1 秒").arg(nSecond);
        }

        return strValue;
    }

    // 毫秒转*d *h *m *s
    static QString timeFormat(qint64 msecs)
    {
        if(msecs < 0)
            msecs = std::abs(msecs);
        QString strValue;
        QString strSpacing(" ");
        double secs = msecs / 1000.0; // 计算 秒
        int seconds = std::round(secs); // 秒 四舍五入 取整
        if(msecs <= 100) {
            strValue = QString("%1 秒").arg(0);
        } else if (msecs <= 1000) {
            strValue = QString::number(secs, 'f', 2) + " 秒";
        } else if (msecs < 60 * 1000) {
            strValue = QString::number(secs, 'f', 2) + " 秒";
        } else if (msecs < 60 * 60 * 1000) {
            int nMinute = seconds / 60 ;
            int nSecond = seconds - nMinute * 60;

            strValue = QString("%1 分").arg(nMinute);

            if (nSecond > 0)
                strValue += strSpacing + QString("%1 秒").arg(nSecond);
        } else if (msecs < 60 * 60 * 24 * 1000) {
            int nHour = seconds / (60 * 60);
            int nMinute = (seconds - nHour * 60 * 60) / 60;
            int nSecond = seconds - nHour * 60 * 60 - nMinute * 60;

            strValue = QString("%1 小时").arg(nHour);

            if (nMinute > 0)
                strValue += strSpacing + QString("%1 分").arg(nMinute);

            if (nSecond > 0)
                strValue += strSpacing + QString("%1 秒").arg(nSecond);
        } else {
            int nDay = seconds / (60 * 60 * 24);
            int nHour = (seconds - nDay * 60 * 60 * 24) / (60 * 60);
            int nMinute = (seconds - nDay * 60 * 60 * 24 - nHour * 60 * 60) / 60;
            int nSecond = seconds - nDay * 60 * 60 * 24 - nHour * 60 * 60 - nMinute * 60;

            strValue = QString("%1 天").arg(nDay);

            if (nHour > 0)
                strValue += strSpacing + QString("%1 小时").arg(nHour);

            if (nMinute > 0)
                strValue += strSpacing + QString("%1 分").arg(nMinute);

            if (nSecond > 0)
                strValue += strSpacing + QString("%1 秒").arg(nSecond);
        }

        return strValue;
    }
};
#endif // COMMONLITE

