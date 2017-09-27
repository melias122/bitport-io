#include "Settings.h"
#include "ui_Settings.h"

#include <QCloseEvent>
#include <QDebug>
#include <QFileDialog>

#include "Config.h"

Settings::Settings(Config *config, QWidget *parent)
    : QDialog(parent)
    , ui{new Ui::Settings}
    , config(config)
{
    ui->setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	setFixedSize(width(), height());
	setWindowFlags( (windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowMaximizeButtonHint);

//    connect(ui->cancel, &QPushButton::clicked, this, &Settings::on_cancel_clicked);
//    connect(ui->logout, &QPushButton::clicked, this, &Settings::on_logout_clicked);

    ui->maxDownloads->setValue(config->maxConcurentDownloads());
    ui->downloadsDir->setText(config->downloadsDir());
}

void Settings::closeEvent(QCloseEvent *event)
{
    // Hide settings dialog instead of closing it.
    // Closing it will close application.
    event->ignore();
    emit finished(0);
}

void Settings::on_ok_clicked()
{
	qDebug() << "Settings::on_ok_clicked";
    bool ok;
    int maxDownloads = ui->maxDownloads->text().toInt(&ok);
	if (ok && !config->setMaxConcurentDownloads(maxDownloads)) {
        qWarning() << "unable to set maxDownloads" << ui->maxDownloads->text();
    }

    // autostart

    QString downloadsDir = QDir::fromNativeSeparators(ui->downloadsDir->text());
    if (!config->setDownloadsDir(downloadsDir)) {
        qWarning() << "unable to set downloadsDir" << ui->maxDownloads->text();
    }
	emit accept();
}

void Settings::on_cancel_clicked()
{
	emit reject();
}

void Settings::on_logout_clicked()
{
	config->logout();
	emit logout();
	emit accept();
}

void Settings::on_select_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Directory"),
                                                    QDir::toNativeSeparators(config->downloadsDir()),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (QDir::isAbsolutePath(dir)) {
        ui->downloadsDir->setText(dir);
    }
}
