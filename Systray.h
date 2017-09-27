#ifndef SYSTRAY_H
#define SYSTRAY_H

#include <QWidget>

#include "Services.h"

namespace Ui {
class Systray;
}

class QSystemTrayIcon;

class Config;
class FileItem;
class FileProgress;
class FileProgressFactory;
class Settings;

class Systray : public QWidget
{
    Q_OBJECT
public:
    enum Status {
        Connecting,
        LookingForChanges,
        Downloading,
        Synced,
		SyncFailed,
    };
    Q_ENUM(Status)

	explicit Systray(const QIcon &icon, QWidget *parent = 0);

private:
	void setStatus(Status status);

private slots:
	void start();
	void on_pollTimeout();
	void on_dirSyncStarted();
	void on_dirSyncFinished(DirSync::Status status);
	void on_fileProgressCreated(FileProgress *f);
	void on_fileItemDone(FileItem *f);

	void on_openFolder_clicked();
	void on_openSettings_clicked();

private:
	Ui::Systray *ui;

	QSystemTrayIcon *mSystrayIcon;
	QTimer *mPoll;

	Settings *mSettings;
	Config *mConfig;
	DirSync *mDirSync;
	FileProgressFactory *mFileProgressFactory;
};

#endif // SYSTRAY_H
