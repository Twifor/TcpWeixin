#ifndef CENTERWIDGET_H
#define CENTERWIDGET_H

#include <QGraphicsDropShadowEffect>
#include <QWidget>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QWidget>
#include "commonwidget.h"
#include "httpclient.h"
#include "commonwidget.h"
#include "databean.h"

class CenterWidget : public QWidget
{
	Q_OBJECT
public:
	explicit CenterWidget(QWidget *parent = nullptr);

signals:
	void loginSuccess(QString userName, int token);

public slots:
	void getLoginData(QString data);
	void getRegData(QString data);
	// QWidget interface
protected:
	void paintEvent(QPaintEvent *event) override;
	void startClient();
	void showErr(QString msg);

private:
	QGraphicsDropShadowEffect *effect;
	QPushButton *exit_btn, *mini;
	QGraphicsOpacityEffect *opacity;
	QPropertyAnimation *animation;
	QLabel *userName_label, *pwd_label, *reg_userName_label, *reg_pwd_label, *reg_pwd_label2;
	QLineEdit *userName_edit, *pwd_edit, *reg_userName_edit, *reg_pwd_edit, *reg_pwd_edit2;
	QPushButton *login, *reg, *ok, *back;
	QStackedWidget *sta;//用于切换注册和登录的布局
	QWidget *w0, *w1;
	CommonWidget *netWorkErr, *loginErr;
};

#endif // CENTERWIDGET_H
