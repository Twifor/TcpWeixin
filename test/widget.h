#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
	Q_OBJECT

public:
	explicit Widget(QWidget *parent = nullptr);
	~Widget();

private:
	Ui::Widget *ui;
	QTcpSocket *socket;
	QString commonMSG = "%1 %2 HTTP/1.1\r\n"
						"Host: 127.0.0.1:2333\r\n"
						"Connection: keep-alive\r\n"
						"Accept-Encoding:  gzip\r\n"
						"Content-Type: text/plain\r\n"
						"Content-Length: %3\r\n\r\n";
};

#endif // WIDGET_H
