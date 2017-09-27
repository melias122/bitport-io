#include "Config.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QRunnable>
#include <QStandardPaths>
#include <QThreadPool>

class Deleter : public QRunnable
{
public:
	Deleter(const QString &path) : path{path} {}

private:
	void run() override {
		QDir d(path);
		d.removeRecursively();
	}

	QString path;
};

QtAwesome *getAwesome() {
	static QtAwesome *a = nullptr;
	if (!a) {
		a = new QtAwesome;
	}
	return a;
}

Config::Config(QObject *parent)
    : QObject(parent)
	, _configDir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation))
{
    // TODO(m): what to do if this path is empty?
    QDir confDir(_configDir);
    if (!confDir.exists() && !confDir.mkpath(confDir.path())) {
        qDebug() << "unable to create direcotory " << confDir.path();
    }

    // try to load saved configs
    if (!load()) {
        // if it does not exist, write default config
		qInfo() << "creating default config";
		setDefaults();
        save();
    }
}

Token Config::token() const
{
    return _token;
}

bool Config::setToken(const Token &t)
{
    _token = t;
    return save();
}

QString Config::downloadsDir() const
{
    return _downloadsDir;
}

bool Config::setDownloadsDir(const QString &path)
{
	if (!QDir::isAbsolutePath(path)) {
		return false;
	}
	if (path == _downloadsDir) {
		return true;
	}
    _downloadsDir = path;
    return save();
}

int Config::maxConcurentDownloads() const
{
    return _maxConcurentDownloads;
}

bool Config::setMaxConcurentDownloads(int max)
{
    if (max < 1) {
        max = 1;
    }
    // set some reasonable maximum ...
    if (max > 20) {
        max = 20;
    }
	if (max == _maxConcurentDownloads) {
		return true;
	}
    _maxConcurentDownloads = max;
    return save();
}

bool Config::logout()
{
	QThreadPool::globalInstance()->start(new Deleter(downloadsDir()));
	setDefaults();
	return save();
}

void Config::read(const QJsonObject &json)
{
    _token.read(json["token"].toObject());
    _downloadsDir = json["downloads_dir"].toString();
    _maxConcurentDownloads = json["max_concurent_downloads"].toInt();
}

void Config::write(QJsonObject &json)
{
    QJsonObject token;
    _token.write(token);
    json["token"] = token;
    json["downloads_dir"] = _downloadsDir;
    json["max_concurent_downloads"] = _maxConcurentDownloads;
}

bool Config::load()
{
    QFile f(_configDir + "/config.json");
    if (!f.open(QIODevice::ReadOnly)) {
        qWarning() << "could not open" << f.fileName();
        return false;
    }

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(f.readAll(), &err);
    if (err.error != QJsonParseError::NoError) {
        qWarning() << err.errorString();
        return false;
    }
    read(doc.object());
    return true;
}

bool Config::save()
{
    QJsonObject obj;
    write(obj);

    QFile f(_configDir + "/config.json");
    if (!f.open(QIODevice::WriteOnly)) {
        qWarning() << "could not open" << f.fileName();
        return false;
    }

    QJsonDocument doc(obj);
    f.write(doc.toJson());
    return true;
}

void Config::setDefaults()
{
	_downloadsDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/Bitport Downloads";
	_maxConcurentDownloads = 3;
	_token = Token();
}
