#ifndef FILEPROGRESS_H
#define FILEPROGRESS_H

#include "IFileProgreess.h"

#include <QTimer>
#include <deque>

class FileProgress : public IFileProgress
{
	Q_OBJECT
	friend class FileItem;
public:
	FileProgress(const QString &name, long long n, QObject *parent = 0);
	~FileProgress();

signals:
	void changed(quint64 speed, quint64 written, quint64 total);
	void done();

public slots:
	void addWritten(long long n);
	void setDone();

private:
	QTimer mTimer;
	QString mName;
	quint64 mBytesWritten, mBytesTotal;

	// circular list
	std::deque<quint64> mQueue;
	quint64 mLastBytesWritten;
};

class FileProgressFactory : public QObject, public IFileProgressFactory
{
	Q_OBJECT
public:
	FileProgressFactory(QObject *parent = 0);
	IFileProgress* newFileProgress(const QString &p, long long n);

signals:
	void fileProgress_created(FileProgress*);

private slots:
	FileProgress* create(const QString &p, long long n);
};

#endif // FILEPROGRESS_H
