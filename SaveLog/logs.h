#ifndef LOGS
#define LOGS

#include "savelog.h"

#define SetSaveLog \
SaveLog::Instance()->setMaxRow(0); \
SaveLog::Instance()->setMaxSize(0); \
SaveLog::Instance()->setUseContext(true); \
SaveLog::Instance()->setPath(QString("%1/logs").arg(qApp->applicationDirPath())); \
SaveLog::Instance()->start(); \

#endif // LOGS

