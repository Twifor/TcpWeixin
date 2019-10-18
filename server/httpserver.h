#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QFile>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "databean.h"
#include <QMap>

class HttpServer : public QObject
{
	Q_OBJECT
public:
	HttpServer();
	~HttpServer();
	void start();

public slots:
	void newClient();
	void getRequest();
	void disConnect();

private:
	QTcpServer *server;
	QString commonMSG = "HTTP/1.1 %1 %2\r\n"
						"Content-Type: application/json;charset=utf-8\r\n"
						"Connection: keep-alive\r\n"
						"Content-Length: %3\r\n\r\n";
//	SplayTree<UserData> *splayTree;


protected:
	void post(QTcpSocket *socket, QString path, QString data);
	int offLine(QString name, int token);
	void initDataBase();
	int login(QString userName, QString pwd, int &token, int &id);
	void insOnlineUser(int id, QTcpSocket *socket);
	void gankIDFromTree(int id);
	int registerNewUser(QString userName, QString pwd);
	int getFriends(QString userName, int token, QList<QString> &list);
	void addFriend(QString x, QString y);
	int makeFriend(QString userName, QString who, int token);
	int makeFriendCheck(QString userName, QString who, int token, int isOk);
	void sendFriend(QTcpSocket *socket, QString from);
	void sendRes(QTcpSocket *socket, QString from, int isOk);
	int delFriend(QString userName, QString fr, int token);
	int sendToFriend(QString userName, QString to, int token, QString msg);
	void senTo(QTcpSocket *socket, QString from, QString msg);
	QMap<int, QTcpSocket *> *map;
};

#endif // HTTPSERVER_H
