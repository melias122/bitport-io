#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

#include "Token.h"

namespace Ui {
class Login;
}

class Config;

class Login : public QDialog
{
    Q_OBJECT

public:
	explicit Login(Config *config, QWidget *parent = 0);
    ~Login();

private slots:
	void on_token(const Token &t);
	void on_login_clicked();

private:
    Ui::Login *ui;
	Config *mConfig;
};

#endif // LOGIN_H
