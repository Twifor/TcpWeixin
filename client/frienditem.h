#ifndef FRIENDITEM_H
#define FRIENDITEM_H

#include <QObject>
#include <QListWidgetItem>
#include <QListWidget>
#include <QMenu>
#include "commonwidget.h"
#include "msgitem.h"

class FriendItem : public QObject, public QListWidgetItem
{
	Q_OBJECT
public:
	explicit FriendItem(QWidget *parent = nullptr, QListWidget *list = nullptr);
	void showMenu(QPoint pos);
	uint getTime_t();
	void setTime_t(uint s);
	QListWidget *getList();
	~FriendItem();

signals:
	void addFriend();
	void delFriend(QString name);

private:
	QListWidget *mainList;
	QListWidget *parList;
	QWidget *par;
	QAction *del, *add;
	uint time_t;
};

#endif // FRIENDITEM_H
