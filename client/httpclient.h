#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H
#include <QTcpSocket>
#include <QObject>
#include <QHostAddress>
#include "commonwidget.h"
#include "databean.h"

class HTTPClient : public QObject//单例模式管理网络
{
	Q_OBJECT
public:
	static HTTPClient *Instance();
	bool start(QString ip);
	void setCallBack(QWidget *p);
	int sendMsg(QString type, QString content);
	int sendRawMsg(QString type, QString content);

public slots:
	void solve();

signals:
	void login(QString response);
	void reg(QString response);
	void friends(QString response);
	void mkFriend(QString res);
	void delFriend(QString res);
	void friendRequest(QString res);
	void mkFriendRes(QString res);
	void msgData(QString res);
	void msgRes(QString res);

private:
	QTcpSocket *socket;
	HTTPClient();
	static HTTPClient *INSTANCE;
	QWidget *p;
	CommonWidget *netWorkErr;
	QString commonMSG = "POST /%1 HTTP/1.1\r\n"
						"Content-Type: application/json;charset=utf-8\r\n"
						"Connection: keep-alive\r\n"
						"Content-Length: %2\r\n\r\n";
	void readData();
};

#endif // HTTPCLIENT_H
