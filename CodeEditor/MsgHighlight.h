#ifndef MSGHIGHLIGHT_H
#define MSGHIGHLIGHT_H

#include <QObject>
#include <QPlainTextEdit>
#include "highlighter.h"
#include <QDebug>

enum HighlightType
{
    Normal = 0,
    Success = 1,
    Info = 2,
    Tip = 3,
    Warning = 4,
    Error = 5,
    Fail = 6
};
//Q_ENUM(HighlightType)		//如不使用HighlightType，可省略
//Q_DECLARE_FLAGS(HighlightTypeFlags, HighlightType)
//Q_FLAG(HighlightTypeFlags)

class MsgHighlight : public QObject
{
    Q_OBJECT
public:
    explicit MsgHighlight(QPlainTextEdit *parent);
    ~MsgHighlight();

    void appendMsg(HighlightType type, QString text);
    void appendMsg(HighlightType type, QStringList textList, QString hyphen = QString());
    void appendMsg(HighlightType type, qint64 value);
    void appendMsg(HighlightType type, QString text, qint64 value, QString hyphen = QString());
    void appendMsg(HighlightType type, QList<qint64> values, QString hyphen = QString());

    void normal(QString text);
    void normal(QStringList textList);
    void normal(QStringList textList, QString hyphen);
    void normal(qint64 value);
    void normal(QString text, qint64 value, QString hyphen = QString());
    void normal(QList<qint64> values, QString hyphen = QString());

    void success(QString text);
    void success(QStringList textList);
    void success(QStringList textList, QString hyphen);
    void success(qint64 value);
    void success(QString text, qint64 value, QString hyphen = QString());
    void success(QList<qint64> values, QString hyphen = QString());

    void info(QString text);
    void info(QStringList textList);
    void info(QStringList textList, QString hyphen);
    void info(qint64 value);
    void info(QString text, qint64 value, QString hyphen = QString());
    void info(QList<qint64> values, QString hyphen = QString());

    void tip(QString text);
    void tip(QStringList textList);
    void tip(QStringList textList, QString hyphen);
    void tip(qint64 value);
    void tip(QString text, qint64 value, QString hyphen = QString());
    void tip(QList<qint64> values, QString hyphen = QString());

    void warning(QString text);
    void warning(QStringList textList);
    void warning(QStringList textList, QString hyphen);
    void warning(qint64 value);
    void warning(QString text, qint64 value, QString hyphen = QString());
    void warning(QList<qint64> values, QString hyphen = QString());

    void error(QString text);
    void error(QStringList textList);
    void error(QStringList textList, QString hyphen);
    void error(qint64 value);
    void error(QString text, qint64 value, QString hyphen = QString());
    void error(QList<qint64> values, QString hyphen = QString());

    void fail(QString text);
    void fail(QStringList textList);
    void fail(QStringList textList, QString hyphen);
    void fail(qint64 value);
    void fail(QString text, qint64 value, QString hyphen = QString());
    void fail(QList<qint64> values, QString hyphen = QString());

private:
    QPlainTextEdit *mPlainTextEdit;
    Highlighter *highlighter;
    QString prefix(HighlightType type);
    QStringList intToStringList(QList<qint64> values);
};

#endif // MSGHIGHLIGHT_H
