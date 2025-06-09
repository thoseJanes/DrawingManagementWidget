#pragma once
#include<QDockWidget>
#include <QCloseEvent>
#include <QDebug>

class StatusDockWidget : public QDockWidget {
    Q_OBJECT
signals:
    void closedRespond();
public:
    StatusDockWidget(const QString& title, QWidget* parent = nullptr,
        Qt::WindowFlags flags = Qt::WindowFlags()) :QDockWidget(title, parent ,flags) {};

    void closeEvent(QCloseEvent* event) override{
        qDebug() << "closed";
        emit closedRespond();
        event->accept();
    }
};
