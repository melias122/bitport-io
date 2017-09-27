#include "FileProgress.h"

#include <QDebug>

FileProgress::FileProgress(const QString &name, long long n, QObject *parent)
	: mName(name)
	, mBytesWritten(0)
	, mBytesTotal(n)
	, mLastBytesWritten(0)
{
	setParent(parent);

	connect(&mTimer, &QTimer::timeout, [=]() {

		mQueue.push_front(mLastBytesWritten);
		if (mQueue.size() > 10) {
			mQueue.pop_back();
		}
		mLastBytesWritten = 0;

		long long sum = 0;
		for (auto v : mQueue) {
			sum += v;
		}
		sum /= mQueue.size();

		emit changed(sum, mBytesWritten, mBytesTotal);
	});

	mTimer.start(1000);
}

FileProgress::~FileProgress()
{
	qDebug() <<  "deleting FileProgress";
}

void FileProgress::addWritten(long long n)
{
	mLastBytesWritten += n;
	mBytesWritten += n;
}

void FileProgress::setDone()
{
	mTimer.stop();
	emit done();
}

// FileProgressFactory
FileProgressFactory::FileProgressFactory(QObject *parent)
{
	setParent(parent);
}

IFileProgress* FileProgressFactory::newFileProgress(const QString &p, long long n)
{
	FileProgress* ret = nullptr;
	bool ok = QMetaObject::invokeMethod(this, "create", Qt::BlockingQueuedConnection,
							  Q_RETURN_ARG(FileProgress*, ret),
							  Q_ARG(QString, p),
							  Q_ARG(long long, n));
	if (!ok) {
		qDebug() << "newFileProgress" << ret;
	}
	return ret;
}

FileProgress *FileProgressFactory::create(const QString &p, long long n)
{
	auto f = new FileProgress(p, n, this);
	emit fileProgress_created(f);
	return f;
}
