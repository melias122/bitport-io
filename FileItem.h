#ifndef FILEITEM_H
#define FILEITEM_H

#include <QWidget>

namespace Ui {
class FileItem;
}

class FileProgress;
class QListWidgetItem;

class FileItem : public QWidget
{
	Q_OBJECT
	friend class Systray;

public:
	FileItem(FileProgress *p, QListWidgetItem *item, QWidget *parent = nullptr);
	FileItem(const QString &name, QWidget *parent = nullptr);
	~FileItem();

signals:
	void done(FileItem *item);

private slots:
	void on_progressChanged(quint64 speed, quint64 written, quint64 total);
	void on_progressDone();

private:
	Ui::FileItem *ui;
	FileProgress *mProgress{nullptr};
	QListWidgetItem *mItem{nullptr};

	QString name();
};

#endif // FILEITEM_H
