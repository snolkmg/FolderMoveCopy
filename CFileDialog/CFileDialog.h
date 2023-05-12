#ifndef CFILEDIALOG_H
#define CFILEDIALOG_H

#include <QWidget>
#include <QFileDialog>
#include <QTreeView>
#include <QListView>
#include <QDialogButtonBox>
#include <QFileSystemModel>
#include <QLineEdit>
#include <QFileInfo>
#include <QDebug>

class CFileDialog : public QFileDialog
{
    Q_OBJECT
public:
    explicit CFileDialog(QWidget *parent = 0);
    ~CFileDialog();

Q_SIGNALS:
    void selectedChanged(QStringList fileList);

private Q_SLOTS:
    void onChoose();
    void onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

private:
    QStringList selectedList;
    QLineEdit *fileNameEdit;
    QFileSystemModel *fileModel;
    QStringList realDir(QStringList fileList, bool &ok);
};

#endif // CFILEDIALOG_H
