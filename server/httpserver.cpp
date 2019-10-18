#include "httpserver.h"

HttpServer::HttpServer()
{
	srand(time(nullptr));
	server = new QTcpServer();
	map = new QMap<int, QTcpSocket*>();
	connect(server, &QTcpServer::newConnection, this, &HttpServer::newClient);//信号槽
}

HttpServer::~HttpServer()
{
	delete server;
	delete map;
}

void HttpServer::start()
{
	initDataBase();
	server->setMaxPendingConnections(30);//设置最大连接数
	server->listen(QHostAddress::Any, 2333);//监听本地地址,2333端口
}

void HttpServer::newClient()
{
	QTcpSocket *socket = server->nextPendingConnection();
	QObject::connect(socket, &QTcpSocket::disconnected, this, &HttpServer::disConnect);
	QObject::connect(socket, &QTcpSocket::readyRead, this, &HttpServer::getRequest);
}

void HttpServer::getRequest()
{
	QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
	QByteArray msg = socket->readAll();
	QString data = QString(msg), dd;
	QStringList list = data.split("\r\n");
	QStringList header = list[0].split(" ");
	int i = 0;
	for(i = 0; i < list.size(); i++) if(list[i] == "") break;
	for(i = i + 1; i < list.size(); i++) dd += list[i]; //剥离content
	if(header[0] == "POST") post(socket, header[1], dd); //处理POST请求
}

void HttpServer::disConnect()
{
	QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
	qDebug() << socket << "socket断开连接";
	socket->deleteLater();
}

//GET请求
void HttpServer::post(QTcpSocket *socket, QString path, QString data)
{
	path.remove(0, 1);
	if(path == "login") {//登录
		int id;
		LoginBean loginBean;
		LoginResponseBean responseBean;
		loginBean.converFromJson(data);
		int c = login(loginBean.getUserName(), loginBean.getPwd(), responseBean.token, id);

		responseBean.setCode(c);
		responseBean.setType("login");
		if(c == 0) {
			responseBean.setMsg("OK");
			insOnlineUser(id, socket);//添加这个socket进入平衡树
		}
		else if(c == 1) responseBean.setMsg("密码错误");
		else if(c == -1) responseBean.setMsg("无此用户");
		else if(c == 2) {
			responseBean.setMsg("已经登录了");
			gankIDFromTree(id);//去除之前的socket
			insOnlineUser(id, socket);
		}
		loginBean.converFromJson(data);
		QString json = responseBean.converFromObject();
		socket->write((commonMSG.arg("200").arg("OK").arg(json.toUtf8().size()) + json).toUtf8());
	}else if(path == "offline") {//离线
		NormalRequestBean offLineBean;
		NormalResponseBean response;
		offLineBean.converFromJson(data);
		int c = offLine(offLineBean.getUserName(), offLineBean.getToken());
		response.setCode(c);
		response.setType("offline");
		if(c == 0) {
			response.setMsg("OK");
		}
		else if(c == 1) response.setMsg("已经离线了");
		else if(c == 2) response.setMsg("token无效");
		else if(c == -1) response.setMsg("用户不存在");
		QString json = response.converFromObject();
		socket->write((commonMSG.arg("200").arg("OK").arg(json.toUtf8().size()) + json).toUtf8());
		socket->disconnectFromHost();
	}else if(path == "register") {//注册
		RegisterBean registerBean;
		NormalResponseBean response;
		registerBean.converFromJson(data);
		int c = registerNewUser(registerBean.getUserName(), registerBean.getPwd());
		response.setCode(c);
		response.setType("register");
		if(c == -1) response.setMsg("重复的用户名");
		else response.setMsg("注册成功");
		QString json = response.converFromObject();
		socket->write((commonMSG.arg("200").arg("OK").arg(json.toUtf8().size()) + json).toUtf8());
	}else if(path == "friends") {//获取朋友
		NormalRequestBean request;
		FriendResponseBean response;
		request.converFromJson(data);
		int c = getFriends(request.getUserName(), request.getToken(), response.l);
		response.setType("friends");
		response.setCode(c);
		if(c == 0) response.setMsg("OK");
		else if(c == -1) response.setMsg("用户不存在");
		else if(c == 1) response.setMsg("token无效");
		QString json = response.converFromObject();
		socket->write((commonMSG.arg("200").arg("OK").arg(json.toUtf8().size()) + json).toUtf8());
	}else if(path == "makefriend") {
		MakeFriendRequestBean request;
		NormalResponseBean response;
		request.converFromJson(data);
		int c = makeFriend(request.getUserName(), request.getWho(), request.getToken());
		response.setCode(c);
		response.setType("makefriend");
		if(c == 0) response.setMsg("发送请求成功");
		else if(c == -1) response.setMsg("用户不存在");
		else if(c == 1) response.setMsg("token无效");
		else if(c == 2) response.setMsg("已经是好友了");
		else if(c == 3) response.setMsg("目标用户不存在");
		else if(c == -2) response.setMsg("目标用户不在线");
		QString json = response.converFromObject();
		socket->write((commonMSG.arg("200").arg("OK").arg(json.toUtf8().size()) + json).toUtf8());
	}else if(path == "result") {
		ResultOfFriendBean request;
		NormalResponseBean response;
		request.converFromJson(data);
		int c = makeFriendCheck(request.getFrom(), request.getTo(), request.getToken(), request.getResult());
		response.setCode(c);
		response.setType("result");
		if(c == 0) {
			response.setMsg("发送结果成功");
			if(request.getResult()) addFriend(request.getFrom(), request.getTo());
		}
		else if(c == -1) response.setMsg("用户不存在");
		else if(c == 1) response.setMsg("token无效");
		else if(c == 2) response.setMsg("已经是好友了");
		else if(c == 3) response.setMsg("目标用户不存在");
		else if(c == -2) response.setMsg("目标用户不在线");
		QString json = response.converFromObject();
		socket->write((commonMSG.arg("200").arg("OK").arg(json.toUtf8().size()) + json).toUtf8());
	}else if(path == "delete") {
		MakeFriendRequestBean request;
		NormalResponseBean response;
		request.converFromJson(data);
		int c = delFriend(request.getUserName(), request.getWho(), request.getToken());
		response.setCode(c);
		response.setType("delete");
		if(c == 0) response.setMsg("删除好友成功");
		else if(c == 1) response.setMsg("token无效");
		else if(c == 2) response.setMsg("没有这个好友");
		else if(c == -1) response.setMsg("没有这个用户");
		QString json = response.converFromObject();
		socket->write((commonMSG.arg("200").arg("OK").arg(json.toUtf8().size()) + json).toUtf8());
	}else if(path == "sendmsg") {
		SendMsgRequestBean request;
		NormalResponseBean response;
		request.converFromJson(data);
		int c = sendToFriend(request.getUserName(), request.getTo(), request.getToken(), request.getMsg());
		response.setCode(c);
		response.setType("sendmsgres");
		if(c == 0) response.setMsg("发送成功");
		else if(c == -1) response.setMsg("没有这个用户");
		else if(c == 1) response.setMsg("token无效");
		else if(c == 2) response.setMsg("没有这个好友");
		else if(c == -2) response.setMsg("当前用户不在线");
		QString json = response.converFromObject();
		socket->write((commonMSG.arg("200").arg("OK").arg(json.toUtf8().size()) + json).toUtf8());
	}
}

int HttpServer::offLine(QString name, int token)//一位用户要离线
{
	int id;
	QSqlDatabase dataBase;
	if(QSqlDatabase::contains("C1")) dataBase = QSqlDatabase::database("C1");
	else dataBase = QSqlDatabase::addDatabase("QSQLITE", "C1");

	QSqlQuery query(dataBase);
	query.exec(QString("SELECT * FROM UserData WHERE userName = '%1'").arg(name));
	if(query.next()) {
		if(query.value(3).toInt() == 0) return 1; //表示已经离线了
		else if(query.value(4).toInt() != token) return 2; //表示token不一样
		id = query.value(1).toInt();
	}else return -1; //没有这个用户
	query.exec(QString("UPDATE UserData SET isOnline = 0 WHERE userName = '%1'").arg(name));
	qDebug() << "用户" << id << "离线";
	qDebug() << "当前在线用户" << map->size() - 1;
	map->erase(map->find(id));//删除这个用户
	return 0;
}

void HttpServer::initDataBase()
{
	QSqlDatabase dataBase = QSqlDatabase::addDatabase("QSQLITE", "C1");//连接QSQLITE数据库，连接号C1
	dataBase.setDatabaseName("userData.db");
	dataBase.setUserName("Liyh");
	dataBase.setPassword("66662333");

	if(!dataBase.open()) {
		qDebug() << "Can't open database.";
		return;
	}

	QSqlQuery query(dataBase);
	query.exec("CREATE TABLE IF NOT EXISTS UserData("//用户信息表
			   "userName VARCHAR(20) PRIMARY KEY,"
			   "ID INT,"
			   "passWord VARCHAR(20),"
			   "isOnline INT,"
			   "token INT"
			   ")");
	query.exec("CREATE TABLE IF NOT EXISTS UserFriends("//好友关系表
			   "userName VARCHAR(20),"
			   "friendName VARCHAR(20),"
			   "ID INT PRIMARY KEY"                                                                                                                                                                                                                                                                                //这个只用来标识
			   ")");
	//重启服务器强行断开所有用户的连接
	query.exec(QString("UPDATE UserData SET isOnline = 0"));
}

//login返回0表示ok，返回-1表示无用户，返回1表示密码错误，2表示已经上线了
int HttpServer::login(QString userName, QString pwd, int &token, int &id)
{
	QSqlDatabase dataBase;
	if(QSqlDatabase::contains("C1")) dataBase = QSqlDatabase::database("C1");
	else dataBase = QSqlDatabase::addDatabase("QSQLITE", "C1");

	QSqlQuery query(dataBase);
	query.exec(QString("SELECT * FROM UserData WHERE userName = '%1'").arg(userName));
	if(query.next()) {
		if(query.value(2).toString() != pwd) return 1;
		else if(query.value(3).toInt() == 1) {
			token = rand();
			id = query.value(1).toInt();
			query.exec(QString("UPDATE UserData SET "
							   "isOnline = 1, token = %1 "
							   "WHERE userName = '%4'").arg(token).arg(userName));
			return 2;
		}
		else {
			token = rand();
			id = query.value(1).toInt();
			query.exec(QString("UPDATE UserData SET "
							   "isOnline = 1, token = %1 "
							   "WHERE userName = '%4'").arg(token).arg(userName));
			return 0;
		}
	}else return -1;
}

void HttpServer::insOnlineUser(int id, QTcpSocket *socket)
{
	qDebug() << "新登入用户, id:" << id;
	qDebug() << "当前在线用户数量" << map->size() + 1;
	map->insert(id, socket);
}

void HttpServer::gankIDFromTree(int id)
{
	qDebug() << "用户" << id << "断开重连";
	map->erase(map->find(id));
}

int HttpServer::registerNewUser(QString userName, QString pwd)
{
	QSqlDatabase dataBase;
	if(QSqlDatabase::contains("C1")) dataBase = QSqlDatabase::database("C1");
	else dataBase = QSqlDatabase::addDatabase("QSQLITE", "C1");

	QSqlQuery query(dataBase);
	query.exec(QString("SELECT * FROM UserData WHERE userName = '%1'").arg(userName));
	if(query.next()) return -1; //表示冲突
	else{
		query.exec("SELECT MAX(ID) FROM UserData");//找到目前最大的ID
		query.first();//移动到第一条记录
		int id = query.value(0).toInt() + 1;
		query.exec(QString("INSERT INTO UserData VALUES('%1',%2,'%3',0,0)").arg(userName).arg(id).arg(pwd));
		return 0;
	}
}

int HttpServer::getFriends(QString userName, int token, QList<QString> &list)
{
	QSqlDatabase dataBase;
	if(QSqlDatabase::contains("C1")) dataBase = QSqlDatabase::database("C1");
	else dataBase = QSqlDatabase::addDatabase("QSQLITE", "C1");

	QSqlQuery query(dataBase);
	query.exec(QString("SELECT * FROM UserData WHERE userName = '%1'").arg(userName));
	if(query.next()) {
		if(query.value(4).toInt() != token) return 1; //token无效
		query.exec(QString("SELECT friendName FROM UserFriends WHERE userName = '%1'").arg(userName));
		while(query.next()) list.append(query.value(0).toString());
		return 0;
	}else return -1; //没有这个用户
}

void HttpServer::addFriend(QString x, QString y)
{
	QSqlDatabase dataBase;
	if(QSqlDatabase::contains("C1")) dataBase = QSqlDatabase::database("C1");
	else dataBase = QSqlDatabase::addDatabase("QSQLITE", "C1");

	QSqlQuery query(dataBase);
	query.exec("SELECT MAX(ID) FROM UserFriends");
	query.first();
	int id = query.value(0).toInt();
	query.exec(QString("INSERT INTO UserFriends VALUES('%1','%2',%3)").arg(x).arg(y).arg(id + 1));
	query.exec(QString("INSERT INTO UserFriends VALUES('%1','%2',%3)").arg(y).arg(x).arg(id + 2));
}

int HttpServer::makeFriend(QString userName, QString who, int token)
{
	QSqlDatabase dataBase;
	if(QSqlDatabase::contains("C1")) dataBase = QSqlDatabase::database("C1");
	else dataBase = QSqlDatabase::addDatabase("QSQLITE", "C1");

	QSqlQuery query(dataBase);
	query.exec(QString("SELECT * FROM UserData WHERE userName = '%1'").arg(userName));
	if(query.next()) {
		if(query.value(4).toInt() != token) return 1; //token无效
		query.exec(QString("SELECT * FROM UserFriends WHERE userName = '%1' AND friendName = '%2'").arg(userName).arg(who));
		if(query.next()) return 2; //已经是好友了
		query.exec(QString("SELECT ID FROM UserData WHERE userName = '%1'").arg(who));
		if(query.next()) {
			QMap<int, QTcpSocket*>::iterator it = map->find(query.value(0).toInt());
			if(it == map->end()) return -2;
			QTcpSocket *s = it.value();
			sendFriend(s, userName);
			return 0;
		}else return 3;
	}else return -1; //没有这个用户
}

int HttpServer::makeFriendCheck(QString userName, QString who, int token, int isOk)
{
	QSqlDatabase dataBase;
	if(QSqlDatabase::contains("C1")) dataBase = QSqlDatabase::database("C1");
	else dataBase = QSqlDatabase::addDatabase("QSQLITE", "C1");

	QSqlQuery query(dataBase);
	query.exec(QString("SELECT * FROM UserData WHERE userName = '%1'").arg(userName));
	if(query.next()) {
		if(query.value(4).toInt() != token) return 1; //token无效
		query.exec(QString("SELECT * FROM UserFriends WHERE userName = '%1' AND friendName = '%2'").arg(userName).arg(who));
		if(query.next()) return 2; //已经是好友了
		query.exec(QString("SELECT ID FROM UserData WHERE userName = '%1'").arg(who));
		if(query.next()) {
			QMap<int, QTcpSocket*>::iterator it = map->find(query.value(0).toInt());
			if(it == map->end()) return -2;
			QTcpSocket *s = it.value();
			sendRes(s, userName, isOk);
			return 0;
		}else return 3;
	}else return -1; //没有这个用户
}

void HttpServer::sendFriend(QTcpSocket *socket, QString from)//发送好友请求
{
	MakeFriendResponseBean response;
	response.setCode(0);
	response.setMsg("好友请求");
	response.setType("friendrequest");
	response.from = from;
	QString json = response.converFromObject();
	socket->write((commonMSG.arg("200").arg("OK").arg(json.toUtf8().size()) + json).toUtf8());
}

void HttpServer::sendRes(QTcpSocket *socket, QString from, int isOk)
{
	NormalResponseBean response;
	response.setCode(isOk);
	response.setMsg(from);
	response.setType("makefriendres");
	QString json = response.converFromObject();
	socket->write((commonMSG.arg("200").arg("OK").arg(json.toUtf8().size()) + json).toUtf8());
}

int HttpServer::delFriend(QString userName, QString fr, int token)
{
	QSqlDatabase dataBase;
	if(QSqlDatabase::contains("C1")) dataBase = QSqlDatabase::database("C1");
	else dataBase = QSqlDatabase::addDatabase("QSQLITE", "C1");

	QSqlQuery query(dataBase);
	query.exec(QString("SELECT * FROM UserData WHERE userName = '%1'").arg(userName));
	if(query.next()) {
		if(query.value(4).toInt() != token) return 1; //token无效
		query.exec(QString("SELECT * FROM UserFriends WHERE userName = '%1' AND friendName = '%2'").arg(userName).arg(fr));
		if(query.next()) {
			query.exec(QString("DELETE FROM UserFriends WHERE userName = '%1' AND friendName = '%2'").arg(userName).arg(fr));
			query.exec(QString("DELETE FROM UserFriends WHERE userName = '%1' AND friendName = '%2'").arg(fr).arg(userName));
			return 0;
		}else return 2; //没有这个好友
	}else return -1; //没有这个用户
}

int HttpServer::sendToFriend(QString userName, QString to, int token, QString msg)
{
	QSqlDatabase dataBase;
	if(QSqlDatabase::contains("C1")) dataBase = QSqlDatabase::database("C1");
	else dataBase = QSqlDatabase::addDatabase("QSQLITE", "C1");

	QSqlQuery query(dataBase);
	query.exec(QString("SELECT * FROM UserData WHERE userName = '%1'").arg(userName));
	qDebug() << userName << "发出请求" << "to" << to;
	if(query.next()) {
		if(query.value(4).toInt() != token) return 1; //token无效
		query.exec(QString("SELECT * FROM UserFriends WHERE userName = '%1' AND friendName = '%2'").arg(userName).arg(to));
		if(query.next()) {
			query.exec(QString("SELECT ID FROM UserData WHERE userName = '%1'").arg(to));
			query.first();
			QMap<int, QTcpSocket*>::iterator it = map->find(query.value(0).toInt());
			if(it == map->end()) return -2;
			QTcpSocket *s = it.value();
			senTo(s, userName, msg);
			return 0;
		}else return 2; //没有这个好友
	}else return -1; //没有这个用户
}

void HttpServer::senTo(QTcpSocket *socket, QString from, QString msg)
{
	MakeFriendResponseBean response;
	response.setCode(0);
	response.setMsg(msg);
	response.setType("msg");
	response.from = from;

	QString json = response.converFromObject();
	socket->write((commonMSG.arg("200").arg("OK").arg(json.toUtf8().size()) + json).toUtf8());
}
