#pragma once
#include "FoldWidget.h"
#include "AddFigureWidget.h"

class Figure2DConfig:public FoldWidget {
public:
    struct ScrollAreaList {
        QScrollArea* scroll;
        QWidget* container;
        QVBoxLayout* vbox;
    };
	QTabWidget* configTabWidget;
	std::vector<std::pair<QString, QWidget*>> configTabsVec;
	Figure2DConfig(QWidget* parent = nullptr): FoldWidget(QStringLiteral("ͼ������"), parent) {
        initConfigTabs();
        initTabWidget();
	};
	~Figure2DConfig() {};
	void initTabWidget() {
        auto contentLayout = new QVBoxLayout(contentFrame);
        contentLayout->setContentsMargins(0, 0, 0, 0);
        contentFrame->setLayout(contentLayout);
		configTabWidget = new QTabWidget(contentFrame);
        configTabWidget->setTabPosition(QTabWidget::West);
        contentLayout->addWidget(configTabWidget);
        //put tabs to tabWidget.
        for (auto it = configTabsVec.begin(); it != configTabsVec.end(); it++) {
            QWidget* tab = new QWidget(configTabWidget);
            configTabWidget->addTab(tab, (*it).first);
            QVBoxLayout* verticalLayout = new QVBoxLayout(tab);
            tab->setLayout(verticalLayout);
            verticalLayout->setContentsMargins(0, 0, 0, 0);

            ScrollAreaList param_scroll;
            initScrollAreaList(&param_scroll, 4, tab);
            param_scroll.scroll->setStyleSheet("QScrollArea > QWidget { border: none; background-color: transparent;}"
                "QScrollArea > QWidget >QWidget{ border: none; background-color: transparent;}"
                "QScrollArea { border: none; background-color: transparent;}");
            (*it).second->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);
            param_scroll.vbox->addWidget((*it).second);
            verticalLayout->addWidget(param_scroll.scroll);
            param_scroll.vbox->addStretch(1);
        }
	};
    void initConfigTabs() {
        QString tabName; QWidget* tabContent; QVBoxLayout* grossLayout; QHBoxLayout* hItemLayout;
        int labelStrech = 1;
        int inputStrech = 2;
        tabName = QStringLiteral("����");
        tabContent = new QWidget(this);
        grossLayout = new QVBoxLayout(tabContent);
        tabContent->setLayout(grossLayout);
        configTabsVec.push_back(std::pair<QString, QWidget*>(tabName, tabContent));

        hItemLayout = new QHBoxLayout(this);
        auto nameLabel = new QLabel(QStringLiteral("ͼ��"));
        auto nameEditor = new QLineEdit();
        nameEditor->setObjectName("nameEditor");
        hItemLayout->addWidget(nameLabel, labelStrech);
        hItemLayout->addWidget(nameEditor, inputStrech);
        grossLayout->addLayout(hItemLayout);

        auto paintRealTime = new QCheckBox();
        paintRealTime->setObjectName("isRealTime");
        paintRealTime->setText(QStringLiteral("�Ƿ�ʵʱ��ͼ"));
        grossLayout->addWidget(paintRealTime);

        tabName = QStringLiteral("������");
        tabContent = new QWidget(this);
        grossLayout = new QVBoxLayout(tabContent);
        tabContent->setLayout(grossLayout);
        configTabsVec.push_back(std::pair<QString, QWidget*>(tabName, tabContent));

        auto axisTab = new QTabWidget();
        axisTab->setObjectName("axisTab");
        QStringList axisInitName = { QStringLiteral("x��"), QStringLiteral("y��")};
        for (int i = 0; i < 2; i++) {
            auto axisWidget = new QWidget();
            axisWidget->setObjectName("axis" + QString::number(i));
            auto axisLayout = new QVBoxLayout(axisWidget);
            axisWidget->setLayout(axisLayout);

            auto axisLabelLayout = new QHBoxLayout(axisWidget);
            auto axisLabelLabel = new QLabel(axisInitName[i] + QStringLiteral("��ǩ"), axisWidget);
            auto axisLabelEditor = new QLineEdit(axisWidget);
            axisLabelEditor->setObjectName("axisLabel");
            axisLabelEditor->setText(axisInitName[i]);
            axisLabelLayout->addWidget(axisLabelLabel, 1);
            axisLabelLayout->addWidget(axisLabelEditor, inputStrech);
            axisLayout->addLayout(axisLabelLayout);

            auto autoSetRangeBoxLayout = new QHBoxLayout(axisWidget);
            auto autoSetAxisRangeLabel = new QLabel(QStringLiteral("�Զ������᷶Χ"), axisWidget);
            auto autoSetAxisRangeBox = new QCheckBox(axisWidget);
            autoSetAxisRangeBox->setObjectName("isAutoRange");
            autoSetAxisRangeBox->setChecked(true);
            autoSetRangeBoxLayout->addWidget(autoSetAxisRangeLabel, labelStrech);
            autoSetRangeBoxLayout->addWidget(autoSetAxisRangeBox, inputStrech);
            axisLayout->addLayout(autoSetRangeBoxLayout);

            auto setRangeEditorLayout = new QHBoxLayout(axisWidget);
            auto aSRLabel = new QLabel(QStringLiteral("    �᷶Χ"), axisWidget);
            auto aSRELowerBound = new QLineEdit(axisWidget);
            auto medLabel = new QLabel(QStringLiteral("-"), axisWidget);
            medLabel->setAlignment(Qt::AlignCenter);
            auto aSREUpperBound = new QLineEdit(axisWidget);
            aSRELowerBound->setObjectName("lowerRange");
            aSREUpperBound->setObjectName("upperRange");
            setRangeEditorLayout->addWidget(aSRLabel, labelStrech * 4);
            setRangeEditorLayout->addWidget(aSRELowerBound, inputStrech * 2 - 1);
            setRangeEditorLayout->addWidget(medLabel, 2);
            setRangeEditorLayout->addWidget(aSREUpperBound, inputStrech * 2 - 1);
            axisLayout->addLayout(setRangeEditorLayout);
            aSRLabel->setEnabled(!autoSetAxisRangeBox->isChecked());
            medLabel->setEnabled(!autoSetAxisRangeBox->isChecked());
            aSRELowerBound->setEnabled(!autoSetAxisRangeBox->isChecked());
            aSREUpperBound->setEnabled(!autoSetAxisRangeBox->isChecked());
            connect(autoSetAxisRangeBox, &QCheckBox::clicked, this, [=]() {
                aSRLabel->setEnabled(!autoSetAxisRangeBox->isChecked());
                medLabel->setEnabled(!autoSetAxisRangeBox->isChecked());
                aSRELowerBound->setEnabled(!autoSetAxisRangeBox->isChecked());
                aSREUpperBound->setEnabled(!autoSetAxisRangeBox->isChecked());
                });

            auto autoSetTickSpaceBoxLayout = new QHBoxLayout(axisWidget);
            auto autoSetAxisTickSpaceLabel = new QLabel(QStringLiteral("�Զ��̶ȼ��"), axisWidget);
            auto autoSetAxisTickSpaceBox = new QCheckBox(axisWidget);
            autoSetAxisTickSpaceBox->setObjectName("isAutoTickSpace");
            autoSetAxisTickSpaceBox->setChecked(true);
            autoSetTickSpaceBoxLayout->addWidget(autoSetAxisTickSpaceLabel, labelStrech);
            autoSetTickSpaceBoxLayout->addWidget(autoSetAxisTickSpaceBox, inputStrech);
            axisLayout->addLayout(autoSetTickSpaceBoxLayout);

            auto setTickSpaceEditorLayout = new QHBoxLayout(axisWidget);
            auto aSTSLabel = new QLabel(QStringLiteral("    �̶ȼ��"), axisWidget);
            auto aSTSEditor = new QLineEdit(axisWidget);
            aSTSEditor->setObjectName("tickSpace");
            setTickSpaceEditorLayout->addWidget(aSTSLabel, labelStrech);
            setTickSpaceEditorLayout->addWidget(aSTSEditor, inputStrech);
            axisLayout->addLayout(setTickSpaceEditorLayout);
            aSTSLabel->setEnabled(!autoSetAxisTickSpaceBox->isChecked());
            aSTSEditor->setEnabled(!autoSetAxisTickSpaceBox->isChecked());
            connect(autoSetAxisTickSpaceBox, &QCheckBox::clicked, this, [=]() {
                aSTSLabel->setEnabled(!autoSetAxisTickSpaceBox->isChecked());
                aSTSEditor->setEnabled(!autoSetAxisTickSpaceBox->isChecked());
                });
            axisTab->addTab(axisWidget, axisInitName[0] + QStringLiteral("����"));
        }
        //axisTab->widget(2)->hide();
        grossLayout->addWidget(axisTab);
    };
    void initScrollAreaList(ScrollAreaList* scroll_area, int content_margin, QWidget* parent) {
        scroll_area->scroll = new QScrollArea(parent);
        scroll_area->container = new QWidget(parent);
        //scroll_area->container->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        scroll_area->container->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        //scroll_area->container->setFixedHeight(h);
        //scroll_area->container->setFixedWidth(w);
        scroll_area->scroll->setWidget(scroll_area->container);
        scroll_area->scroll->setWidgetResizable(true);

        //scroll_area->scroll->setFixedWidth(w);
        scroll_area->vbox = new QVBoxLayout(scroll_area->container);
        scroll_area->vbox->setContentsMargins(content_margin, content_margin, content_margin, content_margin);
        //scroll_area->container->setLayout(scroll_area->vbox);
    }
};