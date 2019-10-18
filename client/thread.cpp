#include "thread.h"

#include <QFile>
#include <QFileInfo>

Thread::Thread(QString userName, QString to, int token, QString path, QObject *parent) : QThread(parent)
{
	this->userName = userName;
	this->to = to;
	this->token = token;
	this->path = path;
}

void Thread::run()
{
	QFile file(path);
	file.open(QIODevice::ReadOnly);
	qint64 len = file.size();
	char *op = new char[len];
	QDataStream in(&file);
	in.readRawData(op, (int)len);
	SendMsgRequestBean request;
	request.userName = userName;
	request.token = token;
	request.to = to;
	QDateTime s = QDateTime::currentDateTime();

	//分块发送
	int num = len % 1024 == 0 ? len / 1024 : len / 1024 + 1;//块大小
	for(int i = 0, j = 1; i < len; i += 1024, j++) {
		request.msg = msgHead.arg(s.toTime_t()).arg(QString::number(j) + "*" + QString::number(num) + "*" + QFileInfo(file).fileName())
					  + QString(QLatin1String(op + i, i + 1024 <= len ? 1024 : len - i));
		emit sendMsg(request.converFromObject());
		emit updateMsg("正在发送(" + QString::number(j) + "/" + QString::number(num) + ")");
		QThread::msleep(250);//50ms发一个byte
		qDebug() << QString(QLatin1String(op + i, i + 1024 <= len ? 1024 : len - i)).toLatin1().size();
	}
	delete []op;
}
