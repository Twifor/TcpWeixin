#ifndef COMMONCENTERWIDGET_H
#define COMMONCENTERWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QStyleOption>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QDialog>

//窗口核心类，因为某些奇怪的原因，我没有把login的窗口封装起来
//然后后来就后悔了
//才有了这个类
class CommonCenterWidget : public QWidget//这就叫你中有我，我中有你(误
{
	Q_OBJECT
public:
	explicit CommonCenterWidget(QWidget *parent = nullptr);
	QPushButton *getButton();
	QLineEdit *getEdit();
	QLabel *getLabel();

signals:

public slots:

	// QWidget interface
protected:
	void paintEvent(QPaintEvent *event);
	QGraphicsDropShadowEffect *effect;
	QPushButton *exit_btn;
	QLineEdit *edit;
	QPushButton *button;
	QLabel *label;
};

#endif // COMMONCENTERWIDGET_H
