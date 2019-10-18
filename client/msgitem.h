#ifndef MSGITEM_H
#define MSGITEM_H

#include <QObject>
#include <QListWidget>
#include <QPainter>
#include <QBrush>
#include <qlabel.h>
#include <QFontMetrics>
#include <QDebug>
#include <bits/stdc++.h>

class MsgWidget : public QWidget {
	Q_OBJECT
public:
	explicit MsgWidget(QListWidgetItem *item, QString msg, int from, QWidget *par = nullptr);

private:
	QString msg;
	int from;
	int h, w;

protected:
	void paintEvent(QPaintEvent *event) override;
};

class MsgItem : public QObject, public QListWidgetItem
{
	Q_OBJECT
public:
	explicit MsgItem(QString msg, int from, QObject *parent = nullptr, QListWidget *list = nullptr);

signals:

public slots:

private:
	MsgWidget *w;
};

class TimeWidget : public QWidget {
	Q_OBJECT
public:
	explicit TimeWidget(QString time, QWidget *par = nullptr);
private:
	QString time;

	// QWidget interface
protected:
	void paintEvent(QPaintEvent *event) override;
};

class TimeItem : public QObject, public QListWidgetItem
{
	Q_OBJECT
public:
	explicit TimeItem(QString time, QObject *parent = nullptr, QListWidget *list = nullptr);
private:
	TimeWidget *w;

};

#endif // MSGITEM_H
