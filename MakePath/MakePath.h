#ifndef MAKEPATH_H
#define MAKEPATH_H

#include <QString>
#include <QStringList>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QDebug>

class MakePath
{
public:
    MakePath();
    static QDir toDir(QString dirPath, bool removeIfExist = false);
    static QDir toDir(QDir targetDir, bool removeIfExist = false);
    static QString toDirPath(QString dirPath, bool removeIfExist = false);
    static QString toDirPath(QDir targetDir, bool removeIfExist = false);
    static QString toPath(QString filePath, bool deleteIfExist = false);

private:
    static QDir recursiveDir(QStringList list, bool isDir);
};

#endif // MAKEPATH_H
