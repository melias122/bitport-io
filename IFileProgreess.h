#ifndef IFILEPROGREESS_H
#define IFILEPROGREESS_H

#include <QObject>

class IFileProgress : public QObject
{
    Q_OBJECT
public:
    virtual ~IFileProgress() {}
public slots:
    virtual void addWritten(long long n) = 0;
    virtual void setDone() = 0;
};

// IFileProgressFactory must be thread safe
class IFileProgressFactory
{
public:
    virtual ~IFileProgressFactory() {}
    virtual IFileProgress* newFileProgress(const QString &p, long long n) = 0;
};

#endif // IFILEPROGREESS_H
