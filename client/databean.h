#ifndef DATABEAN_H
#define DATABEAN_H
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QList>
#include <QJsonArray>

class ResponseDataBean : public QObject
{
	Q_OBJECT
protected:
	int code;//返回码
	QString msg;//返回信息
	QString type;//类型
	void solveWith(QString json);

public:
	ResponseDataBean();
	int getCode();
	QString getMsg();
	QString getType();
	virtual void converFromJson(QString json);
};

class RequestDataBean : public QObject {
	Q_OBJECT
public:
	RequestDataBean();
	virtual QString converFromObject() = 0;
};

class LoginBean : public RequestDataBean {

public:
	QString userName, pwd;
	QString converFromObject() override;
};

class LoginResponseBean : public ResponseDataBean {
public:
	void converFromJson(QString json) override;
	int token;
};

//class NormalResponseBean : public ResponseDataBean {
//public:
//	QString converFromObject() override;
//};

class NormalRequestBean : public RequestDataBean {
public:
	QString userName;
	int token;
	QString converFromObject() override;
};

class FriendResponseBean : public ResponseDataBean {
private:
	QList<QString> l;
public:
	QList<QString> getList();
	void converFromJson(QString json) override;
};

class MakeFriendRequestBean : public NormalRequestBean {
public:
	QString who;
	QString converFromObject() override;
};

class MakeFriendResponseBean : public ResponseDataBean {
private:
	QString from;
public:
	QString getFrom();
	void converFromJson(QString json) override;
};

class ResultOfFriendBean : public RequestDataBean {
public:
	int isOk, token;
	QString from, to;
	QString converFromObject() override;
};

class SendMsgRequestBean : public NormalRequestBean {
public:
	QString to, msg;
	QString converFromObject() override;
};

#endif // DATABEAN_H
