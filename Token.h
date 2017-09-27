#ifndef TOKEN_H
#define TOKEN_H

#include <QByteArray>
#include <QJsonObject>

class Token
{
    friend class Config;

public:
    Token() {}
    Token(const QByteArray &data);
    bool valid() const;
    char *data();
    int size() const;

private:
    void read(const QJsonObject &o);
    void write(QJsonObject &o) const;

    QByteArray mData;
};

#endif // TOKEN_H
