#include "centerwidget.h"
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QStyleOption>
#include <QMouseEvent>
#include <cstdlib>
#include <QDebug>

CenterWidget::CenterWidget(QWidget *parent) : QWidget(parent)
{
	setGeometry(10, 10, 480, 380);
	setStyleSheet("QWidget{"
				  "background-color: rgb(255,255, 255);"
				  "border-top-left-radius:15px;"
				  "border-top-right-radius:15px;"
				  "border-bottom-right-radius:15px;"
				  "border-bottom-left-radius:15px;"
				  "}");
	effect = new QGraphicsDropShadowEffect(this);
	effect->setOffset(0, 0);
	effect->setColor(QColor(63, 63, 63, 180));
	effect->setBlurRadius(15);

	opacity = new QGraphicsOpacityEffect(this);
	opacity->setOpacity(1.0);

	setGraphicsEffect(opacity);
	animation = new QPropertyAnimation(this);
	animation->setTargetObject(opacity);
	animation->setPropertyName("opacity");
	animation->setStartValue(0.01);
	animation->setEndValue(1.0);
	animation->setDuration(1000);
	animation->start();

	connect(animation, &QPropertyAnimation::finished, [&](){
		setGraphicsEffect(effect);
		animation->deleteLater();
	});

	exit_btn = new QPushButton(this);
	mini = new QPushButton(this);
	exit_btn->setGeometry(445, 5, 30, 30);
	mini->setGeometry(415, 5, 30, 30);
	exit_btn->setStyleSheet("QPushButton{"
							"background:url(:/std/sucai/btn_exit1.png) -120px 0px no-repeat;"
							"}"
							"QPushButton:hover{"
							"background:url(:/std/sucai/btn_exit2.png) -120px 0px no-repeat;"
							"}"
							"QPushButton:pressed {"
							"background:url(:/std/sucai/btn_exit2.png) -120px 0px no-repeat;"
							"}");
	mini->setStyleSheet("QPushButton{"
						"background:url(:/std/sucai/btn_minimize1.png) -120px 0px no-repeat;"
						"}"
						"QPushButton:hover{"
						"background:url(:/std/sucai/btn_minimize2.png) -120px 0px no-repeat;"
						"}"
						"QPushButton:pressed {"
						"background:url(:/std/sucai/btn_minimize2.png) -120px 0px no-repeat;"
						"}");
	connect(exit_btn, &QPushButton::clicked, this->parentWidget(), &QWidget::close);
	connect(mini, &QPushButton::clicked, this->parentWidget(), &QWidget::showMinimized);

	sta = new QStackedWidget(this);
	sta->setGeometry(0, 180, 480, 200);

	w0 = new QWidget(sta);
	w0->setGeometry(0, 10, 480, 200);
	sta->addWidget(w0);

	userName_label = new QLabel(w0);
	userName_label->setText("用户名");
	userName_label->setGeometry(100, 20, 50, 25);
	userName_edit = new QLineEdit(w0);
	userName_edit->setGeometry(170, 20, 200, 25);
	userName_edit->setStyleSheet("QLineEdit {"
								 "border: 1px solid rgb(50, 50, 100);"
								 "border-radius: 6px;"
								 "background: white;"
								 "selection-background-color: blue;"
								 "font-size: 14px ;"
								 " }"
								 "QLineEdit:hover {"
								 "border: 1px solid blue;"
								 "}");
	pwd_label = new QLabel(w0);
	pwd_label->setText("密码");
	pwd_label->setGeometry(100, 70, 50, 25);
	pwd_edit = new QLineEdit(w0);
	pwd_edit->setGeometry(170, 70, 200, 25);
	pwd_edit->setStyleSheet("QLineEdit {"
							"border: 1px solid rgb(50, 50, 100);"
							"border-radius: 6px;"
							"background: white;"
							"selection-background-color: blue;"
							"font-size: 14px ;"
							" }"
							"QLineEdit:hover {"
							"border: 1px solid blue;"
							"}");
	pwd_edit->setEchoMode(QLineEdit::Password);

	login = new QPushButton(w0);
	login->setText("登录");
	login->setGeometry(100, 120, 100, 40);
	login->setStyleSheet("QPushButton{"
						 "border-radius:10px;"
						 "background-color: rgba(87, 150, 255, 255);"
						 "color:rgb(230, 230, 230);"
						 "}"
						 "QPushButton:hover{"
						 "border-radius:10px;"
						 "background-color: rgba(0, 128, 255, 255);"
						 "color:rgb(230, 230, 230);"
						 "}");
	reg = new QPushButton(w0);
	reg->setText("注册");
	reg->setGeometry(300, 120, 100, 40);
	reg->setStyleSheet("QPushButton{"
					   "border-radius:10px;"
					   "background-color: rgba(87, 150, 255, 255);"
					   "color:rgb(230, 230, 230);"
					   "}"
					   "QPushButton:hover{"
					   "border-radius:10px;"
					   "background-color: rgba(0, 128, 255, 255);"
					   "color:rgb(230, 230, 230);"
					   "}");
	connect(reg, &QPushButton::clicked, [&](){
		sta->setCurrentIndex(1);
	});

	w1 = new QWidget(sta);
	w1->setGeometry(0, 10, 480, 200);
	sta->addWidget(w1);

	reg_userName_label = new QLabel(w1);
	reg_userName_label->setText("用户名");
	reg_userName_label->setGeometry(100, 20, 50, 25);

	reg_userName_edit = new QLineEdit(w1);
	reg_userName_edit->setGeometry(170, 20, 200, 25);
	reg_userName_edit->setStyleSheet("QLineEdit {"
									 "border: 1px solid rgb(50, 50, 100);"
									 "border-radius: 6px;"
									 "background: white;"
									 "selection-background-color: blue;"
									 "font-size: 14px ;"
									 " }"
									 "QLineEdit:hover {"
									 "border: 1px solid blue;"
									 "}");
	reg_pwd_label = new QLabel(w1);
	reg_pwd_label->setText("密码");
	reg_pwd_label->setGeometry(100, 60, 50, 25);
	reg_pwd_edit = new QLineEdit(w1);
	reg_pwd_edit->setGeometry(170, 60, 200, 25);
	reg_pwd_edit->setStyleSheet("QLineEdit {"
								"border: 1px solid rgb(50, 50, 100);"
								"border-radius: 6px;"
								"background: white;"
								"selection-background-color: blue;"
								"font-size: 14px ;"
								" }"
								"QLineEdit:hover {"
								"border: 1px solid blue;"
								"}");
	reg_pwd_label2 = new QLabel(w1);
	reg_pwd_label2->setText("确认密码");
	reg_pwd_label2->setGeometry(100, 100, 60, 25);
	reg_pwd_edit2 = new QLineEdit(w1);
	reg_pwd_edit2->setGeometry(170, 100, 200, 25);
	reg_pwd_edit2->setStyleSheet("QLineEdit {"
								 "border: 1px solid rgb(50, 50, 100);"
								 "border-radius: 6px;"
								 "background: white;"
								 "selection-background-color: blue;"
								 "font-size: 14px ;"
								 " }"
								 "QLineEdit:hover {"
								 "border: 1px solid blue;"
								 "}");


	reg_pwd_edit->setEchoMode(QLineEdit::Password);
	reg_pwd_edit2->setEchoMode(QLineEdit::Password);

	ok = new QPushButton(w1);
	ok->setText("确认");
	ok->setGeometry(300, 140, 100, 40);
	ok->setStyleSheet("QPushButton{"
					  "border-radius:10px;"
					  "background-color: rgba(87, 150, 255, 255);"
					  "color:rgb(230, 230, 230);"
					  "}"
					  "QPushButton:hover{"
					  "border-radius:10px;"
					  "background-color: rgba(0, 128, 255, 255);"
					  "color:rgb(230, 230, 230);"
					  "}");
	back = new QPushButton(w1);
	back->setText("返回");
	back->setGeometry(100, 140, 100, 40);
	back->setStyleSheet("QPushButton{"
						"border-radius:10px;"
						"background-color: rgba(87, 150, 255, 255);"
						"color:rgb(230, 230, 230);"
						"}"
						"QPushButton:hover{"
						"border-radius:10px;"
						"background-color: rgba(0, 128, 255, 255);"
						"color:rgb(230, 230, 230);"
						"}");
	connect(back, &QPushButton::clicked, [&](){
		sta->setCurrentIndex(0);
	});
	sta->setCurrentIndex(0);
	startClient();

	connect(HTTPClient::Instance(), &HTTPClient::login, this, &CenterWidget::getLoginData);
	connect(HTTPClient::Instance(), &HTTPClient::reg, this, &CenterWidget::getRegData);

	connect(login, &QPushButton::clicked, [&](){
		if(userName_edit->text().isEmpty()) showErr("请填写用户名");
		else if(pwd_edit->text().isEmpty()) showErr("请填写密码");
		else{
				LoginBean request;
				request.userName = userName_edit->text();
				request.pwd = pwd_edit->text();
				if(!HTTPClient::Instance()->sendMsg("login", request.converFromObject())) showErr("网络连接错误");
		}
	});

	connect(ok, &QPushButton::clicked, [&](){
		if(reg_userName_edit->text().isEmpty()) showErr("用户名不能为空");
		else if(reg_pwd_edit->text().size() < 3) showErr("密码过短");
		else if(reg_pwd_edit->text() != reg_pwd_edit2->text()) showErr("两次密码不一致");
		else{
				LoginBean request;
				request.userName = reg_userName_edit->text();
				request.pwd = reg_pwd_edit->text();
				if(!HTTPClient::Instance()->sendMsg("register", request.converFromObject())) showErr("网络连接错误");
		}
	});
}

void CenterWidget::getLoginData(QString data)
{
	LoginResponseBean res;
	res.converFromJson(data);
	if(res.getCode() == 1 || res.getCode() == -1) showErr(res.getMsg());
	else emit loginSuccess(userName_edit->text(), res.token); //这就登录成功了
}

void CenterWidget::getRegData(QString data)
{
	ResponseDataBean res;
	res.converFromJson(data);
	if(res.getCode() == -1) showErr(res.getMsg());
	else showErr("注册成功");
}

void CenterWidget::paintEvent(QPaintEvent *event)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	p.drawPixmap(0, 0, 480, 180, QPixmap(":/std/sucai/bg.jpeg"));
}

void CenterWidget::startClient()
{
	HTTPClient::Instance()->setCallBack(this);
	if(!HTTPClient::Instance()->start("127.0.0.1")) {
		netWorkErr = new CommonWidget(this);
		netWorkErr->center()->getLabel()->setText("连接服务器失败！\n请输入新的ip：");
		netWorkErr->center()->getButton()->setText("确认");
		connect(netWorkErr->center()->getButton(), &QPushButton::clicked, [&](){
			if(!HTTPClient::Instance()->start(netWorkErr->center()->getEdit()->text())) {
					netWorkErr->center()->getButton()->setText("连接失败,重试");
			}else {
					netWorkErr->close();
			}
		});
		netWorkErr->exec();
	}
}

void CenterWidget::showErr(QString msg)
{
	loginErr = new CommonWidget(this);
	loginErr->center()->getLabel()->setText(msg);
	loginErr->center()->getEdit()->hide();
	loginErr->center()->getButton()->setText("确认");
	connect(loginErr->center()->getButton(), &QPushButton::clicked, loginErr, &QWidget::close);
	loginErr->exec();
}
