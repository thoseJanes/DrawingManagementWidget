#pragma once
#include <QtWidgets/QtWidgets>
#include "figureInfo.h"
#include "ItemWidget.h"
class AddFigureWidget :public QWidget {
    Q_OBJECT
signals:
    void figureCreated();
    void closedRespond();
public:
    QComboBox* figureTypeBox;
    QLineEdit* nameEditor;
    QCheckBox* isAutoRange;
    QListWidget* curveListWidget;
    QCheckBox* paintRealTime;
    QWidget* addCurveWidget;
    QPushButton* addCurveButton;
    QPushButton* deleteCurveButton;
    QTabWidget* axisTab;
    std::vector<QWidget*> axisWidgetVec;
    QComboBox* paintTypeBox;

    AddFigureWidget(QWidget* parent = nullptr) :QWidget(parent) {
        initUi();
        this->resize(500, 400);
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
    ~AddFigureWidget() {
        figureTypeBox->deleteLater();
        nameEditor->deleteLater();
        curveListWidget->deleteLater();
        paintRealTime->deleteLater();
        addCurveWidget->deleteLater();
        addCurveButton->deleteLater();
        deleteCurveButton->deleteLater();
        for (auto it = axisWidgetVec.begin(); it != axisWidgetVec.end(); it++) {
            (*it)->deleteLater();
        }
    };

    void initUi() {
        int labelStrech = 1;
        int inputStrech = 2;

        auto vLayout = new QVBoxLayout(this);

        auto hLabelLayout = new QHBoxLayout(this);
        auto nameLabel = new QLabel("图名");
        nameEditor = new QLineEdit(this);
        hLabelLayout->addWidget(nameLabel, 1);
        hLabelLayout->addWidget(nameEditor, inputStrech);
        vLayout->addLayout(hLabelLayout);

        auto hfigureTypeLayout = new QHBoxLayout(this);
        auto figureTypeLabel = new QLabel("绘图类型");
        figureTypeBox = new QComboBox(this);
        figureTypeBox->setEditable(false);
        hfigureTypeLayout->addWidget(figureTypeLabel, 1);
        hfigureTypeLayout->addWidget(figureTypeBox, inputStrech);
        vLayout->addLayout(hfigureTypeLayout);

        axisTab = new QTabWidget(this);
        QStringList axisInitName = { "x轴", "y轴", "z轴" };
        for (int i = 0; i < 3; i++) {
            auto axisWidget = new QWidget();
            auto axisLayout = new QVBoxLayout(axisWidget);
            axisWidget->setLayout(axisLayout);
            axisWidgetVec.push_back(axisWidget);
            
            auto axisLabelLayout = new QHBoxLayout(axisWidget);
            auto axisLabelLabel = new QLabel(axisInitName[i]+ "标签", axisWidget);
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
            setRangeEditorLayout->addWidget(aSRLabel, labelStrech*4);
            setRangeEditorLayout->addWidget(aSRELowerBound, inputStrech*2-1);
            setRangeEditorLayout->addWidget(medLabel, 2);
            setRangeEditorLayout->addWidget(aSREUpperBound, inputStrech*2-1);
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
            auto autoSetAxisTickSpaceLabel = new QLabel("自动刻度", axisWidget);
            
            auto autoSetAxisTickSpaceBox = new QCheckBox(axisWidget);
            autoSetAxisTickSpaceBox->setObjectName("isAutoTickSpace");
            autoSetAxisTickSpaceBox->setChecked(true);
            autoSetTickSpaceBoxLayout->addWidget(autoSetAxisTickSpaceLabel, labelStrech);
            autoSetTickSpaceBoxLayout->addWidget(autoSetAxisTickSpaceBox, inputStrech);
            axisLayout->addLayout(autoSetTickSpaceBoxLayout);

            auto setTickSpaceEditorLayout = new QHBoxLayout(axisWidget);
            //auto aSTSLabel = new QLabel("    刻度间隔", axisWidget);
            auto aSTSLabel = new QComboBox(axisWidget);
            aSTSLabel->setObjectName("tickType");
            auto aSTSEditor = new QLineEdit(axisWidget);
            for (int i = 0; i < tickTypeToText.size(); i++) {
                aSTSLabel->addItem("    " + tickTypeToText[static_cast<TickType>(i)]);
            }
            aSTSLabel->setEditable(false);
            connect(aSTSLabel, QOverload<int>::of(&QComboBox::currentIndexChanged), aSTSEditor, [=](int index) {
                if (static_cast<TickType>(index) == TickType::TickSpace) {
                    aSTSEditor->setValidator(new QRegExpValidator(QRegExp("^(-?\\d+)(\\.\\d+)?(e-?\\d+)?$")));
                }
                else if (static_cast<TickType>(index) == TickType::TickNum) {
                    aSTSEditor->setValidator(new QIntValidator(2, 20, aSTSEditor));
                }
            });
            aSTSEditor->setValidator(new QRegExpValidator(QRegExp("^(-?\\d+)(\\.\\d+)?(e-?\\d+)?$")));
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
        }
        axisTab->addTab(axisWidgetVec[0], axisInitName[0] + "设置");
        axisTab->addTab(axisWidgetVec[1], axisInitName[1] + "设置");
        //axisTab->widget(2)->hide();
        vLayout->addWidget(axisTab);

        auto curveLayout = new QVBoxLayout(this);
        auto curveButtonLayout = new QHBoxLayout(this);
        curveLayout->addLayout(curveButtonLayout);
        addCurveButton = new QPushButton(this);
        deleteCurveButton = new QPushButton(this);
        addCurveButton->setText("添加曲线");
        deleteCurveButton->setText("删除曲线");
        curveButtonLayout->addWidget(addCurveButton);
        curveButtonLayout->addWidget(deleteCurveButton);
        curveLayout->addLayout(curveButtonLayout);

        curveListWidget = new QListWidget(this);
        curveLayout->addWidget(curveListWidget);

        auto curveGroupBox = new QGroupBox(this);
        curveGroupBox->setTitle("绘制曲线设置");
        curveGroupBox->setLayout(curveLayout);
        vLayout->addWidget(curveGroupBox, 1);

        //paintRealTime = new QCheckBox(this);
        //paintRealTime->setText("是否实时绘图");
        //vLayout->addWidget(paintRealTime);
        paintTypeBox = new QComboBox(this);
        for (int i = 0; i < 3; i++) {
            paintTypeBox->addItem(paintTypeToText.at(static_cast<PaintType>(i)));
        }
        vLayout->addWidget(paintTypeBox);
    }
    QListWidgetItem* addCurveItem(QString text, QColor qcolor) {
        //创建item
        QListWidgetItem* item = new QListWidgetItem(this->curveListWidget);
        this->curveListWidget->setCurrentItem(item);
        QLabel* itemLabel = new QLabel();
        
        ItemWidget* itemWidget = new ItemWidget();
        QHBoxLayout* itemLayout = new QHBoxLayout(itemWidget);
        itemWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        itemWidget->setLayout(itemLayout);
        itemLayout->addWidget(itemLabel);
        itemLabel->setText(text);
        itemLabel->setStyleSheet("color:" + qcolor.name());
        itemLabel->setObjectName("curveItemName");
        itemLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
        item->setSizeHint(itemWidget->sizeHint());
        this->curveListWidget->setItemWidget(item, itemWidget);
        return item;
    }
    //void initAddCurveUi() {
    //    addCurveWidget = new QWidget();
    //    if (this->figureTypeBox->currentText() == "二维曲线图") {
    //        QHBoxLayout* xAxisLayout = new QHBoxLayout(&dialog);
    //        QLabel* xAxisLayout = new QLabel("x轴");
    //        QComboBox* xAxisCheckBox = new QComboBox(&dialog);
    //        xAxisCheckBox->setEditable(false);
    //        for (int i = 0; i < this->dataSetNames.size(); i++) {
    //            xAxisCheckBox->addItem(this->dataSetNames[i]);
    //        }
    //    }
    //}
    //void freshAddFigureWidget(std::vector<FigureType> figureTypeNames = {}, std::vector<DataType> dataSetNames = {}) {
    //    this->figureTypeBox->clear();
    //    for (int i = 0; i < figureTypeNames.size(); i++)
    //        this->figureTypeBox->addItem(figureTypeNames[i]);
    //}

    //void addCurve() {
    //    QDialog dialog;
    //    dialog.setWindowTitle("添加曲线");
    //void closeEvent(QCloseEvent *event) override {
    //    emit closedRespond();
    //    event->ignore();
    //}
    //    QVBoxLayout* vboxLayout = new QVBoxLayout(&dialog);

    //    dialog.exec();
    //    return;
    //}

};
