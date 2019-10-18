#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QListWidget>
#include "httpclient.h"
#include "maincenterwidget.h"

class MainWidget : public QWidget
{
	Q_OBJECT
public:
	explicit MainWidget(QWidget *parent = nullptr);

signals:

public slots:
	void showThis(QString a, int t);

private:
	MainCenterWidget *center;

protected:
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	QPointF pos;

	// QWidget interface
protected:
	void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWIDGET_H
