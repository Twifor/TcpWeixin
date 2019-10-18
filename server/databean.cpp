#include "databean.h"

void ResponseDataBean::solveWith(QJsonObject &object)
{
	object.insert("code", code);
	object.insert("msg", msg);
	object.insert("type", type);
}

ResponseDataBean::ResponseDataBean() : QObject ()
{

}

void ResponseDataBean::setCode(int c)
{
	code = c;
}

void ResponseDataBean::setMsg(QString ms)
{
	msg = ms;
}

void ResponseDataBean::setType(QString s)
{
	type = s;
}

RequestDataBean::RequestDataBean() : QObject ()
{

}

void LoginBean::converFromJson(QString json)
{
	QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8());
	QJsonObject object = jsonDoc.object();
	userName = object["userName"].toString();
	pwd = object["pwd"].toString();
}

QString LoginBean::getUserName()
{
	return userName;
}

QString LoginBean::getPwd()
{
	return pwd;
}

QString LoginResponseBean::converFromObject()
{
	QJsonObject ansJson;
	solveWith(ansJson);
	ansJson.insert("token", token);

	QJsonDocument document;
	document.setObject(ansJson);
	QByteArray byteArray = document.toJson(QJsonDocument::Compact);
	return QString(byteArray);
}

QString NormalRequestBean::getUserName()
{
	return userName;
}

int NormalRequestBean::getToken()
{
	return token;
}

void NormalRequestBean::converFromJson(QString json)
{
	QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8());
	QJsonObject object = jsonDoc.object();
	userName = object["userName"].toString();
	token = object["token"].toInt();
}

QString NormalResponseBean::converFromObject()
{
	QJsonObject ansJson;
	solveWith(ansJson);

	QJsonDocument document;
	document.setObject(ansJson);
	QByteArray byteArray = document.toJson(QJsonDocument::Compact);
	return QString(byteArray);
}

QString RegisterBean::getUserName()
{
	return userName;
}

QString RegisterBean::getPwd()
{
	return pwd;
}

void RegisterBean::converFromJson(QString json)
{
	QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8());
	QJsonObject object = jsonDoc.object();
	userName = object["userName"].toString();
	pwd = object["pwd"].toString();
}

QString FriendResponseBean::converFromObject()
{
	QJsonObject ansJson;
	solveWith(ansJson);
	QJsonArray array;
	for(QString s:l) array.push_back(s);
	ansJson.insert("friends", array);

	QJsonDocument document;
	document.setObject(ansJson);
	QByteArray byteArray = document.toJson(QJsonDocument::Compact);
	return QString(byteArray);
}

QString MakeFriendRequestBean::getWho()
{
	return who;
}

void MakeFriendRequestBean::converFromJson(QString json)
{
	NormalRequestBean::converFromJson(json);
	QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8());
	QJsonObject object = jsonDoc.object();
	who = object["who"].toString();
}

QString MakeFriendResponseBean::converFromObject()
{
	QJsonObject ansJson;
	solveWith(ansJson);
	ansJson.insert("from", from);

	QJsonDocument document;
	document.setObject(ansJson);
	QByteArray byteArray = document.toJson(QJsonDocument::Compact);
	return QString(byteArray);
}

int ResultOfFriendBean::getResult()
{
	return isOk;
}

int ResultOfFriendBean::getToken()
{
	return token;
}

QString ResultOfFriendBean::getFrom()
{
	return from;
}

QString ResultOfFriendBean::getTo()
{
	return to;
}

void ResultOfFriendBean::converFromJson(QString json)
{
	QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8());
	QJsonObject object = jsonDoc.object();
	isOk = object["isOk"].toInt();
	from = object["from"].toString();
	to = object["to"].toString();
	token = object["token"].toInt();
}

QString SendMsgRequestBean::getTo()
{
	return to;
}

QString SendMsgRequestBean::getMsg()
{
	return msg;
}

void SendMsgRequestBean::converFromJson(QString json)
{
	NormalRequestBean::converFromJson(json);
	QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8());
	QJsonObject object = jsonDoc.object();
	to = object["to"].toString();
	msg = object["msg"].toString();
}
