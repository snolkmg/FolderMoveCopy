#include "MsgHighlight.h"

MsgHighlight::MsgHighlight(QPlainTextEdit *parent) :
    QObject(parent)
{
    mPlainTextEdit = parent;
    highlighter = new Highlighter(mPlainTextEdit->document());
}

MsgHighlight::~MsgHighlight()
{

}

QString MsgHighlight::prefix(HighlightType type)
{
    QString text;
    switch (type) {
    case HighlightType::Success:
        text = QString("成功：");
        break;
    case HighlightType::Info:
        text = QString("信息：");
        break;
    case HighlightType::Tip:
        text = QString("提示：");
        break;
    case HighlightType::Warning:
        text = QString("警告：");
        break;
    case HighlightType::Error:
        text = QString("错误：");
        break;
    case HighlightType::Fail:
        text = QString("失败：");
        break;
    default:
        break;
    }
    return text;
}

QStringList MsgHighlight::intToStringList(QList<qint64> values)
{
    QStringList list;
    for(auto value : values)
        list << QString::number(value);
    return list;
}

//!====================================appendMsg=======================================
void MsgHighlight::appendMsg(HighlightType type, QString text)
{
    mPlainTextEdit->appendPlainText(prefix(type) + text);
}

void MsgHighlight::appendMsg(HighlightType type, QStringList textList, QString hyphen)
{
    mPlainTextEdit->appendPlainText(prefix(type) + textList.join(hyphen));
}

void MsgHighlight::appendMsg(HighlightType type, qint64 value)
{
    mPlainTextEdit->appendPlainText(prefix(type) + QString::number(value));
}

void MsgHighlight::appendMsg(HighlightType type, QString text, qint64 value, QString hyphen)
{
    mPlainTextEdit->appendPlainText(prefix(type) + text + hyphen + QString::number(value));
}

void MsgHighlight::appendMsg(HighlightType type, QList<qint64> values, QString hyphen)
{
    mPlainTextEdit->appendPlainText(prefix(type) + intToStringList(values).join(hyphen));
}

//!====================================normal=======================================
void MsgHighlight::normal(QString text)
{
    appendMsg(HighlightType::Normal, text);
}

void MsgHighlight::normal(QStringList textList)
{
    foreach (QString text, textList)
        appendMsg(HighlightType::Normal, text);
}

void MsgHighlight::normal(QStringList textList, QString hyphen)
{
    appendMsg(HighlightType::Normal, textList.join(hyphen));
}

void MsgHighlight::normal(qint64 value)
{
    appendMsg(HighlightType::Normal, QString::number(value));
}

void MsgHighlight::normal(QString text, qint64 value, QString hyphen)
{
    appendMsg(HighlightType::Normal, text + hyphen + QString::number(value));
}

void MsgHighlight::normal(QList<qint64> values, QString hyphen)
{
    appendMsg(HighlightType::Normal, intToStringList(values).join(hyphen));
}

//!====================================success=======================================
void MsgHighlight::success(QString text)
{
    appendMsg(HighlightType::Success, text);
}

void MsgHighlight::success(QStringList textList)
{
    foreach (QString text, textList)
        appendMsg(HighlightType::Success, text);
}

void MsgHighlight::success(QStringList textList, QString hyphen)
{
    appendMsg(HighlightType::Success, textList.join(hyphen));
}

void MsgHighlight::success(qint64 value)
{
    appendMsg(HighlightType::Success, QString::number(value));
}

void MsgHighlight::success(QString text, qint64 value, QString hyphen)
{
    appendMsg(HighlightType::Success, text + hyphen + QString::number(value));
}

void MsgHighlight::success(QList<qint64> values, QString hyphen)
{
    appendMsg(HighlightType::Success, intToStringList(values).join(hyphen));
}

//!====================================info=======================================
void MsgHighlight::info(QString text)
{
    appendMsg(HighlightType::Info, text);
}

void MsgHighlight::info(QStringList textList)
{
    foreach (QString text, textList)
        appendMsg(HighlightType::Info, text);
}

void MsgHighlight::info(QStringList textList, QString hyphen)
{
    appendMsg(HighlightType::Info, textList.join(hyphen));
}

void MsgHighlight::info(qint64 value)
{
    appendMsg(HighlightType::Info, QString::number(value));
}

void MsgHighlight::info(QString text, qint64 value, QString hyphen)
{
    appendMsg(HighlightType::Info, text + hyphen + QString::number(value));
}

void MsgHighlight::info(QList<qint64> values, QString hyphen)
{
    appendMsg(HighlightType::Info, intToStringList(values).join(hyphen));
}

//!====================================tip=======================================
void MsgHighlight::tip(QString text)
{
    appendMsg(HighlightType::Tip, text);
}

void MsgHighlight::tip(QStringList textList)
{
    foreach (QString text, textList)
        appendMsg(HighlightType::Tip, text);
}

void MsgHighlight::tip(QStringList textList, QString hyphen)
{
    appendMsg(HighlightType::Tip, textList.join(hyphen));
}

void MsgHighlight::tip(qint64 value)
{
    appendMsg(HighlightType::Tip, QString::number(value));
}

void MsgHighlight::tip(QString text, qint64 value, QString hyphen)
{
    appendMsg(HighlightType::Tip, text + hyphen + QString::number(value));
}

void MsgHighlight::tip(QList<qint64> values, QString hyphen)
{
    appendMsg(HighlightType::Tip, intToStringList(values).join(hyphen));
}

//!====================================warning=======================================
void MsgHighlight::warning(QString text)
{
    appendMsg(HighlightType::Warning, text);
    qWarning() << "警告" << text;
}

void MsgHighlight::warning(QStringList textList)
{
    foreach (QString text, textList)
        appendMsg(HighlightType::Warning, text);
    qWarning() << "警告" << textList;
}

void MsgHighlight::warning(QStringList textList, QString hyphen)
{
    appendMsg(HighlightType::Warning, textList.join(hyphen));
    qWarning() << "警告" << textList.join(hyphen);
}

void MsgHighlight::warning(qint64 value)
{
    appendMsg(HighlightType::Warning, QString::number(value));
    qWarning() << "警告" << QString::number(value);
}

void MsgHighlight::warning(QString text, qint64 value, QString hyphen)
{
    appendMsg(HighlightType::Warning, text + hyphen + QString::number(value));
    qWarning() << "警告" << text + hyphen + QString::number(value);
}

void MsgHighlight::warning(QList<qint64> values, QString hyphen)
{
    appendMsg(HighlightType::Warning, intToStringList(values).join(hyphen));
    qWarning() << "警告" << intToStringList(values).join(hyphen);
}

//!====================================error=======================================
void MsgHighlight::error(QString text)
{
    appendMsg(HighlightType::Error, text);
    qCritical() << "错误" << text;
}

void MsgHighlight::error(QStringList textList)
{
    foreach (QString text, textList)
        appendMsg(HighlightType::Error, text);
    qCritical() << "错误" << textList;
}

void MsgHighlight::error(QStringList textList, QString hyphen)
{
    appendMsg(HighlightType::Error, textList.join(hyphen));
    qCritical() << "错误" << textList.join(hyphen);
}

void MsgHighlight::error(qint64 value)
{
    appendMsg(HighlightType::Error, QString::number(value));
    qCritical() << "错误" << QString::number(value);
}

void MsgHighlight::error(QString text, qint64 value, QString hyphen)
{
    appendMsg(HighlightType::Error, text + hyphen + QString::number(value));
    qCritical() << "错误" << text + hyphen + QString::number(value);
}

void MsgHighlight::error(QList<qint64> values, QString hyphen)
{
    appendMsg(HighlightType::Error, intToStringList(values).join(hyphen));
    qCritical() << "错误" << intToStringList(values).join(hyphen);
}

//!====================================fail=======================================
void MsgHighlight::fail(QString text)
{
    appendMsg(HighlightType::Fail, text);
    qCritical() << "失败" << text;
}

void MsgHighlight::fail(QStringList textList)
{
    foreach (QString text, textList)
        appendMsg(HighlightType::Fail, text);
    qCritical() << "失败" << textList;
}

void MsgHighlight::fail(QStringList textList, QString hyphen)
{
    appendMsg(HighlightType::Fail, textList.join(hyphen));
    qCritical() << "失败" << textList.join(hyphen);
}

void MsgHighlight::fail(qint64 value)
{
    appendMsg(HighlightType::Fail, QString::number(value));
    qCritical() << "失败" << QString::number(value);
}

void MsgHighlight::fail(QString text, qint64 value, QString hyphen)
{
    appendMsg(HighlightType::Fail, text + hyphen + QString::number(value));
    qCritical() << "失败" << text + hyphen + QString::number(value);
}

void MsgHighlight::fail(QList<qint64> values, QString hyphen)
{
    appendMsg(HighlightType::Fail, intToStringList(values).join(hyphen));
    qCritical() << "失败" << intToStringList(values).join(hyphen);
}
