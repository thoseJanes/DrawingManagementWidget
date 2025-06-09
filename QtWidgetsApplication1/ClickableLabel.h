#pragma once
#include <QtWidgets/QtWidgets>
class ClickableLabel :public QLabel {
    Q_OBJECT
signals:
    void itemDoubleClicked();
public:
    ClickableLabel(QWidget* parent = nullptr) :QLabel(parent) {};
    ~ClickableLabel() {};

    void mouseDoubleClickEvent(QMouseEvent* event) override {
        qDebug() << "clicked";
        emit itemDoubleClicked();
    }
};