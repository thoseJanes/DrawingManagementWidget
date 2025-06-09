#pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1600)    
# pragma execution_character_set("utf-8")    
#endif

#include <QtWidgets/QDialog>
#include<QCloseEvent>
#include <QtWidgets>
class MyDialog :public QDialog {
    Q_OBJECT
signals:
    void closedRespond();
public:
    QPushButton* createButton; QPushButton* cancelButton;
    MyDialog(QWidget* parent = nullptr) :QDialog(parent) {};
    ~MyDialog() {};
    void setContentAndButton(QWidget* widget, QString title, QString createText="", QString cancelText="") {
        this->setWindowTitle(title);
        auto vLayout = new QVBoxLayout(this);

        vLayout->addWidget(widget, 1);

        if (cancelText != "" && createText != "") {
            auto buttonLayout = new QHBoxLayout(this);
            buttonLayout->addStretch(1);
            createButton = new QPushButton(this);
            createButton->setText(createText);
            buttonLayout->addWidget(createButton);
            buttonLayout->addStretch(2);
            cancelButton = new QPushButton(this);
            cancelButton->setText(cancelText);
            buttonLayout->addWidget(cancelButton);
            buttonLayout->addStretch(1);
            vLayout->addLayout(buttonLayout);
        }
        //connect(cancelButton, &QPushButton::clicked, this, &QDialog::accept);
        //connect(cancelButton, &QPushButton::clicked, widget, &QWidget::deleteLater);
        //connect(this, &MyDialog::closedRespond, this, &QDialog::accept);
        //connect(this, &MyDialog::closedRespond, widget, &QWidget::deleteLater);
    }
    void closeEvent(QCloseEvent* event) override {
        event->ignore();
        emit closedRespond();
    }
};