#include "databean.h"

void ResponseDataBean::solveWith(QString json)
{
	QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8());
	QJsonObject object = jsonDoc.object();
	code = object["code"].toInt();
	msg = object["msg"].toString();
	type = object["type"].toString();
}

ResponseDataBean::ResponseDataBean() : QObject ()
{

}

int ResponseDataBean::getCode()
{
	return code;
}

QString ResponseDataBean::getMsg()
{
	return msg;
}

QString ResponseDataBean::getType()
{
	return type;
}

void ResponseDataBean::converFromJson(QString json)
{
	solveWith(json);
}

RequestDataBean::RequestDataBean() : QObject ()
{

}

QString LoginBean::converFromObject()
{
	QJsonObject ansJson;
	ansJson.insert("userName", userName);
	ansJson.insert("pwd", pwd);

	QJsonDocument document;
	document.setObject(ansJson);
	QByteArray byteArray = document.toJson(QJsonDocument::Compact);
	return QString(byteArray);
}

void LoginResponseBean::converFromJson(QString json)
{
	solveWith(json);
	QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8());
	QJsonObject object = jsonDoc.object();
	token = object["token"].toInt();
}

QList<QString> FriendResponseBean::getList()
{
	return l;
}

void FriendResponseBean::converFromJson(QString json)
{
	solveWith(json);
	QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8());
	QJsonObject object = jsonDoc.object();
	QJsonArray array = object["friends"].toArray();
	for(QJsonValueRef s:array) l.append(s.toString());
}

QString NormalRequestBean::converFromObject()
{
	QJsonObject ansJson;
	ansJson.insert("userName", userName);
	ansJson.insert("token", token);

	QJsonDocument document;
	document.setObject(ansJson);
	QByteArray byteArray = document.toJson(QJsonDocument::Compact);
	return QString(byteArray);
}

QString MakeFriendRequestBean::converFromObject()
{
	QJsonObject ansJson;
	ansJson.insert("userName", userName);
	ansJson.insert("token", token);
	ansJson.insert("who", who);

	QJsonDocument document;
	document.setObject(ansJson);
	QByteArray byteArray = document.toJson(QJsonDocument::Compact);
	return QString(byteArray);
}

QString ResultOfFriendBean::converFromObject()
{
	QJsonObject ansJson;
	ansJson.insert("from", from);
	ansJson.insert("to", to);
	ansJson.insert("isOk", isOk);
	ansJson.insert("token", token);

	QJsonDocument document;
	document.setObject(ansJson);
	QByteArray byteArray = document.toJson(QJsonDocument::Compact);
	return QString(byteArray);
}

QString MakeFriendResponseBean::getFrom()
{
	return from;
}

void MakeFriendResponseBean::converFromJson(QString json)
{
	solveWith(json);
	QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8());
	QJsonObject object = jsonDoc.object();
	from = object["from"].toString();
}

QString SendMsgRequestBean::converFromObject()
{
	QJsonObject ansJson;
	ansJson.insert("userName", userName);
	ansJson.insert("to", to);
	ansJson.insert("token", token);
	ansJson.insert("msg", msg);

	QJsonDocument document;
	document.setObject(ansJson);
	QByteArray byteArray = document.toJson(QJsonDocument::Compact);
	return QString(byteArray);
}
