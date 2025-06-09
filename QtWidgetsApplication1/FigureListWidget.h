#pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1600)    
# pragma execution_character_set("utf-8")    
#endif

#include <QtWidgets/QMainWindow>
#include <QtCore/QLibrary>
#include <QtGui>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QtWidgets>
#include <QtCharts/qchartview.h>
#include <QLineSeries>
#include <QValueAxis>

#include <array>

#include "ItemWidget.h"
#include "figureInfo.h"
#include "AddFigureWidget.h"
#include "AddCurveWidget.h"
#include "GLFigure3D.h"
#include "Figure2D.h"
#include "Interval.h"
#include "FigureConfigWidget.h"
#include "GLResource.h"
#include "StatusDockWidget.h"
#include "MyDialog.h"
#include "CurveConfigWidget.h"
#include "DefaultWidgetSize.h"

QT_BEGIN_NAMESPACE

class FigureListWidget : public QWidget
{
    Q_OBJECT
signals:
    void loadFigureData(BasicFigureData* bfd);
public:
    GLResource* figure3DResource;
    std::list<QString> head_name = {"id", 
                                    "ͼ��", 
                                    "��ͼ����",
                                    "ʵʱ��ͼ",
                                    "״̬" };
    std::list<std::type_info> widget_type = {};
    QListWidget* listWidget;
    QSplitter* headSplitter;
    int splitterWidth = 1;
    QFont font;
    QMainWindow* dockParent;
    QPushButton* addPaintButton;
    QPushButton* deletePaintButton;
    QDockWidget* dockBody;

    BasicFigureData* tempBasicFigureData;
    struct TwoDimFigureData {
        Figure2D* chart;
    }*tempTwoDimFigureData;
    struct ThreeDimFigureData {
        GLFigure3D* chart;
    }*tempThreeDimFigureData;

    std::map<QWidget*, BasicFigureData*> figureDataMap;//���ֻ��Ƶ��Ⱥ�

    AddFigureWidget* addFigureWidget;
    AddCurveWidget* addCurveWidget;
    FigureConfigWidget* figureConfigWidget;
    CurveConfigWidget* curveConfigWidget;

    FigureListWidget(QMainWindow* dockParent, QDockWidget* dockBody ,QWidget* parent = nullptr) :QWidget(parent) {
        figureConfigWidget = new FigureConfigWidget(this);
        figureConfigWidget->hide();
        curveConfigWidget = new CurveConfigWidget(this);
        curveConfigWidget->hide();
        this->dockParent = dockParent;
        this->dockBody = dockBody;
        initUi();

        figure3DResource = new GLResource();
        figure3DResource->show();
        figure3DResource->hide();
        //figure3DResource->makeCurrent(nullptr);
        //figure3DResource->initializeGL();
    };
    ~FigureListWidget() {
        figureConfigWidget->deleteLater();
        curveConfigWidget->deleteLater();
        for (auto it = figureDataMap.begin(); it != figureDataMap.end(); it++) {
            deleteFigureDataPtr(it->second);
        }
        figureDataMap.clear();

        //���ڿ�ָ��ʹ��deleteLater�ǰ�ȫ�ġ����Ƕ���delete����ָ��ò�ƻᱨ��
        //addFigureWidget->deleteLater();
        //addCurveWidget->deleteLater();
        listWidget->deleteLater();
        headSplitter->deleteLater();
        figure3DResource->deleteLater();
        this->deleteLater();
    };

    void initUi() {
        QVBoxLayout* vlayout = new QVBoxLayout(this); vlayout->setMargin(5);
        QHBoxLayout* buttonLayout = new QHBoxLayout(); //buttonLayout->setMargin(0);
        QFont font("Arial", 16, QFont::Bold); // �������ơ��ֺź�������ʽ
        // �����Զ���QWidget��Ϊitem widget
        listWidget = new QListWidget(this);
        listWidget->setDragEnabled(true);        // ������ק
        listWidget->setDropIndicatorShown(true); // ��ʾ��קָʾ��

        vlayout->addWidget(listWidget);
        vlayout->addLayout(buttonLayout);
        addPaintButton = new QPushButton();
        addPaintButton->setText("��ӻ�ͼ");
        connect(addPaintButton, &QPushButton::clicked, this, &FigureListWidget::addFigure);
        deletePaintButton = new QPushButton();
        deletePaintButton->setText("ɾ����ͼ");
        connect(deletePaintButton, &QPushButton::clicked, this, &FigureListWidget::deleteFigure);
        buttonLayout->addWidget(addPaintButton);
        buttonLayout->addWidget(deletePaintButton);

        QListWidgetItem* item = new QListWidgetItem(listWidget);
        QWidget* item_widget = new QWidget(this);
        //item_widget->setFixedHeight(15);
        //item_widget->setFixedSize(QSize(120, 120));
        QHBoxLayout* layout = new QHBoxLayout(item_widget);
        headSplitter = new QSplitter(Qt::Horizontal, item_widget);
        headSplitter->setObjectName("splitter");

        int stretchLength[] = { 1, 2, 2, 1 ,1};
        int i = 0;
        for (std::list<QString>::iterator it = head_name.begin(); it != head_name.end(); it++) {
            QPushButton* headButton = new QPushButton();
            headButton->setText(*it);
            //QLabel* label = new QLabel(*it);
            //QFont font = label->font();  // ��ȡ��ǰ����
            //font.setBold(true);         // ��������Ӵ�
            //label->setFont(font);
            //label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
            headButton->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
            headButton->setStyleSheet("QPushButton {border: 0px solid;border-radius: 0px;}"
                "QPushButton:hover {"
                "background-color: #D9EBF9;" // �����ͣʱ�ı�����ɫ
                "}"
                "QPushButton:pressed {"
                "background-color: #BCDCF4;" // �����ͣʱ�ı�����ɫ188 220 244
                "}"); // Բ�� }");
            headSplitter->addWidget(headButton);
            headSplitter->setStretchFactor(headSplitter->count() - 1, stretchLength[i++]);
        }
        
        qDebug() << head_name.size();
        headSplitter->setStyleSheet("QSplitter::handle { background-color: black }"); //���÷ֽ��ߵ���ʽ
        headSplitter->setHandleWidth(splitterWidth);
        QObject::connect(headSplitter, &QSplitter::splitterMoved, this, &FigureListWidget::resizeItemElements);

        layout->addWidget(headSplitter);
        layout->setMargin(0);

        item_widget->setFixedHeight(20);
        item_widget->sizeHint().width();
        item->setSizeHint(QSize(item_widget->sizeHint().width(), 20));
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable &~Qt::ItemIsTristate);
        //item->setBackground(QBrush(QColor(200, 200, 200)));
        listWidget->setContentsMargins(0, 0, 0, 0);
        listWidget->setItemWidget(item, item_widget);
    }
    void addItem(BasicFigureData* basicFigureData) {
        //ʵʱ��ͼ��
        //����λ��
        //ͼ���Ƿ��ѱ���
        QListWidgetItem* item = new QListWidgetItem(listWidget);
        listWidget->setCurrentItem(item);
        ItemWidget* item_widget = new ItemWidget(this);
        QHBoxLayout* layout = new QHBoxLayout(item_widget);
        QSplitter* pHSplitter = new QSplitter(Qt::Horizontal, item_widget);
        layout->addWidget(pHSplitter);
        layout->setMargin(0);
        pHSplitter->setObjectName("splitter");
        QObject::connect(pHSplitter, &QSplitter::splitterMoved, this, &FigureListWidget::resizeItemElements);

        QLabel* idLabel = new QLabel(QString::number(listWidget->count()-1), pHSplitter);
        idLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
        idLabel->setObjectName("id");
        pHSplitter->addWidget(idLabel);
        idLabel->setAlignment(Qt::AlignCenter);

        QLabel* nameLabel = new QLabel(basicFigureData->figureName, pHSplitter);
        nameLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
        pHSplitter->addWidget(nameLabel);
        nameLabel->setAlignment(Qt::AlignCenter);
        nameLabel->setObjectName("figureName");

        QLabel* figureTypeLabel = new QLabel(figureTypeToText[basicFigureData->figureType], pHSplitter);
        figureTypeLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
        figureTypeLabel->setObjectName("figureType");
        figureTypeLabel->setAlignment(Qt::AlignCenter);
        pHSplitter->addWidget(figureTypeLabel);

        QCheckBox* paintRealTime = new QCheckBox(item_widget);
        paintRealTime->setText("");
        if (basicFigureData->paintType == PaintType::LoadData) {
            paintRealTime->setEnabled(false);
        }
        else if (basicFigureData->paintType == PaintType::RealTime) {
            paintRealTime->setEnabled(true);
            paintRealTime->setChecked(true);
        }
        else if (basicFigureData->paintType == PaintType::RunningData) {
            paintRealTime->setEnabled(true);
            paintRealTime->setChecked(false);
        }
        connect(paintRealTime, &QCheckBox::clicked, this, [=]() {
            if (paintRealTime->isChecked()) {
                basicFigureData->paintType = PaintType::RealTime;
            }
            else{
                basicFigureData->paintType = PaintType::RunningData;
            }
            });

        paintRealTime->setObjectName("figureRealTime");
        QWidget* tempLabel = new QWidget(item_widget);
        tempLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
        QHBoxLayout* tempLayout = new QHBoxLayout(tempLabel);
        tempLabel->setLayout(tempLayout);
        tempLayout->addStretch(1);
        tempLayout->addWidget(paintRealTime);
        tempLayout->addStretch(1);
        tempLayout->setMargin(2); //tempLabel->setMinimumHeight(0);
        pHSplitter->addWidget(tempLabel);


        QPushButton* statusButton = new QPushButton(pHSplitter);
        statusButton->setText("����");
        statusButton->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
        statusButton->setObjectName("status");
        statusButton->setMinimumWidth(1);
        connect(statusButton, &QPushButton::clicked, this, [=]() {
            if (basicFigureData->status == figureStatus::hide) {
                openFigure(item_widget);
            }
            else if (basicFigureData->status == figureStatus::show) {
                basicFigureData->dockWindow->close();
            }
        });
        pHSplitter->addWidget(statusButton);


        pHSplitter->setSizes(headSplitter->sizes());

        item->setSizeHint(item_widget->sizeHint());
        //QObject::connect(item_widget, &ItemWidget::itemDoubleClicked, this, &FigureListWidget::openFigure);
        listWidget->setItemWidget(item, item_widget);

        connect(item_widget, &ItemWidget::itemDoubleClicked, this, &FigureListWidget::doubleClickItem);
        figureDataMap[item_widget] = basicFigureData;
        statusButton->click();
        //basicFigureData = nullptr;
    }
    void initFigure() {

    }

    void changeFigureType() {
        if (static_cast<FigureType>(addFigureWidget->figureTypeBox->currentIndex()) == tempBasicFigureData->figureType) {
            return;
        }
        if (addFigureWidget->curveListWidget->count() > 0) {
            QMessageBox msgBox;
            msgBox.setWindowTitle("��ʾ");
            msgBox.setText("�Ƿ�ȷ���ı��ͼ���ͣ��ᶪʧ��ǰ�Ѽ����б���������ߡ�");

            QPushButton * yesButton = msgBox.addButton("ȷ��", QMessageBox::YesRole);
            QPushButton * noButton = msgBox.addButton("ȡ��", QMessageBox::NoRole);

            msgBox.exec();
            if (msgBox.clickedButton() == yesButton) {
                addFigureWidget->curveListWidget->clear();
                changeFigureTypeTempData();
            }
            else {
                auto index = static_cast<int>(tempBasicFigureData->figureType);
                addFigureWidget->figureTypeBox->setCurrentIndex(index);
                //û�иı䣬����Ҫ�����������
            }
        }
        else {
            changeFigureTypeTempData();
        }
        
    }
    void changeFigureTypeTempData() {
        qDebug() << "change figure type to " + QString::number(addFigureWidget->figureTypeBox->currentIndex());
        if (tempBasicFigureData->figureType == FigureType::TwoDimCurveFigure) {//�ͷ��ڴ�
            delete tempTwoDimFigureData->chart;
            delete tempTwoDimFigureData;
            tempTwoDimFigureData = nullptr;
        }
        else if (tempBasicFigureData->figureType == FigureType::ThreeDimCurveFigure) {//�ͷ��ڴ�
            delete tempThreeDimFigureData->chart;
            delete tempThreeDimFigureData;
            tempThreeDimFigureData = nullptr;
        }
        //�������ͷ����µ��ڴ棡��������basicFigureData��ָ�룡
        
        tempBasicFigureData->figureType = static_cast<FigureType>(addFigureWidget->figureTypeBox->currentIndex());
        if (tempBasicFigureData->figureType == FigureType::TwoDimCurveFigure) {
            tempTwoDimFigureData = new TwoDimFigureData;
            tempBasicFigureData->figureData = (void*)tempTwoDimFigureData;
            tempTwoDimFigureData->chart = new Figure2D();
            if (addFigureWidget->axisTab->count() > 2) {
                addFigureWidget->axisTab->removeTab(2);
            }
        }
        else if (tempBasicFigureData->figureType == FigureType::ThreeDimCurveFigure) {
            tempThreeDimFigureData = new ThreeDimFigureData;
            tempBasicFigureData->figureData = (void*)tempThreeDimFigureData;
            tempThreeDimFigureData->chart = new GLFigure3D(this->figure3DResource);
            if (addFigureWidget->axisTab->count() < 3) {
                addFigureWidget->axisTab->addTab(addFigureWidget->axisWidgetVec[2], "z������");
            }
        }
    }

    void addCurve() {
        //����������߽��档
        addCurveWidget = new AddCurveWidget();
        addCurveWidget->setBasicContent(addFigureWidget->curveListWidget->count() + 1,
            static_cast<FigureType>(addFigureWidget->figureTypeBox->currentIndex()));
        MyDialog dialog;
        dialog.setContentAndButton(addCurveWidget,
            "�������", "���", "ȡ��");
        dialog.resize(defaultAddCurveDialogWidth, defaultAddCurveDialogHeight);
        
        //��ť�¼���Ӧ�������˳���ʽ��
        connect(dialog.createButton, &QPushButton::clicked, &dialog, &QDialog::accept);
        connect(dialog.createButton, &QPushButton::clicked, &dialog, [=]() {
            //��ȡ������ɫ��Ϣ
            QColor qcolor(addCurveWidget->colorSelectButton->text());
            QString text = addCurveWidget->nameEditor->text();
            ItemWidget* itemWidget = static_cast<ItemWidget*>(addFigureWidget->curveListWidget->itemWidget(
                addFigureWidget->addCurveItem(text, qcolor)));
            //������ߡ�
            FigureBasic* figureCommon = getFigureCommon(tempBasicFigureData); 
            int dim = getFigureDim(tempBasicFigureData->figureType);
            
            std::vector<DataType> dataTypes;
            for (int curveTypeI = 0; curveTypeI < dim; curveTypeI++) {
                dataTypes.push_back(addCurveWidget->getDataType(curveTypeI));
            }
            figureCommon->addCurve(addCurveWidget->nameEditor->text(), qcolor, addCurveWidget->widthEditor->text().toFloat(), dataTypes);
            connect(itemWidget, &ItemWidget::itemDoubleClicked, this, [=]() {
                auto listWidget = addFigureWidget->curveListWidget;
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
    void deleteCurve() {
        int index = addFigureWidget->curveListWidget->currentRow();
        qDebug() << index;
        
        auto item = addFigureWidget->curveListWidget->currentItem();
        auto itemWidget = addFigureWidget->curveListWidget->itemWidget(item);
        itemWidget->deleteLater();
        delete item;
        auto figureCommon = this->getFigureCommon(tempBasicFigureData);
        figureCommon->deleteCurve(index);
        figureCommon->lineDataType.erase(figureCommon->lineDataType.begin() + index);
    }

    void doubleClickItem() {
        QWidget* itemWidget = (QWidget*)sender();
        BasicFigureData* basicFigureData = figureDataMap[itemWidget];
        if (basicFigureData->status == figureStatus::hide) {
            openFigure(itemWidget);
        }
        else if(basicFigureData->status == figureStatus::show) {
            basicFigureData->dockWindow->raise();
        }
        
    }
    void openFigure(QWidget* itemWidget) {//���޸ġ�//Ϊʲô�����ô�����QDockWidget�޷����أ�
        BasicFigureData* basicFigureData = figureDataMap[itemWidget];
        StatusDockWidget* figureDockWidget;
        if (basicFigureData->dockWindow != nullptr) {
            basicFigureData->dockWindow->deleteLater();
            qDebug() << "one auto deleted";
        }
        figureDockWidget = new StatusDockWidget(basicFigureData->figureName, dockParent);
        basicFigureData->dockWindow = figureDockWidget;

        QWidget* grossWidget = new QWidget(figureDockWidget);
        auto grossLayout = new QHBoxLayout(grossWidget);
        grossWidget->setLayout(grossLayout);
        figureDockWidget->setWidget(grossWidget);

        auto settingLayout = new QVBoxLayout();
        auto figureSettingButton = new QPushButton(grossWidget);
        figureSettingButton->setText("��\n��\nͼ\n��"); figureSettingButton->setFixedWidth(30);
        connect(figureSettingButton, &QPushButton::clicked, this, [=]() {
            this->openSetFigureDialog(basicFigureData, this->getFigureCommon(basicFigureData));
            });

        auto configCurveButton = new QPushButton(grossWidget); configCurveButton->setFixedWidth(30);
        configCurveButton->setText("��\n��\n��\n��");
        connect(configCurveButton, &QPushButton::clicked, this, [=]() {
            this->openSetCurveDialog(basicFigureData, this->getFigureCommon(basicFigureData));
            });

        auto figureLoadButton = new QPushButton(grossWidget); figureLoadButton->setFixedWidth(30);
        figureLoadButton->setText("��\n��\n��\n��");
        connect(figureLoadButton, &QPushButton::clicked, this, [=]() {
            auto figureCommon = getFigureCommon(basicFigureData);
            if (figureCommon->isRunning) {
                QMessageBox::warning(nullptr, "Warning", "����ֹͣ���к����������ݡ�");
                return;
            }

            emit loadFigureData(basicFigureData);
            return;

            });

        auto figureClearButton = new QPushButton(grossWidget); figureClearButton->setFixedWidth(30);
        figureClearButton->setText("��\n��\nͼ\n��");
        connect(figureClearButton, &QPushButton::clicked, this, [=]() {
            auto figureCommon = getFigureCommon(basicFigureData);
            if (figureCommon->isRunning) {
                QMessageBox::warning(nullptr, "Warning", "����ֹͣ���к������ͼ�����ݵ㡣");
                return;
            }
            figureCommon->clearCurveData();
            });

        settingLayout->addWidget(figureSettingButton);
        settingLayout->addWidget(configCurveButton);
        settingLayout->addWidget(figureLoadButton);
        settingLayout->addWidget(figureClearButton);


        if (basicFigureData->figureType == FigureType::TwoDimCurveFigure) {
            //figureDockWidget->setFeatures(figureDockWidget->features() & ~QDockWidget::DockWidgetClosable);
            TwoDimFigureData* figureData = (TwoDimFigureData*)basicFigureData->figureData;
            grossLayout->addWidget(figureData->chart->chartView, 1);
        }
        else if (basicFigureData->figureType == FigureType::ThreeDimCurveFigure) {
            ThreeDimFigureData* figureData = (ThreeDimFigureData*)basicFigureData->figureData;
            grossLayout->addWidget(figureData->chart, 1);
        }
        grossLayout->addLayout(settingLayout);
        
        //figureDockWidget->resize(500, 400);
        //
        //qDebug() << dockParent->dockWidgetArea(dockBody);
        //dockParent->addDockWidget(Qt::LeftDockWidgetArea, figureDockWidget);
        dockParent->tabifyDockWidget(dockBody, figureDockWidget);
        figureDockWidget->show();
        figureDockWidget->raise();
        //
        basicFigureData->status = figureStatus::show;
        itemWidget->findChild<QPushButton*>("status")->setText("��ʾ");
        connect(basicFigureData->dockWindow, &StatusDockWidget::closedRespond, this, [=]() {
            basicFigureData->status = figureStatus::hide;
            itemWidget->findChild<QPushButton*>("status")->setText("����");
            });
    }
    void addFigure() {
        //��ʼ����ʱ���ݣ�ע��ֻ��tempBasicFigure��Ϊ��һ��figureTypeʱ������ʼ����
        if (true) {
            tempBasicFigureData = new BasicFigureData;
            tempTwoDimFigureData = new TwoDimFigureData;
            //tempThreeDimFigureData = nullptr;

            assert(static_cast<FigureType>(0) == FigureType::TwoDimCurveFigure);
            tempBasicFigureData->figureType = static_cast<FigureType>(0);
            tempBasicFigureData->figureName = "";
            //tempBasicFigureData->isRealTime = false;
            tempBasicFigureData->paintType = PaintType::RealTime;
            tempBasicFigureData->figureData = (void*)tempTwoDimFigureData;
            //���ԭ����
            tempTwoDimFigureData->chart = new Figure2D();
        }
        
        //��������
        addFigureWidget = new AddFigureWidget();
        addFigureWidget->nameEditor->setText("��ͼ" + QString::number(listWidget->count()));
        connect(addFigureWidget->addCurveButton, &QPushButton::clicked, this, &FigureListWidget::addCurve);
        connect(addFigureWidget->deleteCurveButton, &QPushButton::clicked, this, &FigureListWidget::deleteCurve);
        connect(addFigureWidget->figureTypeBox, &QComboBox::currentTextChanged, this, &FigureListWidget::changeFigureType);
        for (int i = 0; i < figureTypeToText.size(); i++)
            addFigureWidget->figureTypeBox->addItem(figureTypeToText[static_cast<FigureType>(i)]);

        MyDialog dialog;
        dialog.setWindowTitle("������ͼ");
        dialog.setContentAndButton(addFigureWidget,
            "������ͼ", "����", "ȡ��");

        
        //������Ӧ����
        connect(addFigureWidget, &QWidget::destroyed, &dialog, &QDialog::accept);
        connect(dialog.createButton, &QPushButton::clicked, &dialog, [=]() {
            //����itemʱֻչʾ�������ݣ���չʾ�������ݡ�
            tempBasicFigureData->figureName = addFigureWidget->nameEditor->text();
            //tempBasicFigureData->isRealTime = addFigureWidget->paintRealTime->isChecked();
            tempBasicFigureData->paintType = static_cast<PaintType>(addFigureWidget->paintTypeBox->currentIndex());
            //����������
            FigureBasic* commonChart = getFigureCommon(tempBasicFigureData);
            commonChart->setFigureTitle(tempBasicFigureData->figureName);
            int dim = getFigureDim(tempBasicFigureData->figureType);
            for (int i = 0; i < dim; i++) {
                auto axisWidget = addFigureWidget->axisWidgetVec[i];
                commonChart->setAxisAutoRange(i, axisWidget->findChild<QCheckBox*>("isAutoRange")->isChecked());
                if (!commonChart->isAxisAutoRange(i)) {
                    float upper = axisWidget->findChild<QLineEdit*>("upperRange")->text().toFloat();
                    float lower = axisWidget->findChild<QLineEdit*>("lowerRange")->text().toFloat();
                    if (upper < lower) {
                        QMessageBox msgBox;//����Ӧ�÷��ڲ��������֮ǰ��Ʃ��������ڳ�ʼ��config֮����ôÿ�ξ��淵�ض������´���config������ڴ�й¶��
                        msgBox.setWindowTitle("����");
                        msgBox.setText(axisText[i] + "�ķ�Χ���ò������뱣֤��Χ�½�С�ڷ�Χ�Ͻ磡");

                        QPushButton* yesButton = msgBox.addButton("ȷ��", QMessageBox::YesRole);
                        msgBox.exec();
                        return;
                    }
                    commonChart->setAxisRange(i, axisWidget->findChild<QLineEdit*>("lowerRange")->text().toDouble(),
                        axisWidget->findChild<QLineEdit*>("upperRange")->text().toDouble());
                }
                commonChart->setAxisAutoTick(i, axisWidget->findChild<QCheckBox*>("isAutoTickSpace")->isChecked());
                if (!commonChart->isAxisAutoTick(i)) {
                    commonChart->setAxisTickType(i, static_cast<TickType>(axisWidget->findChild<QComboBox*>("tickType")->currentIndex()));
                    if (commonChart->getAxisTickType(i) == TickType::TickSpace) {
                        commonChart->setAxisTickSpacing(i, axisWidget->findChild<QLineEdit*>("tickSpace")->text().toDouble());
                    }
                    else if (commonChart->getAxisTickType(i) == TickType::TickNum) {
                        commonChart->setAxisTickSpaceNum(i, axisWidget->findChild<QLineEdit*>("tickSpace")->text().toInt());
                    }
                }
                commonChart->setAxisLabel(i, axisWidget->findChild<QLineEdit*>("axisLabel")->text());
            }
            for (int i = 0; i < dim; i++) {
                if (!commonChart->isAxisAutoTick(i) && !commonChart->isAxisAutoRange(i) &&
                    commonChart->getAxisTickType(i) == TickType::TickSpace &&
                    (commonChart->getAxisRange(i)[1] - commonChart->getAxisRange(i)[0]) / commonChart->getAxisTickSpacing(i) > 20) {
                    QMessageBox msgBox;//����Ӧ�÷��ڲ��������֮ǰ��Ʃ��������ڳ�ʼ��config֮����ôÿ�ξ��淵�ض������´���config������ڴ�й¶��
                    msgBox.setWindowTitle("����");
                    msgBox.setText(axisText[i] + "�Ŀ̶ȼ�����ò�������ǰ�����£��̶��������ᳬ��20�����Ӱ��ͼ�ε���ʾЧ������ɿ��٣�");

                    QPushButton* yesButton = msgBox.addButton("ȷ��", QMessageBox::YesRole);
                    msgBox.exec();
                    return;
                }
                if (!commonChart->isAxisAutoTick(i) && commonChart->isAxisAutoRange(i) && 
                    commonChart->getAxisTickType(i) == TickType::TickSpace) {
                    QMessageBox msgBox;
                    msgBox.setWindowTitle("��ʾ");
                    msgBox.setText("ע�⣬��ֻ�����Զ��᷶Χ���̶���̶ȼ��ʱ�����ʵ�ʻ��Ƶ���̶���������20��Ϊ�˱�֤��ͼЧ���������Զ������µ���̶ȼ����");

                    QPushButton* yesButton = msgBox.addButton("ȷ��", QMessageBox::YesRole);
                    msgBox.exec();
                    break;
                }
            }

            
            /*
            if (tempBasicFigureData->figureType == FigureType::ThreeDimCurveFigure) {
                auto ovptr = (ThreeDimFigureData*)tempBasicFigureData->figureData;
                for (int i = 0; i < 3; i++) {
                    auto axisWidget = addFigureWidget->axisWidgetVec[i];
                    GLFigure3D::Axis* axis_i = ovptr->chart->axis_list[i];
                    //tick���ú�interval����˳�����⡣��ϲ����Ѿ����������ˡ�
                    if (!axisWidget->findChild<QCheckBox*>("isAutoTickSpace")->isChecked()) {
                        axis_i->autoTick = false;
                        axis_i->tickType = static_cast<TickType>(axisWidget->findChild<QComboBox*>("tickType")->currentIndex());
                        if (axis_i->tickType == TickType::TickSpace) {
                            ovptr->chart->setAxisTick(i, axisWidget->findChild<QLineEdit*>("tickSpace")->text().toFloat());
                        }
                        else if(axis_i->tickType == TickType::TickNum){
                            axis_i->tickNum =  axisWidget->findChild<QLineEdit*>("tickSpace")->text().toInt();
                        }
                    }
                    //ע�⣡�п���lowerRange����upperRange�����ҵ�����Ԥ��������������緢�������ź���ȷ��accept�ź�
                    if (!axisWidget->findChild<QCheckBox*>("isAutoRange")->isChecked()) {
                        float upper = axisWidget->findChild<QLineEdit*>("upperRange")->text().toFloat();
                        float lower = axisWidget->findChild<QLineEdit*>("lowerRange")->text().toFloat();
                        if (upper < lower) {
                            QMessageBox msgBox;//����Ӧ�÷��ڲ��������֮ǰ��Ʃ��������ڳ�ʼ��config֮����ôÿ�ξ��淵�ض������´���config������ڴ�й¶��
                            msgBox.setWindowTitle("����");
                            msgBox.setText(axisText[i] + "�ķ�Χ���ò������뱣֤��Χ�½�С�ڷ�Χ�Ͻ磡");

                            QPushButton* yesButton = msgBox.addButton("ȷ��", QMessageBox::YesRole);
                            msgBox.exec();
                            return;
                        }
                        axis_i->autoInterval = false;
                        ovptr->chart->setAxisRange(i, lower, upper);
                    }
                    axis_i->label = axisWidget->findChild<QLineEdit*>("axisLabel")->text().toStdWString();//��������
                }
                for (int i = 0; i < 3; i++) {
                    GLFigure3D::Axis* axis_i = ovptr->chart->axis_list[i];
                    auto axisWidget = addFigureWidget->axisWidgetVec[i];
                    if (!axis_i->autoTick && !axis_i->autoInterval && axis_i->tickType == TickType::TickSpace && (axis_i->interval[1]-axis_i->interval[0])*axis_i->propValue / axis_i->tickSpacing > 20) {
                        QMessageBox msgBox;//����Ӧ�÷��ڲ��������֮ǰ��Ʃ��������ڳ�ʼ��config֮����ôÿ�ξ��淵�ض������´���config������ڴ�й¶��
                        msgBox.setWindowTitle("����");
                        msgBox.setText(axisText[i] + "�Ŀ̶ȼ�����ò�������ǰ�����£��̶��������ᳬ��20�����Ӱ��ͼ�ε���ʾЧ������ɿ��٣�");

                        QPushButton* yesButton = msgBox.addButton("ȷ��", QMessageBox::YesRole);
                        msgBox.exec();
                        return;
                    }
                    if (!axis_i->autoTick && axis_i->autoInterval && axis_i->tickType==TickType::TickSpace) {
                        QMessageBox msgBox;
                        msgBox.setWindowTitle("��ʾ");
                        msgBox.setText("ע�⣬��ֻ�����Զ��᷶Χ���̶���̶ȼ��ʱ�����ʵ�ʻ��Ƶ���̶���������20��Ϊ�˱�֤��ͼЧ���������Զ������µ���̶ȼ����");

                        QPushButton* yesButton = msgBox.addButton("ȷ��", QMessageBox::YesRole);
                        msgBox.exec();
                        break;
                    }
                }
                ovptr->chartConfig = new Figure3DConfig(ovptr->chart);
                ovptr->chartConfig->loadContentFromFigure(tempBasicFigureData, ovptr->chart);
            }*/
            //ע�⣬tempBasicFigureData�е�ͼ�������ݵ�void*����
            // ָ���Ѿ��ڸú�����ͷ������changeFigureTypeTempData��������ˣ��������ﲻ��Ҫ�����á�
            this->addItem(tempBasicFigureData);
            tempBasicFigureData = nullptr;
            addFigureWidget->deleteLater();
            addFigureWidget = nullptr;
            });
        connect(dialog.cancelButton, &QPushButton::clicked, &dialog, [=]() {
            deleteFigureDataPtr(tempBasicFigureData);
            addFigureWidget->deleteLater();
            addFigureWidget = nullptr;
            });
        connect(&dialog, &MyDialog::closedRespond, &dialog, [=]() {
            deleteFigureDataPtr(tempBasicFigureData);
            addFigureWidget->deleteLater();
            addFigureWidget = nullptr;
            });
        dialog.exec();
        return;
    }
    void deleteFigure() {
        auto item = listWidget->currentItem();
        auto itemWidget = listWidget->itemWidget(item);
        deleteFigureDataPtr(figureDataMap[itemWidget]);//�ͷ�ָ��ָ��Ŀռ�(ע�������ͷ�void*ָ��ָ��Ŀռ䣡����)
        figureDataMap.erase(itemWidget);
        listWidget->removeItemWidget(item);
        itemWidget->deleteLater();
        delete item;
        for (int i = 1; i < listWidget->count(); i++) {
            QLabel* idLabel = listWidget->itemWidget(listWidget->item(i))->findChild<QLabel*>("id");
            idLabel->setText(QString::number(i));
        }
    }
    void deleteFigureDataPtr(BasicFigureData* basicFigureData) {
        if (basicFigureData) {
            auto vptr = basicFigureData->figureData;
            //�ͷ�figureData��ע����Ҫת����ԭָ������
            if (basicFigureData->figureType == FigureType::TwoDimCurveFigure) {
                auto ovptr = (TwoDimFigureData*)vptr;
                delete ovptr->chart;
                delete ovptr;
            }
            else if (basicFigureData->figureType == FigureType::ThreeDimCurveFigure) {
                auto ovptr = (ThreeDimFigureData*)vptr;
                delete ovptr->chart;
                delete ovptr;
                //���������͡�
            }
            //�ͷ�ͣ������
            if (basicFigureData->dockWindow != nullptr) {
                basicFigureData->dockWindow->deleteLater();
                qDebug() << "one auto deleted";
            }
            //�ͷŻ�����Ϣ
            delete basicFigureData;
        }
        
    }

    FigureBasic* getFigureCommon(BasicFigureData* basicFigureData) {
        auto figureType = basicFigureData->figureType;
        auto figureData = basicFigureData->figureData;
        FigureBasic* figureCommon = nullptr;
        if (figureType == FigureType::TwoDimCurveFigure) {
            figureCommon = static_cast<FigureBasic*>(static_cast<TwoDimFigureData*>(figureData)->chart);
        }
        else if (figureType == FigureType::ThreeDimCurveFigure) {
            figureCommon = static_cast<FigureBasic*>(static_cast<ThreeDimFigureData*>(figureData)->chart);
        }
        return figureCommon;
    }
    int getFigureDim(FigureType figureType) {
        int dim;
        if (figureType == FigureType::TwoDimCurveFigure) {
            dim = 2;
        }
        else if (figureType == FigureType::ThreeDimCurveFigure) {
            dim = 3;
        }
        return dim;
    }

    void resizeItemElements(int pos, int index) {//ע���п��ܻᴥ������ѭ�����á�������޸ġ�
        QSplitter* senderSplitter = (QSplitter*)sender();
        for (int i = 0; i < listWidget->count(); i++) {
            auto itemWidget = listWidget->itemWidget(listWidget->item(i));
            QSplitter* pHSplitter = itemWidget->findChild<QSplitter*>("splitter");
            if(senderSplitter!= pHSplitter)
                pHSplitter->setSizes(senderSplitter->sizes());
        }
    }
    void openSetFigureDialog(BasicFigureData* bFigureData, FigureBasic* figure) {
        MyDialog dialog;
        dialog.setContentAndButton(this->figureConfigWidget, "����ͼ��", "ȷ��", "ȡ��");
        dialog.resize(500, 500);
        this->figureConfigWidget->loadContentFromFigure(bFigureData, figure);
        this->figureConfigWidget->show();
        connect(dialog.createButton, &QPushButton::clicked, &dialog, &QDialog::accept);
        connect(dialog.createButton, &QPushButton::clicked, &dialog, [=]() {
            //����ͼ��ľ���ֵ��
            //����itemʱֻչʾ�������ݣ���չʾ�������ݡ�
            bFigureData->figureName = figureConfigWidget->findChild<QLineEdit*>("nameEditor")->text();
            //bFigureData->isRealTime = figureConfigWidget->findChild<QCheckBox*>("isRealTime")->isChecked();
            bFigureData->paintType = static_cast<PaintType>(figureConfigWidget->findChild<QComboBox*>("paintTypeBox")->currentIndex());
            //����������
            FigureBasic* commonChart = getFigureCommon(bFigureData);
            commonChart->setFigureTitle(bFigureData->figureName);
            int dim = getFigureDim(bFigureData->figureType);
            for (int i = 0; i < dim; i++) {
                auto axisWidget = figureConfigWidget->findChild<QWidget*>("axis" + QString::number(i));
                commonChart->setAxisAutoRange(i, axisWidget->findChild<QCheckBox*>("isAutoRange")->isChecked());
                if (!commonChart->isAxisAutoRange(i)) {
                    float upper = axisWidget->findChild<QLineEdit*>("upperRange")->text().toFloat();
                    float lower = axisWidget->findChild<QLineEdit*>("lowerRange")->text().toFloat();
                    if (upper < lower) {
                        QMessageBox msgBox;//����Ӧ�÷��ڲ��������֮ǰ��Ʃ��������ڳ�ʼ��config֮����ôÿ�ξ��淵�ض������´���config������ڴ�й¶��
                        msgBox.setWindowTitle("����");
                        msgBox.setText(axisText[i] + "�ķ�Χ���ò������뱣֤��Χ�½�С�ڷ�Χ�Ͻ磡");

                        QPushButton* yesButton = msgBox.addButton("ȷ��", QMessageBox::YesRole);
                        msgBox.exec();
                        return;
                    }
                    commonChart->setAxisRange(i, axisWidget->findChild<QLineEdit*>("lowerRange")->text().toDouble(),
                        axisWidget->findChild<QLineEdit*>("upperRange")->text().toDouble());
                }
                commonChart->setAxisAutoTick(i, axisWidget->findChild<QCheckBox*>("isAutoTickSpace")->isChecked());
                if (!commonChart->isAxisAutoTick(i)) {
                    commonChart->setAxisTickType(i, static_cast<TickType>(axisWidget->findChild<QComboBox*>("tickType")->currentIndex()));
                    if (commonChart->getAxisTickType(i) == TickType::TickSpace) {
                        commonChart->setAxisTickSpacing(i, axisWidget->findChild<QLineEdit*>("tickSpace")->text().toDouble());
                    }
                    else if (commonChart->getAxisTickType(i) == TickType::TickNum) {
                        commonChart->setAxisTickSpaceNum(i, axisWidget->findChild<QLineEdit*>("tickSpace")->text().toInt());
                    }
                }
                commonChart->setAxisLabel(i, axisWidget->findChild<QLineEdit*>("axisLabel")->text());
            }
            for (int i = 0; i < dim; i++) {
                if (!commonChart->isAxisAutoTick(i) && !commonChart->isAxisAutoRange(i) &&
                    commonChart->getAxisTickType(i) == TickType::TickSpace &&
                    (commonChart->getAxisRange(i)[1] - commonChart->getAxisRange(i)[0]) / commonChart->getAxisTickSpacing(i) > 20) {
                    QMessageBox msgBox;//����Ӧ�÷��ڲ��������֮ǰ��Ʃ��������ڳ�ʼ��config֮����ôÿ�ξ��淵�ض������´���config������ڴ�й¶��
                    msgBox.setWindowTitle("����");
                    msgBox.setText(axisText[i] + "�Ŀ̶ȼ�����ò�������ǰ�����£��̶��������ᳬ��20�����Ӱ��ͼ�ε���ʾЧ������ɿ��٣�");

                    QPushButton* yesButton = msgBox.addButton("ȷ��", QMessageBox::YesRole);
                    msgBox.exec();
                    return;
                }
                if (!commonChart->isAxisAutoTick(i) && commonChart->isAxisAutoRange(i) &&
                    commonChart->getAxisTickType(i) == TickType::TickSpace) {
                    QMessageBox msgBox;
                    msgBox.setWindowTitle("��ʾ");
                    msgBox.setText("ע�⣬��ֻ�����Զ��᷶Χ���̶���̶ȼ��ʱ�����ʵ�ʻ��Ƶ���̶���������20��Ϊ�˱�֤��ͼЧ���������Զ������µ���̶ȼ����");

                    QPushButton* yesButton = msgBox.addButton("ȷ��", QMessageBox::YesRole);
                    msgBox.exec();
                    break;
                }
            }
            //�޸Ļ�����Ϣ
            for (auto it = figureDataMap.begin(); it != figureDataMap.end(); it++) {
                if (it->second == bFigureData) {
                    auto itemWidget = it->first;
                    auto nameLabel = itemWidget->findChild<QLabel*>("figureName");
                    auto isRealTime = itemWidget->findChild<QCheckBox*>("figureRealTime");
                    auto figureTypeLable = itemWidget->findChild<QLabel*>("figureType");
                    nameLabel->setText(bFigureData->figureName);
                    //isRealTime->setChecked(bFigureData->isRealTime);
                    if (bFigureData->paintType == PaintType::LoadData) {
                        isRealTime->setEnabled(false);
                    }
                    else if (bFigureData->paintType == PaintType::RealTime) {
                        isRealTime->setEnabled(true);
                        isRealTime->setChecked(true);
                    }
                    else if (bFigureData->paintType == PaintType::RunningData) {
                        isRealTime->setEnabled(true);
                        isRealTime->setChecked(false);
                    }
                    figureTypeLable->setText(figureTypeToText.at(bFigureData->figureType));
                    break;
                }
            }
            
            });
        connect(dialog.cancelButton, &QPushButton::clicked, &dialog, &QDialog::accept);
        connect(&dialog, &MyDialog::closedRespond, &dialog, &QDialog::accept);
        dialog.exec();
        this->figureConfigWidget->setParent(this);
        this->figureConfigWidget->hide();
    }
    void openSetCurveDialog(BasicFigureData* bFigureData, FigureBasic* figure) {
        MyDialog dialog;
        dialog.setContentAndButton(this->curveConfigWidget, "��������");
        dialog.resize(500, 500);
        this->curveConfigWidget->loadContentFromFigure(bFigureData, figure);
        this->curveConfigWidget->show();
        connect(&dialog, &MyDialog::closedRespond, &dialog, &QDialog::accept);
        dialog.exec();
        this->curveConfigWidget->setParent(this);
        this->curveConfigWidget->hide();
    }
};