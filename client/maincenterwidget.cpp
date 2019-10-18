#include "maincenterwidget.h"

#include <QPixmap>

MainCenterWidget::MainCenterWidget(QWidget *parent) : QWidget(parent)
{
	setGeometry(10, 10, 780, 580);
	setStyleSheet("QWidget{"
				  "background-color: rgb(250, 250, 250);"
				  "border-top-left-radius:15px;"
				  "border-top-right-radius:15px;"
				  "border-bottom-right-radius:15px;"
				  "border-bottom-left-radius:15px;"
				  "}");
	exit_btn = new QPushButton(this);
	exit_btn->setGeometry(740, 5, 30, 30);
	exit_btn->setStyleSheet("QPushButton{"
							"background:url(:/std/sucai/btn_exit1.png) -120px 0px no-repeat;"
							"}"
							"QPushButton:hover{"
							"background:url(:/std/sucai/btn_exit2.png) -120px 0px no-repeat;"
							"}"
							"QPushButton:pressed {"
							"background:url(:/std/sucai/btn_exit2.png) -120px 0px no-repeat;"
							"}");
	min_btn = new QPushButton(this);
	min_btn->setGeometry(710, 5, 30, 30);
	min_btn->setStyleSheet("QPushButton{"
						   "background:url(:/std/sucai/btn_minimize1.png) -120px 0px no-repeat;"
						   "}"
						   "QPushButton:hover{"
						   "background:url(:/std/sucai/btn_minimize2.png) -120px 0px no-repeat;"
						   "}"
						   "QPushButton:pressed {"
						   "background:url(:/std/sucai/btn_minimize2.png) -120px 0px no-repeat;"
						   "}");
	connect(min_btn, &QPushButton::clicked, this->parentWidget(), &QWidget::showMinimized);
	connect(exit_btn, &QPushButton::clicked, this->parentWidget(), &QWidget::close);

	effect = new QGraphicsDropShadowEffect(this);
	effect->setOffset(0, 0);
	effect->setColor(QColor(63, 63, 63, 180));
	effect->setBlurRadius(15);
	setGraphicsEffect(effect);

	list = new QListWidget(this);
	list->setGeometry(5, 50, 200, 480);
	list->setIconSize(QSize(50, 50));
	list->setContextMenuPolicy(Qt::CustomContextMenu);
	list->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


	//设置菜单动作
	add = new QAction("添加好友");
	connect(add, &QAction::triggered, this, &MainCenterWidget::addFriend);
	connect(list, &QListWidget::customContextMenuRequested, this, &MainCenterWidget::showMenu);
	connect(list, &QListWidget::currentItemChanged, this, &MainCenterWidget::changeItem);

	//实现网络信息回调
	connect(HTTPClient::Instance(), &HTTPClient::friends, this, &MainCenterWidget::readFriends);
	connect(HTTPClient::Instance(), &HTTPClient::mkFriend, this, &MainCenterWidget::mkFriend);
	connect(HTTPClient::Instance(), &HTTPClient::delFriend, this, &MainCenterWidget::delFriendRes);
	connect(HTTPClient::Instance(), &HTTPClient::friendRequest, this, &MainCenterWidget::newFriend);
	connect(HTTPClient::Instance(), &HTTPClient::mkFriendRes, this, &MainCenterWidget::mkFriendRes);
	connect(HTTPClient::Instance(), &HTTPClient::msgData, this, &MainCenterWidget::msgData);
	connect(HTTPClient::Instance(), &HTTPClient::msgRes, this, &MainCenterWidget::showMsgRes);

	now = nullptr;
	newFr = nullptr;
	output = nullptr;
	map = new QMap<QString, FriendItem*>();

	input = new QTextEdit(this);
	input->setGeometry(230, 430, 530, 100);
	input->setStyleSheet("QTextEdit{"
						 "border-width:3px;"
						 "border-color:#BBBBBB;"
						 "border-style:solid;"
						 "}"
						 "QTextEdit::hover{"
						 "border-width:3px;"
						 "border-color:#66CCFF;"
						 "border-style:solid;"
						 "}");
	send_btn = new QPushButton(this);
	send_btn->setGeometry(650, 540, 100, 30);
	send_btn->setText("发送");
	send_btn->setStyleSheet("QPushButton{"
							"border-radius:10px;"
							"background-color: rgba(87, 150, 255, 255);"
							"color:rgb(230, 230, 230);"
							"}"
							"QPushButton:hover{"
							"border-radius:10px;"
							"background-color: rgba(0, 128, 255, 255);"
							"color:rgb(230, 230, 230);"
							"}");
	send_btn->hide();
	send_file = new QPushButton(this);
	send_file->setGeometry(500, 540, 100, 30);
	send_file->setText("发送文件");
	send_file->setStyleSheet("QPushButton{"
							 "border-radius:10px;"
							 "background-color: rgba(87, 150, 255, 255);"
							 "color:rgb(230, 230, 230);"
							 "}"
							 "QPushButton:hover{"
							 "border-radius:10px;"
							 "background-color: rgba(0, 128, 255, 255);"
							 "color:rgb(230, 230, 230);"
							 "}");
	send_file->hide();
	input->hide();
	connect(send_btn, &QPushButton::clicked, [&](){
		if(input->toPlainText().isEmpty()) return;
		SendMsgRequestBean request;
		request.userName = userName;
		request.token = token;
		request.to = now->text();
		QDateTime s = QDateTime::currentDateTime();
		request.msg = msgHead.arg(s.toTime_t()).arg("*") + input->toPlainText();
		if(!HTTPClient::Instance()->sendMsg("sendmsg", request.converFromObject())) {
				send_btn->setText("发送失败");
		}else{
				if(QDateTime::currentDateTime().toTime_t() - now->getTime_t() > 60) {
						now->setTime_t(QDateTime::currentDateTime().toTime_t());
						new TimeItem(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm"), this, now->getList());
			}
				new MsgItem(input->toPlainText(), 1, this, output);
				output->setCurrentRow(output->count() - 1);
				input->clear();
		}
	});
	connect(send_file, &QPushButton::clicked, [&](){//发送文件，使用多线程
		QString path = QFileDialog::getOpenFileName(this, tr("打开文件"), ".", tr("All files(*.*)"));
		if(path.isEmpty()) return;
		Thread *thread = new Thread(userName, now->text(), token, path, this);
		sendFile = new CommonWidget(this);
		sendFile->center()->getButton()->hide();
		sendFile->center()->getEdit()->hide();
		connect(thread, &Thread::updateMsg, sendFile, [&](QString msg){
			sendFile->center()->getLabel()->setText(msg);
		});
		connect(thread, &Thread::sendMsg, HTTPClient::Instance(), [&](QString msg){
			HTTPClient::Instance()->sendMsg("sendmsg", msg);//我真是服了，别问我为什么
		});
		connect(thread, &QThread::finished, thread, &QObject::deleteLater);
		connect(thread, &QThread::finished, sendFile, &QWidget::close);
		thread->start();
		sendFile->exec();
		new MsgItem(path, 1, this, output);
	});
	HTTPClient::Instance()->setCallBack(this);
}

MainCenterWidget::~MainCenterWidget()
{
	delete map;
}

void MainCenterWidget::setUserName(QString u)
{
	userName = u;
}

void MainCenterWidget::setToken(int t)
{
	token = t;
}

void MainCenterWidget::start()
{
	//请求好友列表
	NormalRequestBean request;
	request.userName = userName;
	request.token = token;
	HTTPClient::Instance()->sendMsg("friends", request.converFromObject());
}

QString MainCenterWidget::getUserName()
{
	return userName;
}

int MainCenterWidget::getToken()
{
	return token;
}

void MainCenterWidget::showMenu(const QPoint &pt)
{
	QListWidgetItem *i = nullptr;
	i = list->itemAt(pt);
	if(i != nullptr) {
		static_cast<FriendItem*>(i)->showMenu(list->mapToGlobal(pt));
	}else{
		QMenu menu(this);
		menu.setStyleSheet("QMenu::item:selected {"
						   "background-color: #2dabf9;"
						   "}");
		menu.addAction(add);
		menu.exec(list->mapToGlobal(pt));
	}
}

void MainCenterWidget::readFriends(QString data)
{
	FriendResponseBean res;
	res.converFromJson(data);
	for(QString s:res.getList()) {
		FriendItem *l = new FriendItem(this, list);
		l->setText(s);
		map->insert(s, l);
		connect(l, &FriendItem::addFriend, this, &MainCenterWidget::addFriend);
		connect(l, &FriendItem::delFriend, this, &MainCenterWidget::delFriend);
		list->addItem(l);
	}
}

void MainCenterWidget::addFriend()
{
	w = new CommonWidget(this);
	w->center()->getLabel()->setText("输入用户名:");
	w->center()->getButton()->setText("添加好友");
	connect(w->center()->getButton(), &QPushButton::clicked, [&](){
		MakeFriendRequestBean request;
		request.userName = userName;
		request.token = token;
		request.who = w->center()->getEdit()->text();
		if(!HTTPClient::Instance()->sendMsg("makefriend", request.converFromObject())) {
				w->center()->getButton()->setText("发送失败");
		}else w->close();
	});
	w->exec();
}

void MainCenterWidget::mkFriend(QString data)
{
	ResponseDataBean res;
	res.converFromJson(data);
	w = new CommonWidget(this);
	w->center()->getButton()->setText("知道了");
	w->center()->getEdit()->hide();
	connect(w->center()->getButton(), &QPushButton::clicked, w, &QWidget::close);
	w->center()->getLabel()->setText(res.getMsg());
	w->show();
}

void MainCenterWidget::delFriend(QString name)
{
	FriendItem *from = static_cast<FriendItem*>(sender());
	w = new CommonWidget(this);
	w->center()->getLabel()->setText("你真的要删除好友" + name + "吗?");
	w->center()->getEdit()->hide();
	w->center()->getButton()->setText("确定");
	connect(w->center()->getButton(), &QPushButton::clicked, [&](){
		MakeFriendRequestBean request;
		request.userName = userName;
		request.token = token;
		request.who = name;
		if(!HTTPClient::Instance()->sendMsg("delete", request.converFromObject())) {
				w->center()->getButton()->setText("发送失败");
		}else {
				map->remove(name);
				if(from->getList() == output) {
						output = nullptr;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                //很重要的一步
						send_btn->hide();
						send_file->hide();
						input->hide();
						now = nullptr;
			}
				delete from;
		}
	});
	connect(w->center()->getButton(), &QPushButton::clicked, w, &QWidget::close);
	w->exec();
}

void MainCenterWidget::delFriendRes(QString data)
{
	ResponseDataBean res;
	res.converFromJson(data);
	w = new CommonWidget(this);
	w->center()->getButton()->setText("知道了");
	w->center()->getEdit()->hide();
	connect(w->center()->getButton(), &QPushButton::clicked, w, &QWidget::close);
	w->center()->getLabel()->setText(res.getMsg());
	w->exec();
}

void MainCenterWidget::newFriend(QString data)
{
	newFr = new CommonWidget(this);
	MakeFriendResponseBean res;
	res.converFromJson(data);
	newFr->center()->getLabel()->setText(res.getFrom() + "请求加为好友");
	newFr->center()->getEdit()->hide();
	newFr->center()->getButton()->setText("同意");
	newFr->center()->getButton()->setGeometry(60, 130, 50, 40);
	QPushButton *cancel = new QPushButton(newFr->center());
	cancel->setGeometry(130, 130, 50, 40);
	cancel->setStyleSheet("QPushButton{"
						  "border-radius:10px;"
						  "background-color: rgba(87, 150, 255, 255);"
						  "color:rgb(230, 230, 230);"
						  "}"
						  "QPushButton:hover{"
						  "border-radius:10px;"
						  "background-color: rgba(0, 128, 255, 255);"
						  "color:rgb(230, 230, 230);"
						  "}");
	cancel->setText("不同意");
	connect(newFr->center()->getButton(), &QPushButton::clicked, [&](){
		ResultOfFriendBean ress;
		ress.from = userName;
		ress.to = res.getFrom();
		ress.token = token;
		ress.isOk = 1;
		if(!HTTPClient::Instance()->sendMsg("result", ress.converFromObject())) {
				newFr->center()->getButton()->setText("发送失败");
		}else {
				newFr->close();
				for(int i = 0; i < list->count(); i++) {
						if(list->item(i)->text() == res.getFrom()) {
								map->insert(res.getFrom(), static_cast<FriendItem*>(list->item(i)));
								return;
				}
			}
				FriendItem *i = new FriendItem(this, list);
				connect(i, &FriendItem::addFriend, this, &MainCenterWidget::addFriend);
				connect(i, &FriendItem::delFriend, this, &MainCenterWidget::delFriend);
				i->setText(res.getFrom());
				map->insert(res.getFrom(), i);
				list->addItem(i);
		}
	});
	connect(cancel, &QPushButton::clicked, [&](){
		ResultOfFriendBean ress;
		ress.from = userName;
		ress.to = res.getFrom();
		ress.token = token;
		ress.isOk = 0;
		if(!HTTPClient::Instance()->sendMsg("result", ress.converFromObject())) {
				cancel->setText("发送失败");
		}else {
				newFr->close();
				newFr = nullptr;
		}
	});
	newFr->exec();
}

void MainCenterWidget::mkFriendRes(QString data)
{
	ResponseDataBean res;
	res.converFromJson(data);
	if(res.getCode()) {
		for(int i = 0; i < list->count(); i++) {
			if(list->item(i)->text() == res.getMsg()) {
				map->insert(res.getMsg(), static_cast<FriendItem*>(list->item(i)));
				return;
			}
		}
		FriendItem *i = new FriendItem(this, list);
		connect(i, &FriendItem::addFriend, this, &MainCenterWidget::addFriend);
		connect(i, &FriendItem::delFriend, this, &MainCenterWidget::delFriend);
		i->setText(res.getMsg());
		map->insert(res.getMsg(), i);
		list->addItem(i);
	}
}

void MainCenterWidget::changeItem(QListWidgetItem *i)
{
	if(i == nullptr || i->text().isEmpty()) return;
	list->setCurrentItem(i);
	FriendItem *s = static_cast<FriendItem*>(i);
	if(output != nullptr) output->hide();
	output = s->getList();
	now = s;
	s->getList()->show();
	send_btn->show();
	send_file->show();
	input->show();
}

void MainCenterWidget::msgData(QString data)
{
	MakeFriendResponseBean res;
	res.converFromJson(data);
	QString head = "";
	int at = 0;
	for(int i = 1; i < res.getMsg().size(); i++) {
		if(res.getMsg().at(i) == '}') {
			at = i + 1;
			break;
		}
		head += res.getMsg().at(i);
	}
	QStringList l = head.split(",");
	if(l[0].toUInt() - map->find(res.getFrom()).value()->getTime_t() > 60) {
		map->find(res.getFrom()).value()->setTime_t(l[0].toUInt());
		new TimeItem(QDateTime::fromTime_t(l[0].toUInt()).toString("yyyy/MM/dd hh:mm"), this, map->find(res.getFrom()).value()->getList());
	}
	if(l[1] == "*") {
		new MsgItem(res.getMsg().mid(at), 0, this, map->find(res.getFrom()).value()->getList());
	}else{
		QStringList tsts = l[1].split("*");
		QFile file(tsts[2]);
		if(tsts[0].toInt() != 1) file.open(QIODevice::WriteOnly | QIODevice::Append);
		else file.open(QIODevice::WriteOnly);
		QDataStream in(&file);
		in.writeRawData(res.getMsg().mid(at).toLatin1().data(), res.getMsg().mid(at).toLatin1().size());
		qDebug() << res.getMsg().mid(at).toLatin1().size();
		file.close();
		if(tsts[0] == tsts[1]) new MsgItem(tsts[2], 0, this, map->find(res.getFrom()).value()->getList());
	}
	map->find(res.getFrom()).value()->getList()->setCurrentRow(map->find(res.getFrom()).value()->getList()->count() - 1);
}

void MainCenterWidget::showMsgRes(QString data)
{
	ResponseDataBean res;
	res.converFromJson(data);
	if(res.getCode() != 0) {
		w = new CommonWidget(this);
		w->center()->getLabel()->setText(res.getMsg());
		w->center()->getEdit()->hide();
		w->center()->getButton()->setText("确定");
		connect(w->center()->getButton(), &QPushButton::clicked, w, &QWidget::close);
		w->exec();
	}
}

void MainCenterWidget::paintEvent(QPaintEvent *)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
