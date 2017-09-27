#include "FileItem.h"
#include "ui_FileItem.h"

#include <QDebug>
#include <QListWidgetItem>

#include "Config.h"
#include "FileProgress.h"
#include "Systray.h"

FileItem::FileItem(FileProgress *p, QListWidgetItem *item, QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FileItem)
	, mProgress(p)
	, mItem(item)
{
	ui->setupUi(this);

	ui->icon->setText(QChar(fa::clouddownload));
	ui->icon->setFont(getAwesome()->font(24));
	ui->filename->setText(p->mName);

	connect(p, &FileProgress::changed, this, &FileItem::on_progressChanged);
	connect(p, &FileProgress::done, this, &FileItem::on_progressDone);
}

FileItem::FileItem(const QString &name, QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FileItem)
{
	ui->setupUi(this);
	ui->status->clear();
	ui->speed->clear();

	ui->icon->setText(QChar(fa::fileo));
	ui->icon->setFont(getAwesome()->font(24));
	ui->filename->setText(name);
}

FileItem::~FileItem()
{
	delete ui;
}

void FileItem::on_progressChanged(quint64 _speed, quint64 written, quint64 total)
{
	QString s("bytes/sec");
	double speed = 0;
	if (_speed > 1024) {
		speed = (double)(_speed)/1024.0;
		s = "Kb/sec";
	}

	if (speed > 1024) {
		speed /= 1024.0;
		s = "Mb/sec";
	}

	double perc = (double)(written)/total;
	perc *= 100;

	ui->status->setText(QString("%1\%").arg(perc, 0, 'f', 2));
	ui->speed->setText(QString("%1 %2").arg(speed, 0, 'f', 2).arg(s));
}

void FileItem::on_progressDone()
{
	// set Downloaded
	ui->status->setText(tr("100%"));
	ui->speed->setText("");

	emit done(this);

	// delete progress
	mProgress->deleteLater();
}

QString FileItem::name()
{
	return ui->filename->text();
}
