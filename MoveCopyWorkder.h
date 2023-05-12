#ifndef MOVECOPYWORKDER_H
#define MOVECOPYWORKDER_H

#include <QObject>
#include <QWidget>
#include <QMutex>
#include <QWaitCondition>
#include <QThread>
#include <QFileInfo>
#include <QFileInfoList>
#include <QDir>
#include "MakePath.h"
#include "MsgHighlight.h"
#include <QDebug>

class MoveCopyWorkder : public QThread
{
    Q_OBJECT
public:
    explicit MoveCopyWorkder(QString targetDirPath, QStringList fileList,
                             bool isMove, int typeFolder, int typeFile,
                             QWidget *parent = 0);
    ~MoveCopyWorkder();
    void stop();
    void resume();
    void pause();
    bool isPaused();

Q_SIGNALS:
    void statusChanged(QString text, int timeout = 0);
//    void msgChanged(QString text, int msgType);
    void msgChanged(HighlightType highlightType, QString text);

private:
    QString mTargetDirPath;
    QStringList mFileList;
    bool mIsMove;
    int mTypeFolder;
    int mTypeFile;

    QWaitCondition mPauseCondition;
    QMutex mMutex; //locked when accessing isPaused
    bool mIsPaused;

    void successMsg(QString text);
    void warningMsg(QString text);
    void errorMsg(QString text);
    void failMsg(QString text);

    void moveStarted();
    void copyStarted();

    void moveCopyFile(QFileInfo info, QString targetPath = QString());
    void fileExist(QString filePath, QString targetPath);
    void fileCover(QString filePath, QString targetPath);
    void fileSkip(QString filePath, QString targetPath);
    void fileRetain(QString filePath, QString targetPath);

    void moveCopyFolder(QFileInfo info);

    void recursiveFolder(QFileInfo info);

    void folderFilesByParentFolder(QString dirPath,
                                   QStringList &folderList, QStringList &fileList);

    QStringList listByFolderLength(QStringList folderList, QString dirPath);
    void removeEmptyDir(QString dirPath);

protected:
    void run() override;
};

#endif // MOVECOPYWORKDER_H
