#include "MakePath.h"

MakePath::MakePath()
{

}

/**
  * @brief MakePath::recursiveDir 递归创建文件夹
  * @param list 文件夹名称列表，例：D:/abc/def -> D:, abc, def
  * @param isDir 原路径是否为文件夹，若是，递归到最后一层，否则，递归到倒数第二层，因为最后一层是文件名
  * @return 返回最终递归文件夹
 */
QDir MakePath::recursiveDir(QStringList list, bool isDir)
{
    int depth = isDir ? 0 : 1;
    QDir dir = QString(list.at(0));
    list.removeFirst();
    while (list.size() > depth) {
        QString name = QString(list.at(0));
        QDir subDir = dir.absoluteFilePath(name);
        if(!subDir.exists()) {
            if(!dir.mkdir(name))
                qCritical() << "recursive" << dir.path() << name;
        }
        list.removeFirst();
        dir = subDir;
    }
    return dir;
}

/**
  * @brief MakePath::toDir 递归创建文件夹（存在则直接下一层）
  * @param dirPath 文件夹路径
  * @param removeIfExist 若为true，则删除文件夹路径（若存在），重新创建。默认为false
  * @return 返回最终递归文件夹
 */
QDir MakePath::toDir(QString dirPath, bool removeIfExist)
{
    if(removeIfExist && QDir(dirPath).exists()) {
        if(!QDir(dirPath).removeRecursively())
            qCritical() << "removeRecursively" << dirPath;
    }
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    QStringList list = dirPath.split("/", Qt::SkipEmptyParts);
#else
    QStringList list = dirPath.split("/", QString::SkipEmptyParts);
#endif
    if(list.size() < 2)
        return QDir(dirPath);

    return recursiveDir(list, true);
}

/**
  * @brief MakePath::toDir 递归创建文件夹（存在则直接下一层）
  * @param targetDir 目标文件夹
  * @param removeIfExist 若为true，则删除文件夹路径（若存在），重新创建。默认为false
  * @return 返回最终递归文件夹
 */
QDir MakePath::toDir(QDir targetDir, bool removeIfExist)
{
    return toDir(targetDir.path(), removeIfExist);
}

/**
  * @brief MakePath::toDirPath 递归创建文件夹（存在则直接下一层）
  * @param dirPath 文件夹路径
  * @param removeIfExist 若为true，则删除文件夹路径（若存在），重新创建。默认为false
  * @return 返回最终递归文件夹路径
 */
QString MakePath::toDirPath(QString dirPath, bool removeIfExist)
{
    return toDir(dirPath, removeIfExist).path();
}

/**
  * @brief MakePath::toDirPath 递归创建文件夹（存在则直接下一层）
  * @param targetDir 目标文件夹
  * @param removeIfExist 若为true，则删除文件夹路径（若存在），重新创建。默认为false
  * @return 返回最终递归文件夹路径
 */
QString MakePath::toDirPath(QDir targetDir, bool removeIfExist)
{
    return toDir(targetDir, removeIfExist).path();
}

/**
  * @brief MakePath::toPath 为文件递归创建父文件夹（存在则直接下一层）
  * @param filePath 文件路径
  * @param deleteIfExist 若为true，则删除文件（若存在）。默认为false
  * @return 返回文件路径
 */
QString MakePath::toPath(QString filePath, bool deleteIfExist)
{
    if(deleteIfExist && QFile::exists(filePath)) {
        if(!QFile::remove(filePath))
            qCritical() << "remove" << filePath;
    }
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    QStringList list = filePath.split("/", Qt::SkipEmptyParts);
#else
    QStringList list = filePath.split("/", QString::SkipEmptyParts);
#endif

    if(list.size() > 1)
        recursiveDir(list, false);
    return filePath;
}
