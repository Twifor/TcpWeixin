#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
	setAttribute(Qt::WA_TranslucentBackground);

	setFixedSize(800, 600);
	center = new MainCenterWidget(this);
}

void MainWidget::showThis(QString a, int t)
{
	center->setUserName(a);
	center->setToken(t);
	center->start();
	show();
}

void MainWidget::mousePressEvent(QMouseEvent *event)
{
	pos = event->globalPos();
}

void MainWidget::mouseMoveEvent(QMouseEvent *event)
{
	QPointF now = event->globalPos() - pos;
	move(geometry().x() + now.x(), geometry().y() + now.y());
	pos = event->globalPos();
}

void MainWidget::closeEvent(QCloseEvent *event)
{
	NormalRequestBean res;
	res.userName = center->getUserName();
	res.token = center->getToken();
	HTTPClient::Instance()->sendMsg("offline", res.converFromObject());
	event->accept();
}
