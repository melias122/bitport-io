#ifndef SERVICES_H
#define SERVICES_H

#include <QObject>
#include <QRunnable>
#include <QMutexLocker>
#include <QHash>

#include "IFileProgreess.h"
#include "Token.h"

class Config;

class TokenRetriever : public QObject, public QRunnable
{
    Q_OBJECT

public:
    TokenRetriever(const QString &id, const QString &secret, const QString &code);

signals:
    void token(const Token &t);

private:
    void run() override;

    QByteArray mId, mSecret, mCode;
};

class DirSync : public QObject, public QRunnable
{
    Q_OBJECT

public:
	enum Status {
		Synced,
		Failed
	};
	Q_ENUM(Status)

    DirSync(Config *c, IFileProgressFactory *f);

signals:
    void started();
	void finished(Status status);

private:
    void run() override;

    static void* bitport_sync_progress_file_new_cb(void *ctx, char *f, long long n);
    static void bitport_sync_progress_file_written_cb(void *ctx, long long n);
    static void bitport_sync_progress_file_done_cb(void *ctx, int status);

private:
    Config *mConfig;
    IFileProgressFactory *mFileProgressFacotry;
};

#endif // SERVICES_H
