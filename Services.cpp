#include "Services.h"

#include <QDebug>

#include "bitport/bitport.h"
#include "Config.h"

TokenRetriever::TokenRetriever(const QString &id, const QString &secret, const QString &code)
{
    mId = id.toUtf8();
    mSecret = secret.toUtf8();
    mCode = code.toUtf8();
}

void TokenRetriever::run() {
    auto ret = bitport_login_token(mId.data(), mSecret.data(), mCode.data());
    auto data = QByteArray::fromRawData(ret.r0, ret.r1);
    Token t(data);
    emit token(t);
}

DirSync::DirSync(Config *c, IFileProgressFactory *f)
    : mConfig(c)
    , mFileProgressFacotry(f)
{
}

void DirSync::run()
{
    bitport_sync_cb progressCtx = {
        mFileProgressFacotry,
        &DirSync::bitport_sync_progress_file_new_cb,
        &DirSync::bitport_sync_progress_file_written_cb,
        &DirSync::bitport_sync_progress_file_done_cb
    };
    Token t = mConfig->token();
    QString dir = mConfig->downloadsDir();

	emit started();
	int ret = bitport_sync(t.data(), t.size(), dir.toUtf8().data(), mConfig->maxConcurentDownloads(), progressCtx);
	emit finished(Status(ret));
}

void *DirSync::bitport_sync_progress_file_new_cb(void *ctx, char *f, long long n)
{
    IFileProgressFactory *factory = static_cast<IFileProgressFactory*>(ctx);
    IFileProgress *p = factory->newFileProgress(QString::fromUtf8(f), n);
    return p;
}

void DirSync::bitport_sync_progress_file_written_cb(void *ctx, long long n)
{
    IFileProgress *f = static_cast<IFileProgress*>(ctx);
    bool ok = QMetaObject::invokeMethod(f, "addWritten", Qt::QueuedConnection, Q_ARG(long long, n));
    Q_ASSERT(ok);
}

void DirSync::bitport_sync_progress_file_done_cb(void *ctx, int status)
{
    IFileProgress *f = static_cast<IFileProgress*>(ctx);
    bool ok = QMetaObject::invokeMethod(f, "setDone", Qt::QueuedConnection);
    Q_ASSERT(ok);
}
