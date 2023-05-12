#include "FolderMoveCopy.h"
#include "ui_FolderMoveCopy.h"

#define mType ui->moveRbn->isChecked() ? "移动" : "复制"
#define mTypeTip ui->moveRbn->isChecked() ? "移动文件（夹）" : "复制文件（夹）"

FolderMoveCopy::FolderMoveCopy(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FolderMoveCopy)
{
    ui->setupUi(this);
    initWidget();
}

FolderMoveCopy::~FolderMoveCopy()
{
    delete ui;
}

/**
  * @brief initWidget 初始化
 */
void FolderMoveCopy::initWidget()
{
    setWindowTitle("移动 & 复制文件（夹）");
    setWindowIcon(QIcon(":/icons/logo.svg"));
    setAcceptDrops(true);
    ui->plainTextEdit->setAcceptDrops(false);

    setStyleSheet(QString("font-family: Microsoft Yahei;"));
    loadQss();

    qRegisterMetaType<HighlightType>("HighlightType");

    mFolders << "删除" << "保留";
    mFiles << "覆盖" << "跳过" << "保留";
    ui->folderCbB->addItems(mFolders);
    ui->fileCbB->addItems(mFiles);
    connect(ui->folderCbB, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onFolderIndexChanged(int)));
    connect(ui->fileCbB, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onFileIndexChanged(int)));

    msgHighlight = new MsgHighlight(ui->plainTextEdit);

    connect(ui->originBtn, &QPushButton::clicked, this, &FolderMoveCopy::onOriginFolder);
    connect(ui->targetBtn, &QPushButton::clicked, this, &FolderMoveCopy::onTargetFolder);
    connect(ui->moveRbn, &QRadioButton::clicked, this, &FolderMoveCopy::onChecked);
    connect(ui->copyRbn, &QRadioButton::clicked, this, &FolderMoveCopy::onChecked);

    connect(ui->moveCopyBtn, &QPushButton::clicked, this, &FolderMoveCopy::onMoveCopy);

    connect(ui->clearBtn, &QPushButton::clicked, this, &FolderMoveCopy::onClear);
    ui->clearBtn->setVisible(false);

    initMenuBar();

    SetSaveLog;
}

/**
  * @brief loadQss 加载样式表
 */
void FolderMoveCopy::loadQss()
{
    QString allText;
    QFile file(":/qss");
    if(file.open(QIODevice::ReadOnly)) {
        allText = QString(file.readAll());
        file.close();
    }

    QString borderText = QString("\n#radioWidget:hover\n{\nborder: 1px solid %1;\n}\n")
            .arg(ThemeColor::windowsThemeName());
    allText.append(borderText);

    qApp->setStyleSheet(allText);
}

/**
  * @brief onFolderIndexChanged “若文件夹存在”模式
  * @param index
 */
void FolderMoveCopy::onFolderIndexChanged(int index)
{
    folderGp->actions().at(index)->setChecked(true);
}

/**
  * @brief onFolderIndexChanged “若文件存在”模式
  * @param index
 */
void FolderMoveCopy::onFileIndexChanged(int index)
{
    fileGp->actions().at(index)->setChecked(true);
}


/**
  * @brief initMenuBar 设置菜单栏和工具栏
 */
void FolderMoveCopy::initMenuBar()
{
    // 动作组 - 复制/移动
    QActionGroup *gp = new QActionGroup(this);
    gp->addAction(ui->copyAct);
    gp->addAction(ui->moveAct);
    ui->copyAct->setChecked(true);
    connect(gp, &QActionGroup::triggered, this, &FolderMoveCopy::onMoveCopyTriggered);
    ui->editMenu->addActions(gp->actions());

    // 动作组 - 若文件夹已存在
    QAction *folderExistAct = new QAction("若文件夹已存在", this);
    QMenu *folderExistMenu = new QMenu("文件夹菜单", this);
    folderExistAct->setMenu(folderExistMenu);
    ui->editMenu->addAction(folderExistAct);
    folderGp = new QActionGroup(this);
    foreach (QString name, mFolders) {
        QAction *act = new QAction(name, this);
        act->setCheckable(true);
        folderGp->addAction(act);
    }
    folderGp->actions().at(0)->setChecked(true);
    folderExistMenu->addActions(folderGp->actions());
    connect(folderGp, &QActionGroup::triggered, this, &FolderMoveCopy::onFolderExist);

    // 动作组 - 若文件已存在
    QAction *fileExistAct = new QAction("若文件已存在", this);
    QMenu *fileExistMenu = new QMenu("文件菜单", this);
    fileExistAct->setMenu(fileExistMenu);
    ui->editMenu->addAction(fileExistAct);
    fileGp = new QActionGroup(this);
    foreach (QString name, mFiles) {
        QAction *act = new QAction(name, this);
        act->setCheckable(true);
        fileGp->addAction(act);
    }
    fileGp->actions().at(0)->setChecked(true);
    fileExistMenu->addActions(fileGp->actions());
    connect(fileGp, &QActionGroup::triggered, this, &FolderMoveCopy::onFileExist);

    ui->editMenu->addSeparator();
    lockToolBarAct = new QAction("锁定工具栏", this);
    lockToolBarAct->setCheckable(true);
    lockToolBarAct->setChecked(true);
    ui->toolBar->setMovable(false);
    connect(lockToolBarAct, &QAction::triggered,
            [this](bool checked){ui->toolBar->setMovable(!checked);});
    ui->editMenu->addAction(lockToolBarAct);

    // 退出按钮 最右
    QWidget *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->toolBar->addWidget(spacer);
    ui->toolBar->addAction(ui->quitAct);

    setActionsTip();
    ui->moveCopyAct->setToolTip(mTypeTip);
    ui->moveCopyAct->setStatusTip(mTypeTip);

    connect(ui->originAct, &QAction::triggered, this, &FolderMoveCopy::onOriginFolder);
    connect(ui->fromTxtAct, &QAction::triggered, this, &FolderMoveCopy::onOriginTxt);
    connect(ui->fromClipboardAct, &QAction::triggered, this, &FolderMoveCopy::onOriginClipboard);
    connect(ui->targetAct, &QAction::triggered, this, &FolderMoveCopy::onTargetFolder);

    connect(ui->moveCopyAct, &QAction::triggered, this, &FolderMoveCopy::onMoveCopy);

    connect(ui->clearAct, &QAction::triggered, this, &FolderMoveCopy::onClear);

    connect(ui->quitAct, &QAction::triggered, this, &FolderMoveCopy::close);

    connect(ui->aboutAct, &QAction::triggered, this, &FolderMoveCopy::onAbout);
    connect(ui->aboutQtAct, &QAction::triggered, this, &FolderMoveCopy::onAboutQt);
}

/**
  * @brief onMoveCopyTriggered 切换复制/移动
  * @param act 复制/移动 按钮
 */
void FolderMoveCopy::onMoveCopyTriggered(QAction *act)
{
    Q_UNUSED(act)
    ui->copyRbn->setChecked(ui->copyAct->isChecked());
    ui->moveRbn->setChecked(ui->moveAct->isChecked());
    ui->moveCopyBtn->setText(mType);
    ui->moveCopyAct->setText(mType);
    ui->moveCopyAct->setToolTip(mTypeTip);
    ui->moveCopyAct->setStatusTip(mTypeTip);
    ui->moveCopyAct->setIcon(QIcon(ui->moveRbn->isChecked() ? ":/icons/move.svg"
                                                            : ":/icons/edit-copy.svg"));
}

/**
  * @brief onFolderExist “若文件夹存在”模式
  * @param act 模式按钮
 */
void FolderMoveCopy::onFolderExist(QAction *act)
{
    int index = folderGp->actions().indexOf(act);
    if(index == -1)
        return;
    ui->folderCbB->setCurrentIndex(index);
}

/**
  * @brief onFileExist “若文件存在”模式
  * @param act 模式按钮
 */
void FolderMoveCopy::onFileExist(QAction *act)
{
    int index = fileGp->actions().indexOf(act);
    if(index == -1)
        return;
    ui->fileCbB->setCurrentIndex(index);
}

/**
  * @brief setActionsTip 设置按钮提示
 */
void FolderMoveCopy::setActionsTip()
{
    QList<QAction *> acts = this->findChildren<QAction *>();
    if(acts.size() == 0)
        return;

    for(auto act : acts) {
        QString text = act->text();
        text.replace("&", "");
        QString shortCutStr = act->shortcut().toString();
        QString tooltipStr = shortCutStr.isEmpty() ? text
                                                  : QString("<p>%1<br /><b>快捷键：%2</b></p>")
                                                    .arg(text).arg(shortCutStr);
        QString statusStr = shortCutStr.isEmpty() ? text
                                                : QString("%1 快捷键：%2")
                                                    .arg(text).arg(shortCutStr);
        act->setToolTip(tooltipStr);
        act->setStatusTip(statusStr);
    }
}

/**
  * @brief onOriginFolder 选择源文件（夹）
 */
void FolderMoveCopy::onOriginFolder()
{
    QStringList fileList;
    CFileDialog *fileDialog = new CFileDialog(this);
    connect(fileDialog, &CFileDialog::finished, [&](int result){
        if(result != 1)
            return;
        fileList = fileDialog->selectedFiles();
    });
    fileDialog->exec();

    if(fileList.isEmpty())
        return;

    setOriginName(fileList);
}

/**
  * @brief setOriginName 显示源文件（夹）名称
  * @param fileList 文件夹/文件 列表
 */
void FolderMoveCopy::setOriginName(QStringList fileList)
{
    mSelectFiles.clear();
    QStringList nameList;
    for(auto path : fileList)
        nameList.append("\"" + QFileInfo(path).fileName() + "\"");
    ui->originEdit->setText(nameList.size() > 1 ? nameList.join(" ")
                                                : QString(fileList.at(0)));
    mSelectFiles = fileList;
    lastPath = fileList.at(fileList.size() - 1);
}

/**
  * @brief onOriginTxt 从文本文件导入
 */
void FolderMoveCopy::onOriginTxt()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "打开文本文件",
                                                    lastPath,
                                                    "文本文件(*.txt)");
    if(filePath.isEmpty())
        return;
    loadFromText(CommonLite::readFile(filePath));
    lastPath = filePath;
}

/**
  * @brief loadFromText 从文本中提取文件夹/文件路径
  * @param text 文件夹/文件 列表
 */
void FolderMoveCopy::loadFromText(QString text)
{
    text = text.trimmed();
    if(text.isEmpty())
        return;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    QStringList list = text.split("\n", Qt::SkipEmptyParts);
#else
    QStringList list = text.split("\n", QString::SkipEmptyParts);
#endif
    QStringList fileList;
    foreach (QString path, list) {
        if(path.startsWith("file:///"))
            path = path.mid(8);
        if(path.contains("\""))
            path.replace("\"", "");
        path = QDir::fromNativeSeparators(path);
        QFileInfo info(path);
        if(info.isDir() || info.isFile())
            fileList << path;
    }
    if(fileList.isEmpty())
        return;

    setOriginName(fileList);
}

/**
  * @brief onOriginClipboard 从剪贴板导入
 */
void FolderMoveCopy::onOriginClipboard()
{
    QClipboard *board = QApplication::clipboard();
    QString text = board->text();
    loadFromText(text);
}

/**
  * @brief onTargetFolder 选择目标文件夹
 */
void FolderMoveCopy::onTargetFolder()
{
    QString dirPath = QFileDialog::getExistingDirectory(this,
                                                        "选择目标文件夹",
                                                        lastPath);
    if(dirPath.isEmpty())
        return;
    ui->targetEdit->setText(dirPath);
    lastPath = dirPath;
}

/**
  * @brief onChecked 切换复制/移动
 */
void FolderMoveCopy::onChecked()
{
    ui->moveCopyBtn->setText(mType);
    ui->moveCopyAct->setText(mType);
    ui->copyAct->setChecked(ui->copyRbn->isChecked());
    ui->moveAct->setChecked(ui->moveRbn->isChecked());
    ui->moveCopyAct->setToolTip(mTypeTip);
    ui->moveCopyAct->setStatusTip(mTypeTip);
    ui->moveCopyAct->setIcon(QIcon(ui->moveRbn->isChecked() ? ":/icons/move.svg"
                                                            : ":/icons/edit-copy.svg"));
}

/**
  * @brief onChecked 复制/移动（真正的功能实现）
 */
void FolderMoveCopy::onMoveCopy()
{
    QString dirPath = ui->targetEdit->text();
    if(dirPath.isEmpty() || mSelectFiles.isEmpty())
        return;

    ui->plainTextEdit->clear();
    msgHighlight->tip("目标文件夹：");
    msgHighlight->normal(dirPath);
    msgHighlight->tip("源文件（夹）：");
    foreach (QString path, mSelectFiles) {
        QFileInfo info(path);
        QString typeStr;
        if(info.isDir())
            typeStr = "文件夹";
        else if(info.isFile())
            typeStr = "文件";
        if(info.isSymLink())
            typeStr += "（快捷方式）";
        msgHighlight->normal(typeStr + "：" + path);
    }
    msgHighlight->normal("");

    statusBar()->showMessage(QString("开始%1……").arg(mType));
    mTimeUp.start();

    setThreadWorker();
}

/**
  * @brief setThreadWorker 复制/移动 线程
 */
void FolderMoveCopy::setThreadWorker()
{
    if ( mWorker != nullptr && mWorker->isRunning() )
        mWorker->stop();

    mWorker.reset(new MoveCopyWorkder(ui->targetEdit->text(), mSelectFiles,
                                      ui->moveRbn->isChecked(),
                                      ui->folderCbB->currentIndex(),
                                      ui->fileCbB->currentIndex(),
                                      this));
    connect(mWorker.data(), &MoveCopyWorkder::statusChanged,
            statusBar(), &QStatusBar::showMessage);
    connect(mWorker.data(), &MoveCopyWorkder::msgChanged,
            this, &FolderMoveCopy::onMsgChanged);
    connect(mWorker.data(), &MoveCopyWorkder::finished,
            this, &FolderMoveCopy::onThreadFinished);
    mWorker->start();
}

/**
  * @brief onMsgChanged 消息提示
  * @param highlightType 消息类型
  * @param text 消息
 */
void FolderMoveCopy::onMsgChanged(HighlightType highlightType, QString text)
{
    msgHighlight->appendMsg(highlightType, text);
}

/**
  * @brief onThreadFinished 复制/移动 线程完毕
 */
void FolderMoveCopy::onThreadFinished()
{
    statusBar()->showMessage(QString("%1完毕").arg(mType), 10000);
    timeConsuming(mTimeUp.elapsed());
}

/**
  * @brief timeConsuming 计算用时
  * @param msecs 用时（毫秒）
 */
void FolderMoveCopy::timeConsuming(int msecs)
{
    QString timeStr = CommonLite::timeFormat(static_cast<qint64>(msecs));
    msgHighlight->normal(QString("共计用时 %1\n").arg(timeStr));
}

/**
  * @brief onClear 清空
 */
void FolderMoveCopy::onClear()
{
    ui->plainTextEdit->clear();
    ui->originEdit->clear();
    ui->targetEdit->clear();
    mSelectFiles.clear();
}

/**
  * @brief onAbout 关于
 */
void FolderMoveCopy::onAbout()
{
    QString dateTime = QString("%1").arg(__DATE__);
    QDate date = QLocale(QLocale::English).toDate(dateTime, QString("MMM dd yyyy"));
    if(!date.isValid())
        date = QLocale(QLocale::English).toDate(dateTime, QString("MMM  d yyyy"));
    QString dateText = QLocale().toString(date, QLocale::LongFormat);
    QVersionNumber version(1, 0);
    QString title = windowTitle();

    QString msg = QString("<p><b>%1 v%2<b/></p>"
                          "<p>%1 是一个基于 <a style=\"text-decoration:none;\" "
                          "href=\"https://www.qt.io/\">Qt</a>，"
                          "将选中的源文件夹/文件，复制/移动到目标文件夹的简易工具。</p>"
                          "<p>&copy; snolkmg - %3</p>")
            .arg(title).arg(version.toString()).arg(dateText);
    QMessageBox::about(this, "关于 " + title, msg);
}

/**
  * @brief onAbout 关于 Qt
 */
void FolderMoveCopy::onAboutQt()
{
    QMessageBox::aboutQt(this, "关于 Qt");
}

/**
  * @brief closeEvent 关闭时检测线程状态
  * @param event 关闭事件
 */
void FolderMoveCopy::closeEvent(QCloseEvent *event)
{
    if ( mWorker != nullptr && mWorker->isRunning() ) {
        event->ignore();
        QMessageBox closeMsg( QMessageBox::Question,
                              tr( "%1文件（夹）" ).arg(mType),
                              tr( "正在%1文件（夹），确定停止并退出？" ).arg(mType),
                              QMessageBox::Yes | QMessageBox::No,
                              this );
        closeMsg.setButtonText( QMessageBox::Yes, tr( "确定" ) );
        closeMsg.setButtonText( QMessageBox::No, tr( "取消" ) );
        closeMsg.exec();
        if ( closeMsg.result() == QMessageBox::Yes ) {
            MoveCopyWorkder* worker = mWorker.take();
            if ( worker->isPaused() )
                worker->resume();
            worker->stop();
            event->accept();
        }
    }
}
