﻿#ifndef SAVELOG_H
#define SAVELOG_H

#include <QObject>
#include "qmutex.h"
#include "qdir.h"
#include "qfile.h"
#include "QTextStream"
#include "qdatetime.h"
#include "qcoreapplication.h"
#include "qtimer.h"
#include "qstringlist.h"

class QFile;

//消息类型
enum MsgType {
    MsgType_Debug = 0x0001,
    MsgType_Info = 0x0002,
    MsgType_Warning = 0x0004,
    MsgType_Critical = 0x0008,
    MsgType_Fatal = 0x0010,
};

#ifdef quc
class Q_DECL_EXPORT SaveLog : public QObject
#else
class SaveLog : public QObject
#endif

{
    Q_OBJECT
public:
    static SaveLog *Instance();
    explicit SaveLog(QObject *parent = 0);
    ~SaveLog();

private:
    static QScopedPointer<SaveLog> self;

    //是否在运行
    bool isRun;
    //文件最大行数 0表示不启用
    int maxRow, currentRow;
    //文件最大大小 0表示不启用 单位kb
    int maxSize;
    //是否输出日志上下文
    bool useContext;

    //文件对象
    QFile *file;
    //日志文件路径
    QString path;
    //日志文件名称
    QString name;
    //日志文件完整名称
    QString fileName;
    //消息类型
    MsgType msgType;

private:
    void openFile(const QString &fileName);

public:
    bool getUseContext();
    MsgType getMsgType();

Q_SIGNALS:
    //发送内容信号
    void send(const QString &content);

public Q_SLOTS:
    //启动日志服务
    void start();
    //暂停日志服务
    void stop();

    //清空状态
    void clear();
    //保存日志
    void save(const QString &content);

    //设置日志文件最大行数
    void setMaxRow(int maxRow);
    //设置日志文件最大大小 单位kb
    void setMaxSize(int maxSize);

    //设置是否输出日志上下文
    void setUseContext(bool useContext);

    //设置日志文件存放路径
    void setPath(const QString &path);
    //设置日志文件名称
    void setName(const QString &name);
    //设置消息类型
    void setMsgType(const MsgType &msgType);
    //设置消息级别
    void setMsgLevel(const MsgType &msgType);
};

#endif // SAVELOG_H
