#include <QCoreApplication>
#include "httpserver.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	HttpServer *httpServer = new HttpServer;
	httpServer->start();

	return a.exec();
}
