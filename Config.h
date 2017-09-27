#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>

#include "Token.h"
#include "QtAwesome.h"
#include "QtAwesomeAnim.h"

QtAwesome *getAwesome();

class Config : public QObject
{
    Q_OBJECT

public:
    Config(QObject *parent = nullptr);
    Token token() const;
    bool setToken(const Token &t);

    // download dir
    QString downloadsDir() const;
    bool setDownloadsDir(const QString &path);

    // max downloads
    int maxConcurentDownloads() const;
    bool setMaxConcurentDownloads(int max);

	bool logout();

private:
    void read(const QJsonObject &json);
    void write(QJsonObject &json);
    bool load();
    bool save();
	void setDefaults();

    QString _configDir, _downloadsDir;
    int _maxConcurentDownloads;
    Token _token;
};

#endif // CONFIG_H
