#pragma once
#include <QtWidgets/QtWidgets>
#include <QtCore/QLibrary>
#include <QtGui/QtGui>
#include "GLFigure3D.h"
#include "FoldWidget.h"

class FigureWidget :public QWidget {
	Q_OBJECT
public:
	bool flag = false;
	GLFigure3D* figure;
	QToolBar* toolBar;
	QTextBrowser* debugEdit;
	QTimer* debugTimer;
	FoldWidget* config;
	QVBoxLayout* grossLayout;
	FigureWidget(QWidget* parent = nullptr) {
		toolBar = new QToolBar(tr("组件"), this);
		grossLayout = new QVBoxLayout(this);
		this->setLayout(grossLayout);
		this->figure = new GLFigure3D(nullptr,this);
		
		grossLayout->addWidget(toolBar);
		grossLayout->addWidget(this->figure, 1);

		this->initConfig();

		this->resize(700, 700);
	}

	~FigureWidget() {

	}


	void initConfig() {
		config = new FoldWidget("图像设置", this);
		config->setWindowTitle(QStringLiteral("设置"));
		grossLayout->addWidget(config);
		//debug
		QVBoxLayout* debugLayout = new QVBoxLayout(config);
		debugEdit = new QTextBrowser(this);
		debugLayout->addWidget(debugEdit);
		config->setContentLayout(debugLayout);

		debugTimer = new QTimer(this);
		connect(figure, &GLFigure3D::updateCompleted, this, &FigureWidget::debugUpdate);
	}
	void debugUpdate() {
		if (!debugEdit->isHidden()) {
			debugEdit->setText(QStringLiteral("上次点击位置：") + QString::number(figure->info.lastClickPosition.x) + "," + QString::number(figure->info.lastClickPosition.y));

			debugEdit->append(QStringLiteral("\n当前图范围：") + QString::number(figure->info.figureRectSize.x) + "," + QString::number(figure->info.figureRectSize.y));
			debugEdit->append(QStringLiteral("图范围位置：") +
				QString::number(figure->info.lastClickPosition.x / figure->width() * figure->info.figureRectSize.x - figure->info.figureRectSize.x / 2) +
				"," + QString::number(figure->info.figureRectSize.y / 2 - figure->info.lastClickPosition.y / figure->height() * figure->info.figureRectSize.y));
			debugEdit->append(QStringLiteral("标准化图范围位置：") +
				QString::number(figure->info.lastClickPosition.x / figure->width() * 2 - 1) +
				"," + QString::number(1 - figure->info.lastClickPosition.y / figure->height() * 2));
			if (figure->info.selectedText != nullptr) {
				debugEdit->append(QStringLiteral("选中文字：") + QString::fromStdWString(figure->selectedInfo.selectedText->text));
			}
		}
	}
};