#pragma once
#include <QtWidgets/QtWidgets>
class ItemWidget :public QWidget {
    Q_OBJECT
signals:
    void itemDoubleClicked();
public:
    ItemWidget(QWidget* parent = nullptr) :QWidget(parent) {};
    ~ItemWidget() {};

    void mouseDoubleClickEvent(QMouseEvent* event) override {
        qDebug() << "clicked";
        emit itemDoubleClicked();
    }
};