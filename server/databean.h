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
	void solveWith(QJsonObject &object);

public:
	ResponseDataBean();
	void setCode(int c);
	void setMsg(QString ms);
	void setType(QString s);
	virtual QString converFromObject() = 0;
};

class RequestDataBean : public QObject {
	Q_OBJECT
public:
	RequestDataBean();
	virtual void converFromJson(QString json) = 0;
};

class LoginBean : public RequestDataBean {

private:
	QString userName, pwd;

public:
	void converFromJson(QString json) override;
	QString getUserName();
	QString getPwd();
};

class LoginResponseBean : public ResponseDataBean {
public:
	QString converFromObject() override;
	int token;
};

class NormalResponseBean : public ResponseDataBean {
public:
	QString converFromObject() override;
};

class NormalRequestBean : public RequestDataBean {
private:
	QString userName;
	int token;
public:
	QString getUserName();
	int getToken();
	void converFromJson(QString json) override;
};

class RegisterBean : public RequestDataBean {
private:
	QString userName, pwd;
public:
	QString getUserName();
	QString getPwd();
	void converFromJson(QString json) override;
};

class FriendResponseBean : public ResponseDataBean {
public:
	QList<QString> l;
	QString converFromObject() override;
};

class MakeFriendRequestBean : public NormalRequestBean {
private:
	QString who;
public:
	QString getWho();
	void converFromJson(QString json) override;
};

class MakeFriendResponseBean : public ResponseDataBean {
public:
	QString from;
	QString converFromObject() override;
};

class ResultOfFriendBean : public RequestDataBean {
private:
	int isOk, token;
	QString from, to;
public:
	int getResult();
	int getToken();
	QString getFrom();
	QString getTo();
	void converFromJson(QString json) override;
};

class SendMsgRequestBean : public NormalRequestBean {
private:
	QString to, msg;
public:
	QString getTo();
	QString getMsg();
	void converFromJson(QString json) override;
};

#endif // DATABEAN_H
