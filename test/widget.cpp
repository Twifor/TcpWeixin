#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Widget)
{
	ui->setupUi(this);
	socket = new QTcpSocket;
	setWindowTitle("调试工具");
	ui->comboBox->addItem("start");
	ui->comboBox->addItem("login");
	ui->comboBox->addItem("offline");
	ui->comboBox->addItem("register");
	ui->comboBox->addItem("friends");
	ui->comboBox->addItem("makefriend");
	ui->comboBox->addItem("result");
	ui->comboBox->addItem("delete");
	ui->comboBox->addItem("sendmsg");

	connect(socket, &QTcpSocket::connected, [&](){
		QMessageBox::information(this, "OK", "连接服务器成功");
	});
	connect(socket, &QTcpSocket::disconnected, [&](){
		QMessageBox::critical(this, "err", "与服务器断开连接");
	});
	connect(socket, &QTcpSocket::readyRead, [&](){
		QString s = socket->readAll();
		QMessageBox::information(this, "接受信息", s);
	});
	connect(ui->comboBox, &QComboBox::currentTextChanged, [&](QString str){
		if(str == "start") ui->textEdit->clear();
		else if(str == "login") ui->textEdit->setText("{\"userName\":\"\", \"pwd\":\"\"}");
		else if(str == "offline") ui->textEdit->setText("{\"userName\":\"\", \"token\":  }");
		else if(str == "register") ui->textEdit->setText("{\"userName\":\"\", \"pwd\":\"\"}");
		else if(str == "friends") ui->textEdit->setText("{\"userName\":\"\", \"token\":  }");
		else if(str == "makefriend") ui->textEdit->setText("{\"userName\":\"\", \"token\": ,\"who\":\"\" }");
		else if(str == "result") ui->textEdit->setText("{\"from\":\"\", \"to\":\"\", \"token\": ,\"isOk\": }");
		else if(str == "delete") ui->textEdit->setText("{\"userName\":\"\", \"token\": ,\"who\":\"\" }");
		else if(str == "sendmsg") ui->textEdit->setText("{\"userName\":\"\", \"token\": ,\"to\":\"\",\"msg\":\"\" }");
	});
	QComboBox s;
	connect(ui->pushButton, &QPushButton::clicked, [&](){
		QString s = ui->comboBox->currentText();
		QString p = ui->textEdit->toPlainText();
		if(s == "start") {
				socket->connectToHost(p, 2333);
				return;
		}
		if(socket->state() != QTcpSocket::ConnectedState) {
				QMessageBox::critical(this, "err", "无效的连接");
				return;
		}
		QString msg = commonMSG.arg("GET").arg("/" + s).arg(p.toUtf8().size()) + p;
		socket->write(msg.toUtf8());
	});
}

Widget::~Widget()
{
	delete ui;
}
