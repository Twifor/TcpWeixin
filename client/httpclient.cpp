#include "httpclient.h"

HTTPClient *HTTPClient::INSTANCE = nullptr;
HTTPClient *HTTPClient::Instance()
{
	if(INSTANCE == nullptr) return INSTANCE = new HTTPClient;
	return INSTANCE;
}

bool HTTPClient::start(QString ip)
{
	socket->connectToHost(ip, 2333);
	if(socket->waitForConnected(2000)) {
		connect(socket, &QTcpSocket::disconnected, this, &HTTPClient::solve);
		connect(socket, &QTcpSocket::readyRead, this, &HTTPClient::readData);
		return true;
	}
	else return false;
}

void HTTPClient::setCallBack(QWidget *p)
{
	this->p = p;
}

int HTTPClient::sendMsg(QString type, QString content)
{
	if(socket->state() != QTcpSocket::ConnectedState) return 0;
	QString msg = commonMSG.arg(type).arg(content.toUtf8().size()) + content;
	socket->write(msg.toUtf8());
	socket->flush();
	return 1;
}

int HTTPClient::sendRawMsg(QString type, QString content)
{
	if(socket->state() != QTcpSocket::ConnectedState) return 0;
	QString msg = commonMSG.arg(type).arg(content.toLatin1().size()) + content;
	socket->write(msg.toLatin1());
	socket->flush();
	return 1;
}

void HTTPClient::solve()
{
	netWorkErr = new CommonWidget(p);
	netWorkErr->center()->getLabel()->setText("与服务器连接断开");
	netWorkErr->center()->getEdit()->hide();
	netWorkErr->center()->getButton()->setText("确认");
	connect(netWorkErr->center()->getButton(), &QPushButton::clicked, [&](){
		netWorkErr->close();
	});
	netWorkErr->exec();
}

HTTPClient::HTTPClient() : QObject(nullptr)
{
	socket = new QTcpSocket(this);
}

void HTTPClient::readData()
{
	QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
	QByteArray msg = socket->readAll();
	QString data = QString(msg), dd;
	QStringList list = data.split("\r\n");
	int i = 0;
	for(i = 0; i < list.size(); i++) if(list[i] == "") break;
	for(i = i + 1; i < list.size(); i++) dd += list[i]; //剥离content
	ResponseDataBean response;
	response.converFromJson(dd);
	if(response.getType() == "login") emit login(dd);
	else if(response.getType() == "register") emit reg(dd);
	else if(response.getType() == "friends") emit friends(dd);
	else if(response.getType() == "makefriend") emit mkFriend(dd);
	else if(response.getType() == "delete") emit delFriend(dd);
	else if(response.getType() == "friendrequest") emit friendRequest(dd);
	else if(response.getType() == "makefriendres") emit mkFriendRes(dd);
	else if(response.getType() == "msg") emit msgData(dd);
	else if(response.getType() == "sendmsgres") emit msgRes(dd);

}
