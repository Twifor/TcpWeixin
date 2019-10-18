#ifndef COMMONWIDGET_H
#define COMMONWIDGET_H

#include <QWidget>
#include "commoncenterwidget.h"
#include <QMouseEvent>
#include <QDebug>
#include <QDialog>

class CommonWidget : public QDialog//就是一个好看的窗口
{
	Q_OBJECT
public:
	explicit CommonWidget(QWidget *parent = nullptr);
	CommonCenterWidget *center();
	~CommonWidget();

signals:

public slots:

	// QWidget interface
protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

private:
	CommonCenterWidget *mainWidget;
	QPointF pos;

	// QWidget interface
protected:
	void closeEvent(QCloseEvent *event);
};

#endif // COMMONWIDGET_H
