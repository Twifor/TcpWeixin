#include "msgitem.h"

MsgItem::MsgItem(QString msg, int from, QObject *parent, QListWidget *list) : QObject(parent), QListWidgetItem (list)
{
	w = new MsgWidget(this, msg, from, list);
	list->addItem(this);
	list->setItemWidget(this, w);
	setFlags(Qt::NoItemFlags);
}

MsgWidget::MsgWidget(QListWidgetItem *item, QString msg, int from, QWidget *par) : QWidget (par)
{
	this->msg = msg;
	this->from = from;

	QFont font;
	font.setFamily("MicrosoftYaHei");
	font.setPointSize(10);
	QFontMetrics fm(font);
	QString tr, tmp;
	int rows = 1;
	for(int i = 0; i < this->msg.size(); i++) {
		tmp += this->msg[i];
		if(fm.size(Qt::TextSingleLine, tmp).width() > 400 || this->msg[i] == "\n") {
			tr += tmp;
			if( this->msg[i] != "\n") tr += "\n";
			++rows;
			tmp = "";
		}
	}
	if(!tmp.isEmpty()) tr += tmp;
	w = std::min(fm.width(tr), 400) + 10;
	h = fm.height() * rows + fm.lineSpacing() * (rows - 1) + 20;
	item->setSizeHint(QSize(w + 30, h + 20));
	this->msg = tr;
}

void MsgWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);//消锯齿
	painter.setPen(Qt::NoPen);
	painter.setBrush(QBrush(Qt::gray));

	if(from == 0) {//画对面的
		QColor col_KuangB(230, 230, 230);
		painter.setBrush(QBrush(col_KuangB));
		painter.drawRoundedRect(15, 10, w, h, 4, 4);
		QColor col_Kuang(255, 255, 255);
		painter.setBrush(QBrush(col_Kuang));
		painter.drawRoundedRect(16, 11, w - 2, h - 2, 4, 4);
		QPoint p[] = {
			QPoint(16, 18),
			QPoint(16, 24),
			QPoint(10, 21)
		};
		QPen pen;
		pen.setColor(col_Kuang);
		painter.setPen(pen);
		painter.drawPolygon(p, 3);
		QPen penSanJiaoBian;
		penSanJiaoBian.setColor(col_KuangB);
		painter.setPen(penSanJiaoBian);
		painter.drawLine(QPoint(16, 17), QPoint(9, 21));
		painter.drawLine(QPoint(16, 25), QPoint(9, 21));
		QPen penText;
		QFont font;
		font.setFamily("MicrosoftYaHei");
		font.setPointSize(10);
		painter.setFont(font);
		penText.setColor(QColor(51, 51, 51));
		painter.setPen(penText);
		QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
		option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
		painter.drawText(QRectF(20, 16, w - 10, h - 10), msg, option);
	}else{
		QColor col_Kuang(75, 164, 242);
		painter.setBrush(QBrush(col_Kuang));
		painter.drawRoundedRect(516 - w, 11, w - 2, h - 2, 4, 4);
		QPoint p[] = {
			QPoint(514, 18),
			QPoint(514, 24),
			QPoint(522, 21)
		};
		QPen pen;
		pen.setColor(col_Kuang);
		painter.setPen(pen);
		painter.drawPolygon(p, 3);
		QPen penText;
		QFont font;
		font.setFamily("MicrosoftYaHei");
		font.setPointSize(10);
		painter.setFont(font);
		penText.setColor(QColor(51, 51, 51));
		painter.setPen(penText);
		QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
		option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
		painter.drawText(QRectF(520 - w, 16, w - 10, h - 10), msg, option);
	}
}

TimeItem::TimeItem(QString time, QObject *parent, QListWidget *list) : QObject (parent), QListWidgetItem (list)
{
	setSizeHint(QSize(list->width() - 20, 20));
	list->addItem(this);
	w = new TimeWidget(time, list);
	list->setItemWidget(this, w);
	setFlags(Qt::NoItemFlags);
}

TimeWidget::TimeWidget(QString time, QWidget *par) : QWidget (par)
{
	this->time = time;
}

void TimeWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QPen pen;
	pen.setColor(QColor(180, 180, 180));
	painter.setPen(pen);
	painter.drawText(QRectF(200, 5, 150, 20), time);
}
