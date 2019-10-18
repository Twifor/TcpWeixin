#ifndef THREAD_H
#define THREAD_H

#include <QObject>
#include <QThread>
#include <QDataStream>
#include "httpclient.h"
#include "databean.h"
#include <QDateTime>

class Thread : public QThread
{
	Q_OBJECT
public:
	explicit Thread(QString userName, QString to, int token, QString path, QObject *parent = nullptr);

signals:
	void updateMsg(QString msg);
	void sendMsg(QString msg);

public slots:

	// QThread interface
protected:
	void run() override;
	QString userName, to, path;
	int token;
	const QString msgHead = "{%1,%2}";
};

#endif // THREAD_H
