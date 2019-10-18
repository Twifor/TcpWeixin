#include "commonwidget.h"

CommonWidget::CommonWidget(QWidget *parent) : QDialog(parent)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
	setAttribute(Qt::WA_TranslucentBackground);
	setFixedSize(260, 200);
	mainWidget = new CommonCenterWidget(this);
}

CommonCenterWidget *CommonWidget::center()
{
	return mainWidget;
}

CommonWidget::~CommonWidget()
{
}
void CommonWidget::mousePressEvent(QMouseEvent *event)
{
	pos = event->globalPos();
}

void CommonWidget::mouseMoveEvent(QMouseEvent *event)
{
	QPointF now = event->globalPos() - pos;
	move(geometry().x() + now.x(), geometry().y() + now.y());
	pos = event->globalPos();
}

void CommonWidget::closeEvent(QCloseEvent *event)
{
	deleteLater();
}
