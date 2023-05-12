#include "FolderMoveCopy.h"
#include <QApplication>
#include <QTranslator>
#include <SingleApplication>

// 高版本的Qt 中 Q_OS_WINDOWS 等价于 Q_OS_WIN

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

void raiseWidget(QWidget *w)
{
#ifdef Q_OS_WIN
    HWND hwnd = (HWND)w->winId();

    // check if widget is minimized to Windows task bar
    if (::IsIconic(hwnd)) {
        ::ShowWindow(hwnd, SW_RESTORE);
    }

    ::SetForegroundWindow(hwnd);
#else
    w->show();
    w->raise();
    w->activateWindow();
#endif
}

int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    SingleApplication a(argc, argv, true);

    if (a.isSecondary()) {
        AllowSetForegroundWindow( DWORD( a.primaryPid() ) );
        a.sendMessage("RAISE_WIDGET");
        return 0;
    }
#else
    SingleApplication a(argc, argv);
#endif

    FolderMoveCopy w;

#ifdef Q_OS_WIN
    QObject::connect(&a, &SingleApplication::receivedMessage,
                     [ &w ] () { raiseWidget(&w); });
#else
    QObject::connect( &a, &SingleApplication::instanceStarted,
                      [ &w ] () { raiseWidget(&w); });
#endif

    QTranslator tl2;
    tl2.load(":/qt_zh_CN");
    a.installTranslator(&tl2);

    QTranslator tl;
    tl.load(":/zh_CN");
    a.installTranslator(&tl);

    w.show();

    return a.exec();
}
