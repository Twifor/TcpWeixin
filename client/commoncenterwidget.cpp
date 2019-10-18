#include "commoncenterwidget.h"

CommonCenterWidget::CommonCenterWidget(QWidget *parent) : QWidget(parent)
{
	setGeometry(10, 10, 240, 180);
	setStyleSheet("QDialog{"
				  "background-color: rgb(255,255, 255);"
				  "border-top-left-radius:15px;"
				  "border-top-right-radius:15px;"
				  "border-bottom-right-radius:15px;"
				  "border-bottom-left-radius:15px;"
				  "}");
	exit_btn = new QPushButton(this);
	exit_btn->setGeometry(205, 5, 30, 30);
	exit_btn->setStyleSheet("QPushButton{"
							"background:url(:/std/sucai/btn_exit1.png) -120px 0px no-repeat;"
							"}"
							"QPushButton:hover{"
							"background:url(:/std/sucai/btn_exit2.png) -120px 0px no-repeat;"
							"}"
							"QPushButton:pressed {"
							"background:url(:/std/sucai/btn_exit2.png) -120px 0px no-repeat;"
							"}");
	connect(exit_btn, &QPushButton::clicked, this->parentWidget(), &QWidget::close);

	effect = new QGraphicsDropShadowEffect(this);
	effect->setOffset(0, 0);
	effect->setColor(QColor(63, 63, 63, 180));
	effect->setBlurRadius(15);
	setGraphicsEffect(effect);

	label = new QLabel(this);
	label->setGeometry(20, 40, 200, 50);
	label->setText("");
	edit = new QLineEdit(this);
	edit->setGeometry(20, 100, 200, 20);
	edit->setStyleSheet("QLineEdit {"
						"border: 1px solid rgb(50, 50, 100);"
						"border-radius: 6px;"
						"background: white;"
						"selection-background-color: blue;"
						"font-size: 14px ;"
						" }"
						"QLineEdit:hover {"
						"border: 1px solid blue;"
						"}");
	button = new QPushButton(this);
	button->setText("");
	button->setGeometry(70, 130, 100, 40);
	button->setStyleSheet("QPushButton{"
						  "border-radius:10px;"
						  "background-color: rgba(87, 150, 255, 255);"
						  "color:rgb(230, 230, 230);"
						  "}"
						  "QPushButton:hover{"
						  "border-radius:10px;"
						  "background-color: rgba(0, 128, 255, 255);"
						  "color:rgb(230, 230, 230);"
						  "}");
}

void CommonCenterWidget::paintEvent(QPaintEvent *event)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
//    QBrush brush;
//    brush.setColor(QColor(128, 255, 255));
//    p.setBrush(brush);
//    p.drawRect(0, 0, 240, 10);
}

QPushButton *CommonCenterWidget::getButton()
{
	return button;
}

QLineEdit *CommonCenterWidget::getEdit()
{
	return edit;
}

QLabel *CommonCenterWidget::getLabel()
{
	return label;
}

