#pragma once

#include <QtWidgets/QtWidgets>

class FoldWidget :public QWidget {
	Q_OBJECT
public:
	enum FoldDirection { North, South, West, East, Floating };
	QPushButton* headButton;
	QFrame* contentFrame;
	QLayout* grossLayout;
	bool contentHidden = true;
	QString title;
	FoldWidget(QString name = "", QWidget* parent = nullptr) {
		title = name;
		grossLayout = new QVBoxLayout(this);
		grossLayout->setContentsMargins(0,0,0,0);
		this->setLayout(grossLayout);

		headButton = new QPushButton(this);
		grossLayout->addWidget(headButton);
		
		headButton->setCheckable(true);

		contentFrame = new QFrame(this);
		grossLayout->addWidget(contentFrame);
		contentFrame->setContentsMargins(0, 0, 0, 0);
		contentFrame->hide();

		setHeadButtonConnection(QStringLiteral("▷"), QStringLiteral("∆"));
	}
	~FoldWidget() {
		this->headButton->deleteLater();
		this->contentFrame->deleteLater();
		this->deleteLater();
	};
	void setFoldDirection(FoldDirection dir) {
		delete grossLayout;
		grossLayout = new QHBoxLayout(this);
		QObject::disconnect(headButton, nullptr, this, nullptr);
		if (dir == North || dir == South) {
			title = QStringLiteral("图形设置");
			headButton->setText(title);
			headButton->setMaximumSize(500, 30);
		}
		else if (dir == West || dir == East) {
			title = QStringLiteral("\n图\n形\n设\n置");
			headButton->setText(title);
			headButton->setMaximumSize(30, 500);
		}
		if (dir == North) {
			grossLayout->addWidget(contentFrame);
			grossLayout->addWidget(headButton);
			setHeadButtonConnection(QStringLiteral("▷"), QStringLiteral("∆"));
		}
		else if (dir == South) {
			grossLayout->addWidget(headButton);
			grossLayout->addWidget(contentFrame);
			setHeadButtonConnection(QStringLiteral("▷"), QStringLiteral("∇"));
		}
		else if (dir == West) {
			grossLayout->addWidget(contentFrame);
			grossLayout->addWidget(headButton);
			setHeadButtonConnection(QStringLiteral("∇"), QStringLiteral("◁"));
		}
		else if (dir == East) {
			grossLayout->addWidget(headButton);
			grossLayout->addWidget(contentFrame);
			setHeadButtonConnection(QStringLiteral("∇"), QStringLiteral("▷"));
		}
		if (dir == Floating) {
			headButton->setText(title);
			grossLayout->addWidget(headButton);
		}
		this->setLayout(grossLayout);
	}
	void setContentLayout(QBoxLayout* layout) {
		this->contentFrame->setLayout(layout);
	}
	void setHeadButtonConnection(QString foldString, QString unfoldString) {
		if (this->contentHidden) {
			headButton->setText(foldString + title);
		}
		else {
			headButton->setText(unfoldString + title);
		}
		
		connect(headButton, &QPushButton::clicked, this, [=]() {
			if (!headButton->isChecked()) {
				contentFrame->hide();
				headButton->setText(foldString + this->title);
				contentHidden = true;
			}
			else {
				contentFrame->show();
				headButton->setText(unfoldString + this->title);
				contentHidden = false;
			}
			});
	}
};