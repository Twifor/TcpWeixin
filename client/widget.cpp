#include "widget.h"
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#include <QDebug>

LoginWidget::LoginWidget(QWidget *parent)
	: QWidget(parent)
{
	setWindowFlag(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	setFixedSize(500, 400);
	mainWidget = new CenterWidget(this);
}

CenterWidget *LoginWidget::center()
{
	return mainWidget;
}

LoginWidget::~LoginWidget()
{

}

void LoginWidget::mousePressEvent(QMouseEvent *event)
{
    pos = event->globalPos();
}

void LoginWidget::mouseMoveEvent(QMouseEvent *event)
{
	QPointF now = event->globalPos() - pos;
	move(geometry().x() + now.x(), geometry().y() + now.y());
	pos = event->globalPos();
}
