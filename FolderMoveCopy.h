#ifndef FOLDERMOVECOPY_H
#define FOLDERMOVECOPY_H

#include <QMainWindow>
#include "ThemeColor.h"
#include "CFileDialog.h"
#include "logs.h"
#include "commonlite.h"
#include "MakePath.h"
#include "MoveCopyWorkder.h"
#include <QScopedPointer>
#include <QCloseEvent>
#include <QMessageBox>
#include <QActionGroup>
#include <QAction>
#include <QMenuBar>
#include <QLocale>
#include <QDate>
#include <QMimeData>
#include <QClipboard>
#include <QElapsedTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class FolderMoveCopy; }
QT_END_NAMESPACE

class FolderMoveCopy : public QMainWindow
{
    Q_OBJECT

public:
    FolderMoveCopy(QWidget *parent = nullptr);
    ~FolderMoveCopy();

private Q_SLOTS:
    void onMoveCopyTriggered(QAction *act);
    void onFolderExist(QAction *act);
    void onFileExist(QAction *act);

    void onFolderIndexChanged(int index);
    void onFileIndexChanged(int index);

    void onOriginFolder();
    void onTargetFolder();

    void onOriginTxt();
    void onOriginClipboard();

    void onChecked();

    void onMoveCopy();

    void onClear();

    void onAbout();
    void onAboutQt();

    void onMsgChanged(HighlightType highlightType, QString text);
    void onThreadFinished();

private:
    Ui::FolderMoveCopy *ui;
    QStringList mFolders;
    QStringList mFiles;
    void initWidget();
    void loadQss();
    void initMenuBar();
    QActionGroup *folderGp;
    QActionGroup *fileGp;
    void setActionsTip();
    QAction *lockToolBarAct;

    MsgHighlight *msgHighlight;
    QString lastPath;
    QStringList mSelectFiles;
    void setOriginName(QStringList fileList);
    void loadFromText(QString text);
    void timeConsuming(int msecs);

    void setThreadWorker();
    QScopedPointer< MoveCopyWorkder > mWorker;

    QElapsedTimer mTimeUp;

protected:
    void closeEvent(QCloseEvent *event);
};
#endif // FOLDERMOVECOPY_H
