#include "CFileDialog.h"

CFileDialog::CFileDialog(QWidget *parent) :
    QFileDialog(parent)
{
    this->setOptions(QFileDialog::DontUseNativeDialog);

    fileNameEdit = this->findChild<QLineEdit*>("fileNameEdit");

    //支持多选
    QListView *pListView = this->findChild<QListView*>("listView");
    if (pListView) {
        if(!fileModel)
            fileModel = qobject_cast<QFileSystemModel *>(pListView->model());
        pListView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }

    QTreeView *pTreeView = this->findChild<QTreeView*>("treeView");
    if (pTreeView) {
        if(!fileModel)
            fileModel = qobject_cast<QFileSystemModel *>(pTreeView->model());
        pTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }
    QDialogButtonBox *pButton = this->findChild<QDialogButtonBox *>();

    QItemSelectionModel *model = pListView->selectionModel();
    connect(model, &QItemSelectionModel::selectionChanged, this, &CFileDialog::onSelectionChanged);

    disconnect(pButton, SIGNAL(accepted()), this, SLOT(accept()));//使链接失效
    connect(pButton, SIGNAL(accepted()), this, SLOT(onChoose()));//改成自己的槽
}

CFileDialog::~CFileDialog()
{

}

void CFileDialog::onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected)
    Q_UNUSED(deselected)
    if(nullptr == fileNameEdit)
        return;
//    if(selected.indexes().isEmpty()) {
//        selectedList.clear();
//        return;
//    }

    bool ok = false;
    QStringList fileList = realDir(selectedFiles(), ok);
    if(fileList.isEmpty() || ok) {
        fileNameEdit->clear();
    } else {
        QStringList nameList;
        for(auto path : fileList)
            nameList.append("\"" + QFileInfo(path).fileName() + "\"");
        fileNameEdit->setText(nameList.size() > 1 ? nameList.join(" ")
                                                  : QString(nameList.at(0)).replace("\"", ""));
    }
    selectedList = fileList;
}

void CFileDialog::onChoose()
{
//    qDebug() << "所选文件夹" << selectedList;.
//    emit selectedChanged(selectedList);
    QDialog::accept();
}

//! 打开文件夹自定义窗口产生的bug，临时修复方法
QStringList CFileDialog::realDir(QStringList fileList, bool &ok)
{
    if(fileList.size() != 1)
        return fileList;
    QString path = fileList.at(0);
    QFileInfo info(path);
    if(info.isFile() || info.isDir())
        return fileList;
    QString name = info.fileName();
    QString pName = QFileInfo(info.absoluteDir().path()).fileName();
    if(pName != name)
        return fileList;
    if(pName == name && name.isEmpty())
        return fileList;
    QString dirPath = path;
    dirPath = dirPath.mid(0, dirPath.lastIndexOf("/"));
    QFileInfo fileInfo(dirPath);
    if(fileInfo.isDir())
        fileList[0] = dirPath;
    ok = true;
    return fileList;
}
