#include "databean.h"
#include "frienditem.h"

#include <QAction>

FriendItem::FriendItem(QWidget *parent, QListWidget *list) : QObject(parent), QListWidgetItem (list)
{
	time_t = 0;
	setSizeHint(QSize(175, 80));
	setIcon(QIcon(":/std/sucai/aa.png"));
	parList = list;
	par = parent;
	mainList = new QListWidget(parent);
	mainList->setGeometry(230, 50, 530, 360);
	mainList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	mainList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	mainList->hide();

	del = new QAction("删除好友", list);
	add = new QAction("添加好友", list);

	connect(add, &QAction::triggered, [&](){
		emit addFriend();
	});
	connect(del, &QAction::triggered, [&](){
		emit delFriend(this->text());
	});
}

void FriendItem::showMenu(QPoint pos)
{
	QMenu menu(par);
	menu.setStyleSheet("QMenu::item:selected {"
					   "background-color: #2dabf9;"
					   "}");
	menu.addAction(add);
	menu.addAction(del);
	menu.exec(pos);
}

uint FriendItem::getTime_t()
{
	return time_t;
}

void FriendItem::setTime_t(uint s)
{
	time_t = s;
}

QListWidget *FriendItem::getList()
{
	return mainList;
}

FriendItem::~FriendItem()
{
	delete mainList;
}
