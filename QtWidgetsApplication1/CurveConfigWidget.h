#pragma once
#include "AddFigureWidget.h"
#include "GLFigure3D.h"
#include "Figure2D.h"
#include "MyDialog.h"
#include <QtWidgets/QtWidgets>

class CurveConfigWidget :public QWidget {
public:
    //QPushButton* headButton;
    QVBoxLayout* grossLayout;
    QString title;
    QPushButton* addCurveButton;
    QPushButton* deleteCurveButton;
    QListWidget* curveListWidget;

    struct ScrollAreaList {
        QScrollArea* scroll;
        QWidget* container;
        QVBoxLayout* vbox;
    };
    CurveConfigWidget(QWidget* parent = nullptr) :QWidget(parent) {
        title = "曲线设置";
        grossLayout = new QVBoxLayout(this);
        this->setLayout(grossLayout);
        
        this->initConfigTabs();
        this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        this->show();
    };
    ~CurveConfigWidget() {
        this->grossLayout->deleteLater();
        this->deleteCurveButton->deleteLater();
        this->addCurveButton->deleteLater();
    };
    void initConfigTabs() {
        int labelStrech = 1;
        int inputStrech = 2;

        auto curveButtonLayout = new QHBoxLayout(this);
        grossLayout->addLayout(curveButtonLayout);
        addCurveButton = new QPushButton(this);
        deleteCurveButton = new QPushButton(this);
        addCurveButton->setText("添加曲线");
        deleteCurveButton->setText("删除曲线");
        addCurveButton->setObjectName("addCurve");
        deleteCurveButton->setObjectName("deleteCurve");
        curveButtonLayout->addWidget(addCurveButton);
        curveButtonLayout->addWidget(deleteCurveButton);
        grossLayout->addLayout(curveButtonLayout);
        curveListWidget = new QListWidget(this);
        curveListWidget->setObjectName("curveList");
        grossLayout->addWidget(curveListWidget);

        auto warnLabel = new QLabel(this);
        warnLabel->setText("注意，有关曲线的修改会立即生效！");
        grossLayout->addWidget(warnLabel);


        //if (textTabProcess) {
        //    tabName = "文字";
        //    tabContent = new QWidget(this);
        //    grossLayout = new QVBoxLayout(tabContent);
        //    tabContent->setLayout(grossLayout);
        //    configTabsVec.push_back(std::pair<QString, QWidget*>(tabName, tabContent));

        //    auto curveButtonLayout = new QHBoxLayout(this);
        //    grossLayout->addLayout(curveButtonLayout);
        //    auto addCurveButton = new QPushButton(this);
        //    auto deleteCurveButton = new QPushButton(this);
        //    addCurveButton->setText("添加可拖动文字");
        //    deleteCurveButton->setText("删除文字");
        //    addCurveButton->setObjectName("addText");
        //    deleteCurveButton->setObjectName("deleteText");
        //    curveButtonLayout->addWidget(addCurveButton);
        //    curveButtonLayout->addWidget(deleteCurveButton);
        //    grossLayout->addLayout(curveButtonLayout);
        //    auto curveListWidget = new QListWidget(this);
        //    curveListWidget->setObjectName("textList");
        //    grossLayout->addWidget(curveListWidget);
        //}
    };
    void loadContentFromFigure(BasicFigureData* basicData, FigureBasic* chart) {
        //注意!有关曲线的修改会立即生效。
        curveListWidget->clear();
        for (int i = 0; i < chart->getCurveNum(); i++) {
            //获取曲线颜色信息
            ItemWidget* itemWidget = static_cast<ItemWidget*>(curveListWidget->itemWidget(
                this->addCurveItem(chart->getCurveName(i), chart->getCurveColor(i))));
            connect(itemWidget, &ItemWidget::itemDoubleClicked, this, [=]() {
                for (int i = 0; i < curveListWidget->count(); i++) {
                    QWidget* targetWidget = curveListWidget->itemWidget(curveListWidget->item(i));
                    if (targetWidget == itemWidget) {
                        chart->openSetCurveWidget(i);
                        itemWidget->findChild<QLabel*>("curveItemName")->setText(chart->getCurveName(i));
                        break;
                    }
                }
                });
        }
        connect(addCurveButton, &QPushButton::clicked, this, [=]() {
            if (chart->isRunning) {
                QMessageBox::warning(nullptr, "Warning", "图像正在绘制。请停止运行后再修改曲线。");
                return;
            }
            else {
                addCurve(chart);
                //添加曲线。
            }
            });
        connect(deleteCurveButton, &QPushButton::clicked, this, [=]() {
            if (chart->isRunning) {
                QMessageBox::warning(nullptr, "Warning", "图像正在绘制。请停止运行后再修改曲线。");
                return;
            }
            else {
                deleteCurve(chart);
                //删除曲线。
            }
            });
        //对于文字List，分情况讨论。
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
    void deleteCurve(FigureBasic* figureCommon) {
        int index = this->curveListWidget->currentRow();
        qDebug() << index;

        auto item = this->curveListWidget->currentItem();
        auto itemWidget = this->curveListWidget->itemWidget(item);
        itemWidget->deleteLater();
        delete item;
        figureCommon->deleteCurve(index);
        figureCommon->lineDataType.erase(figureCommon->lineDataType.begin() + index);
    }
    void addCurve(FigureBasic* figureCommon) {
        //创建添加曲线界面。
        auto addCurveWidget = new AddCurveWidget();
        addCurveWidget->setBasicContent(this->curveListWidget->count() + 1, figureCommon->dim);
        MyDialog dialog;
        dialog.setContentAndButton(addCurveWidget,
            "添加曲线", "添加", "取消");

        //按钮事件响应，三种退出方式。
        connect(dialog.createButton, &QPushButton::clicked, &dialog, &QDialog::accept);
        connect(dialog.createButton, &QPushButton::clicked, &dialog, [=]() {
            //获取曲线颜色信息
            QColor qcolor(addCurveWidget->colorSelectButton->text());
            QString text = addCurveWidget->nameEditor->text();
            ItemWidget* itemWidget = static_cast<ItemWidget*>(this->curveListWidget->itemWidget(
                this->addCurveItem(text, qcolor)));
            //添加曲线。
            int dim = figureCommon->dim;

            std::vector<DataType> dataTypes;
            for (int curveTypeI = 0; curveTypeI < dim; curveTypeI++) {
                dataTypes.push_back(addCurveWidget->getDataType(curveTypeI));
            }
            figureCommon->addCurve(addCurveWidget->nameEditor->text(), qcolor, addCurveWidget->widthEditor->text().toFloat(), dataTypes);
            connect(itemWidget, &ItemWidget::itemDoubleClicked, this, [=]() {
                auto listWidget = this->curveListWidget;
                for (int i = 0; i < listWidget->count(); i++) {
                    QWidget* targetWidget = listWidget->itemWidget(listWidget->item(i));
                    if (targetWidget == itemWidget) {
                        figureCommon->openSetCurveWidget(i);
                        itemWidget->findChild<QLabel*>("curveItemName")->setText(figureCommon->getCurveName(i));
                        break;
                    }
                }
                });
            addCurveWidget->deleteLater();
            });
        connect(dialog.cancelButton, &QPushButton::clicked, &dialog, &QDialog::accept);
        connect(dialog.cancelButton, &QPushButton::clicked, addCurveWidget, &QWidget::deleteLater);
        connect(&dialog, &MyDialog::closedRespond, &dialog, &QDialog::accept);
        connect(&dialog, &MyDialog::closedRespond, addCurveWidget, &QWidget::deleteLater);

        dialog.exec();
        return;
    }

};