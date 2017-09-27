#include "Systray.h"
#include "ui_Systray.h"

#include <vector>

#include <QWidgetAction>
#include <QDebug>
#include <QDesktopServices>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QThreadPool>
#include <QUrl>
#include <QWidgetAction>

#include "Config.h"
#include "FileItem.h"
#include "FileProgress.h"
#include "Login.h"
#include "Services.h"
#include "Settings.h"

using Status = Systray::Status;

constexpr int pollInterval = 5 * 60 * 1000; // 5 minutes
constexpr int pollRetryInterval = 60 * 1000;

static QHash<Status, QString> toStringStatus = {
    {Status::Connecting, QObject::tr("Connecting")},
    {Status::LookingForChanges, QObject::tr("Looking for Changes")},
    {Status::Downloading, QObject::tr("Downloading")},
    {Status::Synced, QObject::tr("Synced")},
	{Status::SyncFailed, QObject::tr("Sync Failed")},
};

Systray::Systray(const QIcon &icon, QWidget *parent)
    : QWidget(parent)
	, ui(new Ui::Systray)
	, mSettings(nullptr)
	, mConfig(new Config(this))
{
	ui->setupUi(this);

	ui->openFolder->setIcon(getAwesome()->icon(fa::folder));
	ui->openSettings->setIcon(getAwesome()->icon(fa::cogs));
//	ui->openFolder->setFont(getAwesome()->font(24));

	mSystrayIcon = new QSystemTrayIcon(icon, this);

#ifndef linux
	QWidgetAction *action = new QWidgetAction(nullptr);
	action->setDefaultWidget(this);

	QMenu *menu = new QMenu;
	menu->addAction(action);
	mSystrayIcon->setContextMenu(menu);
#endif

	mPoll = new QTimer(this);
	mPoll->setInterval(pollInterval);
	connect(mPoll, &QTimer::timeout, this, &Systray::on_pollTimeout);

	mFileProgressFactory = new FileProgressFactory(this);
	connect(mFileProgressFactory, &FileProgressFactory::fileProgress_created, this, &Systray::on_fileProgressCreated);

	QTimer::singleShot(0, this, &Systray::start);
}

void Systray::setStatus(Status status)
{
	ui->status->setText(toStringStatus[status]);
//	qDebug() << toStringStatus[status];
}

void Systray::start()
{
	if (mConfig->token().valid()) {
		setStatus(Status::Connecting);
		mSystrayIcon->show();
#ifdef linux
			show();
#endif
		mPoll->start(1500);

	} else {
		mSystrayIcon->hide();
#ifdef linux
			hide();
#endif
		mPoll->stop();

		// create Login dialog
		auto login = new Login(mConfig);
		connect(login, &Login::accepted, this, &Systray::start);
		login->show();
	}
}

void Systray::on_pollTimeout()
{
//	qDebug() << "on_poll_timeout";
	setStatus(Status::LookingForChanges);
	mPoll->stop();

	mDirSync = new DirSync(mConfig, mFileProgressFactory);
	connect(mDirSync, &DirSync::started, this, &Systray::on_dirSyncStarted, Qt::BlockingQueuedConnection);
	connect(mDirSync, &DirSync::finished, this, &Systray::on_dirSyncFinished, Qt::BlockingQueuedConnection);
	QThreadPool::globalInstance()->start(mDirSync);
}

void Systray::on_dirSyncStarted()
{
}

void Systray::on_dirSyncFinished(DirSync::Status status)
{
	disconnect(mDirSync, &DirSync::started, this, &Systray::on_dirSyncStarted);
	disconnect(mDirSync, &DirSync::finished, this, &Systray::on_dirSyncFinished);

	switch (status) {
	case DirSync::Synced:
		setStatus(Status::Synced);
		mPoll->start(pollInterval);
		break;
	case DirSync::Failed:
		setStatus(Status::SyncFailed);
		mPoll->start(pollRetryInterval);
		break;
	default:
		qWarning() << "undefined status=" << status;
		break;
	}
}

void Systray::on_fileProgressCreated(FileProgress *f)
{
	qDebug() << "on_fileProgress_created" << f;
	setStatus(Status::Downloading);

	QListWidgetItem *widgetItem = new QListWidgetItem;
	FileItem *fileItem = new FileItem(f, widgetItem);

	connect(fileItem, &FileItem::done, this, &Systray::on_fileItemDone);
	widgetItem->setSizeHint(fileItem->size());
	widgetItem->setFlags(widgetItem->flags() & ~Qt::ItemIsEnabled);


	ui->downloading->addItem(widgetItem);
	ui->downloading->setItemWidget(widgetItem, fileItem);
}

void Systray::on_fileItemDone(FileItem *f)
{
	qDebug() << "on_fileItemDone";
	disconnect(f, &FileItem::done, this, &Systray::on_fileItemDone);
	f->deleteLater();

	// delete from downloading
	int row = ui->downloading->row(f->mItem);
	delete ui->downloading->takeItem(row);

	// add to recent
	QListWidgetItem *item = new QListWidgetItem;
	FileItem *fileItem = new FileItem(f->name());
	item->setSizeHint(fileItem->size());

	ui->recentlyChanged->addItem(item);
	ui->recentlyChanged->setItemWidget(item, fileItem);
}

void Systray::on_openFolder_clicked()
{
	// TODO: check return
	QDesktopServices::openUrl(QUrl::fromUserInput(mConfig->downloadsDir()));
}

void Systray::on_openSettings_clicked()
{
	if (mSettings) {
		return;
	}

	mSettings = new Settings(mConfig);
	mSettings->show();

	connect(mSettings, &Settings::logout, this, &Systray::start);
	connect(mSettings, &Settings::finished, [=]() { mSettings = nullptr; });
}
