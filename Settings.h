#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

class Config;

namespace Ui {
class Settings;
}

//namespace BitportUi {

class Settings : public QDialog
{
    Q_OBJECT
public:
    explicit Settings(Config *config, QWidget *parent = nullptr);

signals:
    void logout();

private:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_ok_clicked();
    void on_cancel_clicked();
    void on_logout_clicked();
    void on_select_clicked();

private:
    Ui::Settings *ui;
    Config *config;
};

//}

#endif // SETTINGS_H
