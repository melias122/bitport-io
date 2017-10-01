#include "Login.h"
#include "ui_Login.h"

#include <QDebug>
#include <QThreadPool>

#include "Config.h"
#include "Services.h"

Login::Login(Config *config, QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::Login)
	, mConfig(config)
{
    ui->setupUi(this);
	setFixedSize(width(), height());
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowFlags( (windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowMaximizeButtonHint);

	connect(this, &Login::rejected, QApplication::instance(), &QApplication::quit);
}

Login::~Login()
{
	qDebug() << "~Login";
    delete ui;
}

void Login::on_token(const Token &t)
{
	ui->login->setEnabled(true);
	if (!t.valid()) {
		qWarning() << "token not valid";
		ui->info->setText(tr("Invalid code"));
		return;
	}

	if (!mConfig->setToken(t)) {
		qWarning() << "can't set token";
		ui->info->setText(tr("Can't save token"));
		return;
	}
	emit accept();
}

void Login::on_login_clicked()
{
	ui->info->clear();
	qDebug() << "code=" << ui->code->text();

	if (ui->code->text() == "") {
		return;
	}
	ui->info->setText(tr("Connecting"));
	ui->login->setEnabled(false);

	QString id("36414");
	QString secret("gsn911t374");

	TokenRetriever *retriever = new TokenRetriever(id, secret, ui->code->text());
	connect(retriever, &TokenRetriever::token, this, &Login::on_token, Qt::BlockingQueuedConnection);
	QThreadPool::globalInstance()->start(retriever);
}
