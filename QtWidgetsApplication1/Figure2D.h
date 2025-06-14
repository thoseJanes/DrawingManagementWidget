#pragma once
#include <QWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QtWidgets>
#include <QtCharts/qchartview.h>
#include <QLineSeries>
#include <QValueAxis>

#include "Interval.h"
#include "figureInfo.h"
#include "MyDialog.h"
#include "AddCurveWidget.h"
#include "FigureBasic.h"
#include "array"
QT_BEGIN_NAMESPACE

class Figure2D:public QObject,public FigureBasic{
public:
    QChartView* chartView;//ע�⣡���û�и���������Ҫ�ֶ��ͷ��ڴ档
    struct AxisInfo {
        QValueAxis* axis = new QValueAxis;
        bool autoRange = true;
        bool autoTick = true;
        TickType tickType = TickType::TickSpace;
        double tickSpacing;
        double tickSpaceNum;

        ~AxisInfo() {
            delete axis;
        }
    };
    std::vector<QLineSeries*> linesSeries = {};//����ÿ��������һ����
    std::vector<Interval*> range = {};//����ÿ��������һ����
    std::vector<AxisInfo*> axis_list = {};
    Figure2D() :FigureBasic(2){
        this->linesSeries = {};
        this->range = {};//����ÿ�����߶���һ��range��Ϊ�˷�ֹɾ������ʱrange�仯��
        this->axis_list = {};

        this->chartView = new QChartView();
        this->axis_list.push_back(new AxisInfo);
        this->axis_list.push_back(new AxisInfo);

        this->chartView->chart()->legend()->setAlignment(Qt::AlignBottom);
        this->chartView->chart()->addAxis(this->axis_list[0]->axis, Qt::AlignBottom);
        this->chartView->chart()->addAxis(this->axis_list[1]->axis, Qt::AlignLeft);
    }
    ~Figure2D() {
        this->chartView->deleteLater();
        for (int i = 0; i < this->linesSeries.size(); i++) {
            this->linesSeries[i]->deleteLater();
            delete this->range[i];
        }
        for (int i = 0; i < this->axis_list.size(); i++) {
            this->axis_list[i]->axis->deleteLater();
            delete this->axis_list[i];
        }
    }
    
    void setFigureTitle(QString string) override {
        this->chartView->setWindowTitle(string);
    };
    void clearCurveData() {
        for (int index = 0; index < this->linesSeries.size(); index++) {
            this->linesSeries[index]->clear();
        }
        for (int i = 0; i < range.size(); i++) {
            range[i]->clear();
        }
        //������Range
        for (int i = 0; i < this->axis_list.size(); i++) {
           this->axis_list[i]->axis->setRange(this->axis_list[i]->axis->min(), this->axis_list[i]->axis->max());
        }
        //������̶�
        for (int i = 0; i < this->axis_list.size(); i++) {
            AxisInfo* axis_i = this->axis_list[i];
            if (!axis_i->autoTick) {
                if (axis_i->tickType == TickType::TickSpace) {
                    if ((axis_i->axis->max() - axis_i->axis->min()) / axis_i->tickSpacing < 20) {
                        axis_i->axis->setTickType(QValueAxis::TicksDynamic);
                        axis_i->axis->setTickInterval(axis_i->tickSpacing);
                        axis_i->axis->setTickAnchor(this->axis_list[i]->axis->min());
                    }
                    else {
                        axis_i->axis->setTickType(QValueAxis::TicksFixed);
                        axis_i->axis->setTickCount(20);
                    }
                }
                else if (axis_i->tickType == TickType::TickNum) {
                    axis_i->axis->setTickType(QValueAxis::TicksFixed);
                    axis_i->axis->setTickCount(axis_i->tickSpaceNum);
                }
            }
        }
    }

    void addCurve(QString curveName, QColor color, float width = 1.0, std::vector<DataType> dataTypes = {}) override {
        //������
        QLineSeries* lineSeries = new QLineSeries;
        this->chartView->chart()->addSeries(lineSeries);
        this->linesSeries.push_back(lineSeries);
        this->range.push_back(new Interval(2));
        lineSeries->setName(curveName);
        //��ɫ
        QPen pen = lineSeries->pen();
        pen.setWidth(width);
        pen.setColor(color);
        lineSeries->setPen(pen);
        //���ӵ���
        lineSeries->attachAxis(this->axis_list[0]->axis);
        lineSeries->attachAxis(this->axis_list[1]->axis);
        lineSeries->setPointsVisible(true);
        if (dataTypes.empty()) {
            dataTypes = { {DataObject::Unknow, DataQuantity::Null}, {DataObject::Unknow, DataQuantity::Null}, {DataObject::Unknow, DataQuantity::Null} };
        }
        this->lineDataType.push_back(dataTypes);
    }
    void setCurve(int index, QString curveName, QColor color, float width = 1.0) override {
        //������
        this->linesSeries[index]->setName(curveName);
        //��ɫ
        QPen pen = this->linesSeries[index]->pen();
        pen.setWidth(width);
        pen.setColor(color);
        this->linesSeries[index]->setPen(pen);
        this->linesSeries[index]->setPointsVisible(true);
    }
    QString getCurveName(int i) override {
        if (i < linesSeries.size()) {
            return linesSeries[i]->name();
        }
        return "";
    }
    double getCurveWidth(int i) override {
        if (i < linesSeries.size()) {
            return linesSeries[i]->pen().width();
        }
        return 1.0;
    }
    int getCurveNum() override {
        return this->linesSeries.size();
    }
    void setCurveDataType(int index, std::vector<DataType> dataTypes) override {
        assert(dataTypes.size() == 2);
        this->lineDataType[index] = dataTypes;
    }
    void deleteCurve(int index) override {
        QAbstractSeries* series = this->linesSeries[index];
        this->chartView->chart()->removeSeries(series);
        series->deleteLater();
        this->linesSeries.erase(this->linesSeries.begin() + index);
        this->range.erase(this->range.begin() + index);
    }

    bool isAxisAutoTick(int index) override {
        return this->axis_list[index]->autoTick;
    };
    bool isAxisAutoRange(int index) override {
        return this->axis_list[index]->autoRange;
    };
    std::array<double, 2> getAxisRange(int index) override {
        return { {this->axis_list[index]->axis->min(), this->axis_list[index]->axis->max()} };
    }
    TickType getAxisTickType(int index) override {
        return this->axis_list[index]->tickType;
    };
    int getAxisTickSpaceNum(int index) override {
        return this->axis_list[index]->tickSpaceNum;
    };
    double getAxisTickSpacing(int index) override {
        return this->axis_list[index]->tickSpacing;
    };
    QString getAxisLabel(int index) override {
        return this->axis_list[index]->axis->titleText();
    };

    void setAxisAutoTick(int index, bool flag) override {
        this->axis_list[index]->autoTick = flag;
    }
    void setAxisAutoRange(int index, bool flag) override {
        this->axis_list[index]->autoRange = flag;
    }
    void setAxisRange(int index, double lower, double upper) override {
        this->axis_list[index]->axis->setRange(lower, upper);
    }
    void setAxisTickType(int index, TickType tickType) override {
        this->axis_list[index]->tickType = tickType;
    };
    void setAxisTickSpaceNum(int index, int num) override {
        this->axis_list[index]->tickSpaceNum = num;
    };
    void setAxisTickSpacing(int index, float tickSpace) override {
        this->axis_list[index]->tickSpacing = tickSpace;
    }
    void setAxisLabel(int index, QString label) override {
        this->axis_list[index]->axis->setTitleText(label);
    };

    std::vector<DataType> getCurveDataType(int index) {
        return this->lineDataType[index];
    }
    QColor getCurveColor(int index) {
        if (index < linesSeries.size()) {
            return linesSeries[index]->pen().color();
        }
        return QColor(0,0,0);
    }

    void setAxisColor(int index, QColor color) {
        axis_list[index]->axis->setLinePenColor(color);
    }

    void freshAxisRange(int index) override {
        //�Զ���Χ
        qDebug() << "sd";
        Interval* total = new Interval(2);
        //�ϲ����߷�Χ
        for (int i = 0; i < range.size(); i++) {
            total->merge(range[i]);
        }//���������ߵķ�Χ�ϲ���
        //������̶�
        for (int i = 0; i < this->axis_list.size(); i++) {
            AxisInfo* axis_i = this->axis_list[i];
            qDebug() << "setTickType TicksFixed begin";
        }
        //������Range
        for (int i = 0; i < range[index]->dim; i++) {
            if (this->axis_list[i]->autoRange) {
                double lower = total->lower[i];
                double upper = total->upper[i];
                if (total->lower[i] == total->upper[i]) {
                    if (total->lower[i] == 0) {
                        lower = -1;
                        upper = 1;
                    }
                    else {
                        lower = lower / 2;
                    }
                }
                this->axis_list[i]->axis->setRange(lower, upper);
                qDebug() << lower << ":" << upper;
            }
            else {
                this->axis_list[i]->axis->setRange(this->axis_list[i]->axis->min(), this->axis_list[i]->axis->max());
            }
        }
        //������̶�
        for (int i = 0; i < this->axis_list.size(); i++) {
            AxisInfo* axis_i = this->axis_list[i];
            qDebug() << "autospace begin";
            if (!axis_i->autoTick) {
                if (axis_i->tickType == TickType::TickSpace) {
                    if ((axis_i->axis->max() - axis_i->axis->min()) / axis_i->tickSpacing < 20) {
                        axis_i->axis->setTickType(QValueAxis::TicksDynamic);
                        axis_i->axis->setTickInterval(axis_i->tickSpacing);
                        axis_i->axis->setTickAnchor(this->axis_list[i]->axis->min());
                    }
                    else {
                        axis_i->axis->setTickType(QValueAxis::TicksFixed);
                        axis_i->axis->setTickCount(20);
                    }
                }
                else if (axis_i->tickType == TickType::TickNum) {
                    axis_i->axis->setTickType(QValueAxis::TicksFixed);
                    axis_i->axis->setTickCount(axis_i->tickSpaceNum);
                }
                qDebug() << "autospace over";
            }
            else {
                //axis_i->axis->setTickType(QValueAxis::TicksDynamic);
            }
        }

    }
    //void updateData(int index, double* newData) {
    //    
    //}
    void addCurveData(int index, std::vector<double> data, bool freshAxis = true) override {
        assert(data.size() == 2);
        double newData[2] = { data[0], data[1] };
        /*updateData(index, newData);*/
        this->linesSeries[index]->append(
            newData[0], newData[1]
        );
        qDebug() << "update figure2d";

        range[index]->merge(newData);
        //�Զ���Χ
        if (freshAxis) {
            this->freshAxisRange(index);
        }
        
    }
    void addCurveDataList(int index, std::vector<std::vector<double>> data) override {
        for (auto point : data) {
            assert(point.size() == 2);
            this->linesSeries[index]->append(point[0], point[1]);
            range[index]->merge(point);
        }
        freshAxisRange(index);
    }
    void openSetCurveWidget(int index) override {
        auto curveSetWidget = new AddCurveWidget();
        MyDialog dialog; QPushButton* createButton; QPushButton* cancelButton;
        curveSetWidget->setContent(this->getCurveName(index),
            this->getCurveWidth(index),
            this->getCurveColor(index),
            { this->lineDataType[index][0], this->lineDataType[index][1] });
        dialog.setContentAndButton(curveSetWidget,
            "�޸�����", "ȷ��", "ȡ��");
        dialog.resize(defaultAddCurveDialogWidth, defaultAddCurveDialogHeight);
        createButton = dialog.createButton; cancelButton = dialog.cancelButton;
        //��ť�¼���Ӧ�������˳���ʽ��
        QObject::connect(dialog.createButton, &QPushButton::clicked, &dialog, &QDialog::accept);
        QObject::connect(dialog.createButton, &QPushButton::clicked, &dialog, [=]() {
            QColor qcolor(curveSetWidget->colorSelectButton->text());
            this->setCurve(index, curveSetWidget->nameEditor->text()
                , qcolor, curveSetWidget->widthEditor->text().toDouble());
            this->lineDataType[index] = { {
                curveSetWidget->getDataType(0),
                curveSetWidget->getDataType(1)
            } };
            });
        QObject::connect(dialog.cancelButton, &QPushButton::clicked, &dialog, &QDialog::accept);
        QObject::connect(dialog.cancelButton, &QPushButton::clicked, curveSetWidget, &QWidget::deleteLater);
        QObject::connect(&dialog, &MyDialog::closedRespond, &dialog, &QDialog::accept);
        QObject::connect(&dialog, &MyDialog::closedRespond, curveSetWidget, &QWidget::deleteLater);
        dialog.exec();
    };

};