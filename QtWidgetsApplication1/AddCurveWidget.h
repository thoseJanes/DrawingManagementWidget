#pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1600)    
# pragma execution_character_set("utf-8")    
#endif

#include <QtWidgets/QtWidgets>
#include "figureInfo.h"
#include "AddFigureWidget.h"
#include "DefaultWidgetSize.h"
class AddCurveWidget :public QWidget {
    Q_OBJECT
signals:
    void curveCreated();
    void closedRespond();
public:
    QLineEdit* nameEditor;
    QLineEdit* widthEditor;
    QPushButton* colorSelectButton;
    bool comboChangeShield = false;
    //AddFigureWidget* addFigureWidget;
    std::vector<QWidget*> axisDataVec;

    AddCurveWidget(QWidget* parent = nullptr) :QWidget(parent) {
        initUi();
        
        /*这部分应该直接在父控件中加。
        auto buttonLayout = new QHBoxLayout(this);
        createButton = new QPushButton(this);
        cancelButton = new QPushButton(this);
        createButton->setText("创建");
        cancelButton->setText("取消");
        buttonLayout->addStretch(1);
        buttonLayout->addWidget(createButton);
        buttonLayout->addStretch(1);
        buttonLayout->addWidget(cancelButton);
        buttonLayout->addStretch(1);
        vLayout->addLayout(buttonLayout);*/
    };
    ~AddCurveWidget() {
        nameEditor->deleteLater();
        colorSelectButton->deleteLater();
        widthEditor->deleteLater();

        for (auto it = axisDataVec.begin(); it != axisDataVec.end(); it++) {
            (*it)->deleteLater();
        }
    };

    void initUi() {
        int labelStrech = 2; int editorStrech = 8;
        QVBoxLayout* addCurveLayout = new QVBoxLayout(this);
        this->setLayout(addCurveLayout);

        auto hLabelLayout = new QHBoxLayout(this);
        auto nameLabel = new QLabel("曲线名");
        nameEditor = new QLineEdit(this);
        //nameEditor->setText("曲线" + QString::number(addFigureWidget->curveListWidget->count() + 1));
        hLabelLayout->addWidget(nameLabel, labelStrech);
        hLabelLayout->addWidget(nameEditor, editorStrech);
        addCurveLayout->addLayout(hLabelLayout);

        auto hWidthLayout = new QHBoxLayout(this);
        auto widthLabel = new QLabel("线粗");
        widthEditor = new QLineEdit(this);
        QDoubleValidator* validator = new QDoubleValidator(0.01, 100.00, 2, widthEditor);
        validator->setNotation(QDoubleValidator::StandardNotation);
        widthEditor->setValidator(validator);
        widthEditor->setText("1.0");
        hWidthLayout->addWidget(widthLabel, labelStrech);
        hWidthLayout->addWidget(widthEditor, editorStrech);
        addCurveLayout->addLayout(hWidthLayout);

        auto hColorLayout = new QHBoxLayout(this);
        auto colorLabel = new QLabel("颜色");
        hColorLayout->addWidget(colorLabel, labelStrech);
        colorSelectButton = new QPushButton(this);
        colorSelectButton->setFixedHeight(21);
        colorSelectButton->setText("#000000");
        colorSelectButton->setStyleSheet("background-color: black;color:white");
        hColorLayout->addWidget(colorSelectButton, editorStrech);
        connect(colorSelectButton, &QPushButton::clicked, this, [=]() {
            QColor color = QColorDialog::getColor(Qt::white, this, "选择颜色");
            if (color.isValid()) {
                QString colorText = color.name();
                QColor complementColor(255 - color.red(), 255 - color.green(), 255 - color.blue());
                colorSelectButton->setStyleSheet("background-color: " + colorText + ";color:" + complementColor.name());
                colorSelectButton->setText(color.name());
            }
            });

        addCurveLayout->addLayout(hColorLayout);

        int axisNum; axisNum = 3;
        for (int i = 0; i < axisNum; i++) {
            QWidget* dataTypeSetting = new QWidget(); dataTypeSetting->setContentsMargins(0, 0, 0, 0);
            QHBoxLayout* xAxisLayout = new QHBoxLayout(dataTypeSetting);
            dataTypeSetting->setLayout(xAxisLayout);
            QLabel* xAxisLabel = new QLabel(axisText[i] + "数据");

            QHBoxLayout* dataTypeSelectLayout = new QHBoxLayout(dataTypeSetting);
            QVBoxLayout* dataTypeSelectLayoutv = new QVBoxLayout(dataTypeSetting);
            QHBoxLayout* dataTypeSelectLayout2 = new QHBoxLayout(dataTypeSetting);
            QComboBox* dataObjectComboBox = new QComboBox(dataTypeSetting);
            QSpinBox* objectNumberBox = new QSpinBox(dataTypeSetting);
            QComboBox* dataQuantityComboBox = new QComboBox(dataTypeSetting);
            QComboBox* dataDimComboBox = new QComboBox(dataTypeSetting);

            dataObjectComboBox->setEditable(false); dataQuantityComboBox->setEditable(false); dataDimComboBox->setEditable(false);
            dataQuantityComboBox->setEnabled(false); dataDimComboBox->setEnabled(false); objectNumberBox->setEnabled(false); objectNumberBox->setValue(1);
            dataQuantityComboBox->setMinimumWidth(0); dataObjectComboBox->setMinimumWidth(0);
            dataDimComboBox->setMinimumWidth(0); objectNumberBox->setMinimumWidth(0);
            objectNumberBox->setFixedWidth(40); dataDimComboBox->setFixedWidth(40);
            dataObjectComboBox->setObjectName("dataObject");
            objectNumberBox->setObjectName("objectNumber");
            dataQuantityComboBox->setObjectName("dataQuantity");
            dataDimComboBox->setObjectName("dataDim");

            dataTypeSelectLayout->addWidget(dataObjectComboBox, 3);
            dataTypeSelectLayout->addWidget(objectNumberBox);
            dataTypeSelectLayout2->addWidget(dataQuantityComboBox, 3);
            dataTypeSelectLayout2->addWidget(dataDimComboBox);

            dataTypeSelectLayoutv->addLayout(dataTypeSelectLayout);
            dataTypeSelectLayoutv->addLayout(dataTypeSelectLayout2);

            for (int j = 0; j < bearingDataObjects->size(); j++) {
                dataObjectComboBox->addItem(dataObjectToText.at(bearingDataObjects->at(j)));
            }
            //dataObject改变的情况
            connect(dataObjectComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), dataQuantityComboBox, [=](int index) {
                if (not comboChangeShield) {
                    comboChangeShield = true;
                    dataObjectChange(bearingDataObjects->at(index),
                        dataQuantityComboBox,
                        objectNumberBox,
                        dataDimComboBox);
                    comboChangeShield = false;
                }
            });
            connect(dataQuantityComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), dataDimComboBox, [=](int index) {
                if (not comboChangeShield) {
                    DataObject dataObject = bearingDataObjects->at(dataObjectComboBox->currentIndex());
                    DataQuantity quantity = dataObjectToDataQuantity->at(dataObject).at(index);
                    dataQuantityChange(dataObject,
                        quantity,
                        dataDimComboBox);
                }
                });
            xAxisLayout->addWidget(xAxisLabel, labelStrech);
            xAxisLayout->addLayout(dataTypeSelectLayoutv, editorStrech);
            addCurveLayout->addWidget(dataTypeSetting);
            axisDataVec.push_back(dataTypeSetting);
        }
        addCurveLayout->addSpacing(1);
    }
    void setBasicContent(int curve_id, FigureType type) {
        nameEditor->setText("曲线" + QString::number(curve_id));
        if (type == FigureType::TwoDimCurveFigure) {
            this->axisDataVec[2]->hide();
        }
        else {
            this->axisDataVec[2]->show();
        }
    }
    void setBasicContent(int curve_id, int axisDim) {
        nameEditor->setText("曲线" + QString::number(curve_id));
        if (axisDim == 2) {
            this->axisDataVec[2]->hide();
        }
        else if(axisDim==3){
            this->axisDataVec[2]->show();
        }
    }
    void setContent(QString curveName, float lineWidth, QColor color, std::vector<DataType> dataType) {
        this->nameEditor->setText(curveName);
        this->widthEditor->setText(QString::number(lineWidth));
        comboChangeShield = true;
        for (int i = 0; i < axisDataVec.size(); i++) {
            if (dataType.size() > i) {
                QWidget* axisWidget = axisDataVec.at(i);
                axisDataVec.at(i)->show();
                QComboBox* dataObjectComboBox = axisWidget->findChild<QComboBox*>("dataObject");
                QComboBox* dataQuantityComboBox = axisWidget->findChild<QComboBox*>("dataQuantity");
                QComboBox* dataDimComboBox = axisWidget->findChild<QComboBox*>("dataDim");
                QSpinBox* objectNumberBox = axisWidget->findChild<QSpinBox*>("objectNumber");
                auto it = std::find(bearingDataObjects->begin(), bearingDataObjects->end(), dataType.at(i).dataObject);
                assert(it != bearingDataObjects->end());
                if (it != bearingDataObjects->end()) {
                    // 找到元素，计算索引
                    int index = std::distance(bearingDataObjects->begin(), it);
                    dataObjectComboBox->setCurrentIndex(index);
                }
                dataObjectChange(dataType.at(i).dataObject,
                    dataQuantityComboBox,
                    objectNumberBox,
                    dataDimComboBox);
                auto dataQuantities = dataObjectToDataQuantity->at(dataType.at(i).dataObject);
                auto it2 = std::find(dataQuantities.begin(), dataQuantities.end(), dataType.at(i).dataQuantity);
                assert(it2 != dataQuantities.end());
                if (it2 != dataQuantities.end()) {
                    // 找到元素，计算索引
                    int index = std::distance(dataQuantities.begin(), it2);
                    dataQuantityComboBox->setCurrentIndex(index);
                }
                dataQuantityChange(dataType.at(i).dataObject, dataType.at(i).dataQuantity, dataDimComboBox);
                dataDimComboBox->setCurrentIndex(dataType.at(i).dimIndex);
                objectNumberBox->setValue(dataType.at(i).objectIndex+1);
            }
            else {
                axisDataVec.at(i)->hide();
            }
        }
        comboChangeShield = false;
        if (color.isValid()) {
            QString colorText = color.name();
            QColor complementColor(255 - color.red(), 255 - color.green(), 255 - color.blue());
            colorSelectButton->setStyleSheet("background-color: " + colorText + ";color:" + complementColor.name());
            colorSelectButton->setText(color.name());
        }
    }
    void dataObjectChange(DataObject dataObject, QComboBox* quantityBox, QSpinBox* numberBox, QComboBox* dimBox) {
        quantityBox->clear();
        dimBox->clear();
        if (dataObjectEntityNum.find(dataObject) == dataObjectEntityNum.end()) { QMessageBox::warning(nullptr, "Warning", "dataObjectEntityNum找不到object"); }
        auto entityNumber = dataObjectEntityNum[dataObject];
        if (entityNumber == 1) {
            numberBox->setValue(1);
            numberBox->setEnabled(false);
        }
        else if (entityNumber > 1) {
            numberBox->setValue(1);
            numberBox->setRange(1, entityNumber);
            numberBox->setEnabled(true);
        }

        auto dataQuantityIt = dataObjectToDataQuantity->find(dataObject);
        if (dataQuantityIt != dataObjectToDataQuantity->end()) {//存在DataQuantity
            quantityBox->setEnabled(true);
            auto dataQuantity = (*dataQuantityIt).second;
            auto fileNameMap = dataTypeToFileName->at(dataObject);
            for (int j = 0; j < dataQuantity.size(); j++) {
                quantityBox->addItem(fileNameMap[dataQuantity[j]].paramName);
            }
            auto dimInfo = fileNameMap[dataQuantity[quantityBox->currentIndex()]].extra;
            if (dimInfo.size() > 1) {
                for (int k = 0; k < dimInfo.size(); k++) {
                    dimBox->addItem(dimInfo[k]);
                    dimBox->setEnabled(true);
                }
            }
            else {
                dimBox->setEnabled(false);
            }
        }
        else {
            quantityBox->setEnabled(false);
        }
    }
    void dataQuantityChange(DataObject dataObject, DataQuantity dataQuantity, QComboBox* dimBox) {
        dimBox->clear();

        auto fileName = dataTypeToFileName->at(dataObject).at(dataQuantity);
        auto dimInfo = fileName.extra;
        if (dimInfo.size() > 1) {
            for (int k = 0; k < dimInfo.size(); k++) {
                dimBox->addItem(dimInfo[k]);
                dimBox->setEnabled(true);
            }
        }
        else {
            dimBox->setEnabled(false);
        }
    }
    
    DataType getDataType(int index) {
        auto qLayout = axisDataVec[index];
        QComboBox* dataObjectComboBox = qLayout->findChild<QComboBox*>("dataObject");
        QComboBox* dataQuantityComboBox = qLayout->findChild<QComboBox*>("dataQuantity");
        QComboBox* dataDimComboBox = qLayout->findChild<QComboBox*>("dataDim");
        QSpinBox* objectNumberBox = qLayout->findChild<QSpinBox*>("objectNumber");
        DataType dataType;
        dataType.dataObject = bearingDataObjects->at(dataObjectComboBox->currentIndex());
        dataType.objectIndex = objectNumberBox->value() - 1;
        
        if (dataQuantityComboBox->isEnabled()) {
            auto allDataQuantity = dataObjectToDataQuantity->at(dataType.dataObject);
            dataType.dataQuantity = allDataQuantity[dataQuantityComboBox->currentIndex()];
            if (dataDimComboBox->isEnabled()) {
                dataType.dimIndex = dataDimComboBox->currentIndex();
            }
            auto dataQuantityMap = dataTypeToFileName->at(dataType.dataObject);
            auto fileInfo = dataQuantityMap[dataType.dataQuantity];
            dataType.fileName = fileInfo.fileName;

            
            int column = 0;
            if (fileInfo.extra.size() > 0) {
                auto columnInfo = fileInfo.extra;
                if (columnInfo[0].startsWith("column")) {
                    QStringList columnSlist = columnInfo[0].split(":");
                    if (columnSlist.size() == 2) {
                        column = columnSlist[1].toInt();
                    }
                }
            }
            dataType.columnIndex = column+dataType.dimIndex;
        }
        else {
            dataType.dataQuantity = DataQuantity::Null;
        }
        qLayout->deleteLater();
        return dataType;
    }

};
