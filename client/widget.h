#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QGraphicsDropShadowEffect>
#include "centerwidget.h"

class LoginWidget : public QWidget
{
	Q_OBJECT

public:
	LoginWidget(QWidget *parent = 0);
	CenterWidget *center();
	~LoginWidget();

	// QWidget interface
private:
	CenterWidget *mainWidget;
	QPointF pos;

	// QWidget interface
protected:
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // WIDGET_H
