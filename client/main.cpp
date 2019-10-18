#include "widget.h"
#include <QApplication>
#include "mainwidget.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	LoginWidget w;
	MainWidget ww;
	QObject::connect(w.center(), &CenterWidget::loginSuccess, &ww, &MainWidget::showThis);
	QObject::connect(w.center(), &CenterWidget::loginSuccess, &w, &LoginWidget::close);

	w.show();
	return a.exec();
}
