#include "MoveCopyWorkder.h"

MoveCopyWorkder::MoveCopyWorkder(QString targetDirPath, QStringList fileList,
                                 bool isMove, int typeFolder, int typeFile,
                                 QWidget *parent) :
    QThread(parent),
    mTargetDirPath(targetDirPath),
    mFileList(fileList),
    mIsMove(isMove),
    mTypeFolder(typeFolder),
    mTypeFile(typeFile),
    mIsPaused( false )
{

}

MoveCopyWorkder::~MoveCopyWorkder()
{

}

/**
  * @brief stop 终止线程
 */
void MoveCopyWorkder::stop()
{
    disconnect();
    requestInterruption();
    wait();
}

/**
  * @brief resume 恢复线程
 */
void MoveCopyWorkder::resume()
{
    mMutex.lock();
    mIsPaused = false;
    mMutex.unlock();
    mPauseCondition.wakeAll();
}

/**
  * @brief pause 暂停线程
 */
void MoveCopyWorkder::pause()
{
    QMutexLocker locker( &mMutex );
    mIsPaused = true;
}

/**
  * @brief isPaused 线程是否暂停
 */
bool MoveCopyWorkder::isPaused()
{
    QMutexLocker locker( &mMutex );
    return mIsPaused;
}

/**
  * @brief successMsg 成功 消息提示
  * @param text 消息
 */
void MoveCopyWorkder::successMsg(QString text)
{
    emit msgChanged(HighlightType::Success, text);
}

/**
  * @brief warningMsg 警告 消息提示
  * @param text 消息
 */
void MoveCopyWorkder::warningMsg(QString text)
{
    emit msgChanged(HighlightType::Warning, text);
}

/**
  * @brief errorMsg 错误 消息提示
  * @param text 消息
 */
void MoveCopyWorkder::errorMsg(QString text)
{
    emit msgChanged(HighlightType::Error, text);
}

/**
  * @brief failMsg 失败 消息提示
  * @param text 消息
 */
void MoveCopyWorkder::failMsg(QString text)
{
    emit msgChanged(HighlightType::Fail, text);
}

/**
  * @brief run 运行线程
 */
void MoveCopyWorkder::run()
{
    QDir targetDir(mTargetDirPath);
    if(!targetDir.exists()) {
        errorMsg("目标文件夹不存在：" + mTargetDirPath);
        return;
    }
    moveStarted();
}

/**
  * @brief moveStarted 复制/移动 启动
 */
void MoveCopyWorkder::moveStarted()
{
    qDebug() << "MoveCopyWorkder start:" << QThread::currentThreadId();

    foreach (QString path, mFileList) {
        QFileInfo info(path);
        if(info.isFile())
            moveCopyFile(info);
        else if(info.isDir())
            moveCopyFolder(info);
    }
}

/**
  * @brief moveCopyFile 移动/复制文件
  * @param info 源文件 QFileInfo
  * @param targetPath 目标文件
 */
void MoveCopyWorkder::moveCopyFile(QFileInfo info, QString targetPath)
{
    QString filePath = info.filePath();
    emit statusChanged(QString("正在%1文件 %2 ……").arg(mIsMove ? "移动" : "复制").arg(filePath));
    QString fileName = info.fileName();
    if(targetPath.isEmpty()) {
        QDir targetDir(mTargetDirPath);
        targetPath = targetDir.absoluteFilePath(fileName);
    }
    if(targetPath == filePath) {
        warningMsg(QString("目标文件 %1 与源文件 %2 相同").arg(targetPath).arg(filePath));
        return;
    }

    if(QFile::exists(targetPath)) {
        fileExist(filePath, targetPath);
    } else {
        if(mIsMove) {
            if(!QFile::rename(filePath, targetPath)) {
                failMsg(QString("文件 %1 移动 %2").arg(filePath).arg(targetPath));
            }
        } else {
            if(!QFile::copy(filePath, targetPath)) {
                failMsg(QString("文件 %1 移动 %2").arg(filePath).arg(targetPath));
            }
        }
    }
}

/**
  * @brief fileExist 若文件存在
  * @param filePath 源文件
  * @param targetPath 目标文件
 */
void MoveCopyWorkder::fileExist(QString filePath, QString targetPath)
{
    switch (mTypeFile) {
    case 0: // 覆盖
        fileCover(filePath, targetPath);
        break;
    case 1: // 跳过
        fileSkip(filePath, targetPath);
        break;
    case 2: // 保留（编号）
        fileRetain(filePath, targetPath);
        break;
    default:
        break;
    }
}

/**
  * @brief fileCover 文件覆盖
  * @param filePath 源文件
  * @param targetPath 目标文件
 */
void MoveCopyWorkder::fileCover(QString filePath, QString targetPath)
{
    if(QFile::remove(targetPath)) {
        if(mIsMove) {
            if(!QFile::rename(filePath, targetPath)) {
                failMsg(QString("覆盖文件 %1").arg(targetPath));
            } else{
                warningMsg(QString("文件 %1 存在，已覆盖").arg(targetPath));
            }
        } else {
            if(!QFile::copy(filePath, targetPath)) {
                failMsg(QString("覆盖文件 %1").arg(targetPath));
            } else{
                warningMsg(QString("文件 %1 存在，已覆盖").arg(targetPath));
            }
        }
    } else {
        failMsg(QString("覆盖（删除已存在）文件 %1").arg(targetPath));
    }
}

/**
  * @brief fileSkip 文件跳过
  * @param filePath 源文件
  * @param targetPath 目标文件
 */
void MoveCopyWorkder::fileSkip(QString filePath, QString targetPath)
{
    Q_UNUSED(filePath)
    warningMsg(QString("文件 %1 存在，已跳过").arg(targetPath));
}

/**
  * @brief fileRetain 文件保留并编号
  * @param filePath 源文件
  * @param targetPath 目标文件
 */
void MoveCopyWorkder::fileRetain(QString filePath, QString targetPath)
{
    QFileInfo info(targetPath);
    QString baseName = info.completeBaseName();
    QString suffix = info.suffix();
    int k = 0;
    QString path = targetPath;
    while (QFile::exists(path)) {
        k++;
        QString name = QString("%1_%2.%3").arg(baseName).arg(k).arg(suffix);
        path = info.absoluteDir().absoluteFilePath(name);
    }
    targetPath = path;
    if(mIsMove) {
        if(!QFile::rename(filePath, targetPath)) {
            failMsg(QString("保留文件 %1").arg(targetPath));
        } else {
            warningMsg(QString("文件 %1 存在，已保留").arg(targetPath));
        }
    } else {
        if(!QFile::copy(filePath, targetPath)) {
            failMsg(QString("保留文件 %1").arg(targetPath));
        } else {
            warningMsg(QString("文件 %1 存在，已保留").arg(targetPath));
        }
    }
}

/**
  * @brief moveCopyFolder 移动/复制文件夹
  * @param info 源文件夹QFileInfo
 */
void MoveCopyWorkder::moveCopyFolder(QFileInfo info)
{
    QString dirPath = info.filePath();
    emit statusChanged(QString("正在%1文件夹 %2 ……").arg(mIsMove ? "移动" : "复制").arg(dirPath));
    QString dirName = info.fileName();
    QDir targetDir(mTargetDirPath);
    QString targetPath = targetDir.absoluteFilePath(dirName);
    if(targetPath == dirPath) {
        warningMsg("目标文件夹与源文件夹相同：" + dirPath);
        return;
    }

    if(targetPath.startsWith(dirPath + "/")) {
        errorMsg(QString("目标文件夹 %1 是源文件夹 %2 的子文件夹").arg(targetPath).arg(dirPath));
        return;
    }

    QDir dir(targetPath);
    if(dir.exists() && mTypeFolder == 0) {
        if(!dir.removeRecursively()) {
            failMsg(QString("删除目标文件夹：%1").arg(targetPath));
            return;
        }
    }
    recursiveFolder(info);
}

/**
  * @brief recursiveFolder 通过遍历源文件夹的方式复制/移动文件夹
  * @param info 源文件夹QFileInfo
 */
void MoveCopyWorkder::recursiveFolder(QFileInfo info)
{
    QString dirPath = info.filePath();
    QString dirName = info.fileName();

    QDir targetDir = QDir(mTargetDirPath).absoluteFilePath(dirName);
    QString targetPath = targetDir.path();
    if(!targetDir.exists()) {
        if(!QDir(mTargetDirPath).mkdir(dirName)) {
            failMsg(QString("创建目标文件夹 %1").arg(targetPath));
            return;
        }
    }

    QStringList folderList;
    QStringList fileList;
    folderFilesByParentFolder(dirPath, folderList, fileList);
    qDebug() << "源文件夹" << dirPath;

    foreach (QString subDirPath, folderList) {
        qDebug() << "源子文件夹" << subDirPath;
        QString target = subDirPath;
        target.replace(dirPath + "/", targetPath + "/");
        qDebug() << "目标子文件夹" << subDirPath;
        MakePath::toDirPath(target);
    }

    foreach (QString filePath, fileList) {
        qDebug() << "源子文件" << filePath;
        QFileInfo info(filePath);
        QString target = filePath;
        target.replace(dirPath + "/", targetPath + "/");
        qDebug() << "目标子文件" << target;
        moveCopyFile(info, target);
    }

    if(!mIsMove)
        return;
    // 文件夹排序
    QStringList emptyList = listByFolderLength(folderList, dirPath);
    // 移动后删除源文件夹下的空文件夹（包括源文件夹）
    foreach (QString subDirPath, emptyList)
        removeEmptyDir(subDirPath);
}

/**
  * @brief folderFilesByParentFolder 遍历选中的源文件夹下所有文件夹和文件，并生成目标文件夹和文件
  * @param dirPath 要遍历的源文件夹
  * @param folderList 源文件夹的子文件夹
  * @param fileList 源文件夹的子文件
 */
void MoveCopyWorkder::folderFilesByParentFolder(QString dirPath,
                                                QStringList &folderList, QStringList &fileList)
{
    QDir dir(dirPath);
    QFileInfoList info_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot
                                                | QDir::NoSymLinks | QDir::AllDirs);
    foreach(QFileInfo file_info, info_list) {
        QString path = file_info.absoluteFilePath();
        if(file_info.isDir()) {
            folderList << path;
            folderFilesByParentFolder(path, folderList, fileList);
        } else if(file_info.isFile()) {
            fileList << path;
        }
    }
}

/**
  * @brief listByFolderLength 按照文件夹长度为源文件夹排序，确定删除空文件夹的顺序
  * @param folderList 源子文件夹列表
  * @param dirPath 源文件夹
  * @return 按长度排序要删除的空文件夹列表（需要先检测是否为空，然后决定是否删除）
 */
QStringList MoveCopyWorkder::listByFolderLength(QStringList folderList, QString dirPath)
{
    QStringList list = folderList;

    std::stable_sort(list.begin(), list.end(), [](QString before, QString after){
        return before.split("/").size() > after.split("/").size();
    });

    list.append(dirPath);
    return list;
}

/**
  * @brief removeEmptyDir 移动后删除源文件夹里的空文件夹
  * @param dirPath 要遍历的源文件夹
 */
void MoveCopyWorkder::removeEmptyDir(QString dirPath)
{
    QDir dir(dirPath);
    QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot
                                           | QDir::NoSymLinks | QDir::AllDirs);


    if(list.isEmpty()) {
        if(!dir.removeRecursively())
            failMsg(QString("删除空文件夹：%1").arg(dirPath));
        return;
    }
    foreach (QFileInfo info, list) {
        if(info.isDir())
            removeEmptyDir(info.absoluteFilePath());
    }
}
