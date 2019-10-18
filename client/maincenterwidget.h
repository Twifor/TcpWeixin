#ifndef MAINCENTERWIDGET_H
#define MAINCENTERWIDGET_H

#include <QGraphicsDropShadowEffect>
#include <QPushButton>
#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include <QListWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QMenu>
#include "httpclient.h"
#include "commonwidget.h"
#include "frienditem.h"
#include <QTextEdit>
#include "msgitem.h"
#include <QMap>
#include <QDateTime>
#include <QFileDialog>
#include "thread.h"

class MainCenterWidget : public QWidget
{
	Q_OBJECT
public:
	explicit MainCenterWidget( QWidget *parent = nullptr);
	~MainCenterWidget();
	void setUserName(QString u);
	void setToken(int t);
	void start();
	QString getUserName();
	int getToken();

signals:

public slots:
	void showMenu(const QPoint&pt);
	void readFriends(QString data);
	void addFriend();
	void mkFriend(QString data);
	void delFriend(QString name);
	void delFriendRes(QString data);
	void newFriend(QString data);
	void mkFriendRes(QString data);
	void changeItem(QListWidgetItem *i);
	void msgData(QString data);
	void showMsgRes(QString data);

protected:
	void paintEvent(QPaintEvent *event);
	QGraphicsDropShadowEffect *effect;
	QPushButton *exit_btn, *min_btn, *send_btn, *send_file;
	QListWidget *list;
	QString userName;
	int token;
	QAction *add;
	CommonWidget *w, *newFr, *sendFile;
	FriendItem *now;
	QTextEdit *input;
	QListWidget *output;
	QMap<QString, FriendItem*> *map;

private:
	const QString msgHead = "{%1,%2}";
};

#endif // MAINCENTERWIDGET_H
