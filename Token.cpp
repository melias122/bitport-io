#include "Token.h"

Token::Token(const QByteArray &data)
    : mData(data)
{
}

bool Token::valid() const
{
    return mData.size() > 0;
}

char *Token::data()
{
    return mData.data();
}

int Token::size() const
{
    return mData.size();
}

void Token::read(const QJsonObject &json)
{
    QString s = json["data"].toString();
    mData = QByteArray::fromBase64(s.toUtf8());
}

void Token::write(QJsonObject &json) const
{
    QString s(mData.toBase64());
    json["data"] = s;
}
