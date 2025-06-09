#pragma once
#include "AddFigureWidget.h"
#include "GLFigure3D.h"
#include "Figure2D.h"
#include "MyDialog.h"
#include <QtWidgets/QtWidgets>

class FigureConfigWidget:public QWidget {
public:
    //QPushButton* headButton;
    QFrame* contentFrame;
    QLayout* grossLayout;
    QString title;

    struct ScrollAreaList {
        QScrollArea* scroll;
        QWidget* container;
        QVBoxLayout* vbox;
    };
    QTabWidget* configTabWidget;
    std::vector<std::pair<QString, QWidget*>> configTabsVec;
    FigureConfigWidget(QWidget* parent = nullptr):QWidget(parent){
        title = "图像设置";
        grossLayout = new QVBoxLayout(this);
        grossLayout->setContentsMargins(0, 0, 0, 0);
        this->setLayout(grossLayout);

        //headButton = new QPushButton(this);
        //headButton->setCheckable(true);

        contentFrame = new QFrame(this);
        contentFrame->setContentsMargins(0, 0, 0, 0);
        grossLayout->addWidget(contentFrame);
        contentFrame->show();
        //contentFrame->hide();
        //connect(headButton, &QPushButton::clicked, this, &FigureConfigWidget::openSetFigureDialog);

        initConfigTabs();
        initTabWidget();
        //this->setEnabled(false);
    };
    ~FigureConfigWidget() {};
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
        bool generalTabProcess = true; bool axisTabProcess = true;
        bool curveTabProcess = false; bool textTabProcess = false;
        if (generalTabProcess) {
            tabName = "常规";
            tabContent = new QWidget(this);
            grossLayout = new QVBoxLayout(tabContent);
            tabContent->setLayout(grossLayout);
            configTabsVec.push_back(std::pair<QString, QWidget*>(tabName, tabContent));

            hItemLayout = new QHBoxLayout(this);
            auto nameLabel = new QLabel("图名");
            auto nameEditor = new QLineEdit();
            nameEditor->setObjectName("nameEditor");
            hItemLayout->addWidget(nameLabel, labelStrech);
            hItemLayout->addWidget(nameEditor, inputStrech);
            grossLayout->addLayout(hItemLayout);

            //auto paintRealTime = new QCheckBox();
            //paintRealTime->setObjectName("isRealTime");
            //paintRealTime->setText("是否实时绘图");
            //grossLayout->addWidget(paintRealTime);

            auto paintTypeBox = new QComboBox(this);
            paintTypeBox->setObjectName("paintTypeBox");
            for (int i = 0; i < 3; i++) {
                paintTypeBox->addItem(paintTypeToText.at(static_cast<PaintType>(i)));
            }
            grossLayout->addWidget(paintTypeBox);
        }
        if (axisTabProcess) {
            tabName = "坐标轴";
            tabContent = new QWidget(this);
            grossLayout = new QVBoxLayout(tabContent);
            tabContent->setLayout(grossLayout);
            configTabsVec.push_back(std::pair<QString, QWidget*>(tabName, tabContent));

            auto axisTab = new QTabWidget();
            axisTab->setObjectName("axisTab");
            QStringList axisInitName = { "x轴", "y轴" ,"z轴" };
            for (int i = 0; i < 3; i++) {
                auto axisWidget = new QWidget();
                axisWidget->setObjectName("axis" + QString::number(i));
                auto axisLayout = new QVBoxLayout(axisWidget);
                axisWidget->setLayout(axisLayout);

                auto axisLabelLayout = new QHBoxLayout(axisWidget);
                auto axisLabelLabel = new QLabel(axisInitName[i] + "标签", axisWidget);
                auto axisLabelEditor = new QLineEdit(axisWidget);
                axisLabelEditor->setObjectName("axisLabel");
                axisLabelEditor->setText(axisInitName[i]);
                axisLabelLayout->addWidget(axisLabelLabel, 1);
                axisLabelLayout->addWidget(axisLabelEditor, inputStrech);
                axisLayout->addLayout(axisLabelLayout);

                auto autoSetRangeBoxLayout = new QHBoxLayout(axisWidget);
                auto autoSetAxisRangeLabel = new QLabel("自动设置轴范围", axisWidget);
                auto autoSetAxisRangeBox = new QCheckBox(axisWidget);
                autoSetAxisRangeBox->setObjectName("isAutoRange");
                autoSetAxisRangeBox->setChecked(true);
                autoSetRangeBoxLayout->addWidget(autoSetAxisRangeLabel, labelStrech);
                autoSetRangeBoxLayout->addWidget(autoSetAxisRangeBox, inputStrech);
                axisLayout->addLayout(autoSetRangeBoxLayout);

                auto setRangeEditorLayout = new QHBoxLayout(axisWidget);
                auto aSRLabel = new QLabel("    轴范围", axisWidget);
                auto aSRELowerBound = new QLineEdit(axisWidget);
                auto medLabel = new QLabel("-", axisWidget);
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
                auto autoSetAxisTickSpaceLabel = new QLabel("自动刻度间隔", axisWidget);
                auto autoSetAxisTickSpaceBox = new QCheckBox(axisWidget);
                autoSetAxisTickSpaceBox->setObjectName("isAutoTickSpace");
                autoSetAxisTickSpaceBox->setChecked(true);
                autoSetTickSpaceBoxLayout->addWidget(autoSetAxisTickSpaceLabel, labelStrech);
                autoSetTickSpaceBoxLayout->addWidget(autoSetAxisTickSpaceBox, inputStrech);
                axisLayout->addLayout(autoSetTickSpaceBoxLayout);

                auto setTickSpaceEditorLayout = new QHBoxLayout(axisWidget);
                auto aSTSLabel = new QLabel("    刻度间隔", axisWidget);
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
                axisTab->addTab(axisWidget, axisInitName[i] + "设置");
            }
            //axisTab->widget(2)->hide();
            grossLayout->addWidget(axisTab);
        }
        if (curveTabProcess) {
            tabName = "曲线";
            tabContent = new QWidget(this);
            grossLayout = new QVBoxLayout(tabContent);
            tabContent->setLayout(grossLayout);
            configTabsVec.push_back(std::pair<QString, QWidget*>(tabName, tabContent));

            auto curveButtonLayout = new QHBoxLayout(this);
            grossLayout->addLayout(curveButtonLayout);
            auto addCurveButton = new QPushButton(this);
            auto deleteCurveButton = new QPushButton(this);
            addCurveButton->setText("添加曲线");
            deleteCurveButton->setText("删除曲线");
            addCurveButton->setObjectName("addCurve");
            deleteCurveButton->setObjectName("deleteCurve");
            curveButtonLayout->addWidget(addCurveButton);
            curveButtonLayout->addWidget(deleteCurveButton);
            grossLayout->addLayout(curveButtonLayout);
            auto curveListWidget = new QListWidget(this);
            curveListWidget->setObjectName("curveList");
            grossLayout->addWidget(curveListWidget);
            
            auto warnLabel = new QLabel(this);
            warnLabel->setText("注意，有关曲线的修改会立即生效！");
            grossLayout->addWidget(warnLabel);
            
        }
        if (textTabProcess) {
            tabName = "文字";
            tabContent = new QWidget(this);
            grossLayout = new QVBoxLayout(tabContent);
            tabContent->setLayout(grossLayout);
            configTabsVec.push_back(std::pair<QString, QWidget*>(tabName, tabContent));

            auto curveButtonLayout = new QHBoxLayout(this);
            grossLayout->addLayout(curveButtonLayout);
            auto addCurveButton = new QPushButton(this);
            auto deleteCurveButton = new QPushButton(this);
            addCurveButton->setText("添加可拖动文字");
            deleteCurveButton->setText("删除文字");
            addCurveButton->setObjectName("addText");
            deleteCurveButton->setObjectName("deleteText");
            curveButtonLayout->addWidget(addCurveButton);
            curveButtonLayout->addWidget(deleteCurveButton);
            grossLayout->addLayout(curveButtonLayout);
            auto curveListWidget = new QListWidget(this);
            curveListWidget->setObjectName("textList");
            grossLayout->addWidget(curveListWidget);
        }
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
    void setContentEnabled(bool enable) {
        for (auto it = configTabsVec.begin(); it != configTabsVec.end(); it++) {
            (*it).second->setEnabled(enable);
        }
    }
    void loadContentFromFigure(BasicFigureData* basicData, FigureBasic* chart) {
        this->setContentEnabled(true);
        this->findChild<QLineEdit*>("nameEditor")->setText(basicData->figureName);
        //this->findChild<QCheckBox*>("isRealTime")->setChecked(basicData->isRealTime);
        this->findChild<QComboBox*>("paintTypeBox")->setCurrentIndex(static_cast<int>(basicData->paintType));
        for (int i = 0; i < chart->dim; i++) {
            auto axisWidget = this->findChild<QWidget*>("axis" + QString::number(i));
            axisWidget->findChild<QLineEdit*>("axisLabel")->setText(chart->getAxisLabel(i));
            axisWidget->findChild<QCheckBox*>("isAutoRange")->setChecked(chart->isAxisAutoRange(i));
            axisWidget->findChild<QCheckBox*>("isAutoTickSpace")->setChecked(chart->isAxisAutoTick(i));
            if (!chart->isAxisAutoRange(i)) {
                auto range_i = chart->getAxisRange(i);
                axisWidget->findChild<QLineEdit*>("lowerRange")->setText(QString::number(range_i[0]));
                axisWidget->findChild<QLineEdit*>("upperRange")->setText(QString::number(range_i[1]));
            }
            if (!chart->isAxisAutoTick(i)) {
                axisWidget->findChild<QLineEdit*>("tickSpace")->setText(QString::number(chart->getAxisTickSpacing(i)));
            }
        }
        ////注意!有关曲线的修改会立即生效。
        //auto curveList = this->findChild<QListWidget*>("curveList");
        //curveList->clear();
        //for (int i = 0; i < chart->getCurveNum(); i++) {
        //    //获取曲线颜色信息
        //    ItemWidget* itemWidget = static_cast<ItemWidget*>(curveList->itemWidget(
        //        this->addCurveItem(chart->getCurveName(i), chart->getCurveColor(i))));
        //    connect(itemWidget, &ItemWidget::itemDoubleClicked, this, [=]() {
        //        for (int i = 0; i < curveList->count(); i++) {
        //            QWidget* targetWidget = curveList->itemWidget(curveList->item(i));
        //            if (targetWidget == itemWidget) {
        //                chart->openSetCurveWidget(i);
        //                itemWidget->findChild<QLabel*>("curveItemName")->setText(chart->getCurveName(i));
        //                break;
        //            }
        //        }
        //        });
        //}
        ////对于文字List，分情况讨论。
        //if (basicData->figureType == FigureType::ThreeDimCurveFigure) {
        //    auto chart3d = static_cast<GLFigure3D*>(chart);
        //    auto textList = this->findChild<QListWidget*>("textList");
        //    textList->clear();
        //    textList->parentWidget()->show();
        //    for (int i = 0; i < chart3d->textInfoList.size(); i++) {
        //        auto textInfo = chart3d->textInfoList[i];
        //        //获取曲线颜色信息
        //        QColor qcolor(textInfo->color.x * 255, textInfo->color.y * 255, textInfo->color.z * 255);
        //        //创建item
        //        QListWidgetItem* item = new QListWidgetItem(textList);
        //        textList->setCurrentItem(item);
        //        QLabel* itemLabel = new QLabel();
        //        itemLabel->setText(QString::fromStdWString(textInfo->text));
        //        itemLabel->setStyleSheet("color:" + qcolor.name());
        //        textList->setItemWidget(item, itemLabel);
        //    }
        //}
        //else if (basicData->figureType == FigureType::TwoDimCurveFigure) {
        //    auto textList = this->findChild<QListWidget*>("textList");
        //    textList->parentWidget()->hide();
        //}


    }
    QListWidgetItem* addCurveItem(QString text, QColor qcolor) {
        //创建item
        auto curveList = this->findChild<QListWidget*>("curveList");
        QListWidgetItem* item = new QListWidgetItem(curveList);
        curveList->setCurrentItem(item);
        QLabel* itemLabel = new QLabel();

        ItemWidget* itemWidget = new ItemWidget();
        QHBoxLayout* itemLayout = new QHBoxLayout(itemWidget);
        itemWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        itemWidget->setLayout(itemLayout);
        itemLayout->addWidget(itemLabel);
        itemLabel->setText(text);
        itemLabel->setStyleSheet("color:" + qcolor.name());
        itemLabel->setObjectName("curveItemName");
        itemLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
        item->setSizeHint(itemWidget->sizeHint());
        curveList->setItemWidget(item, itemWidget);
        return item;
    }
};