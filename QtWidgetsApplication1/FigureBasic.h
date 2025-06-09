#pragma once
#include <QWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QtWidgets>
#include <QtCharts/qchartview.h>
#include <QLineSeries>
#include <QValueAxis>
#include "figureInfo.h"

class FigureBasic{

public:
	FigureBasic(int dim) { this->dim = dim; };
	//virtual ~FigureBasic() = 0;
	int dim = 0;
	bool isRunning = false;
	std::vector<std::vector<DataType>> lineDataType = {};
	virtual void setFigureTitle(QString string) = 0;
	virtual void clearCurveData() = 0;
	virtual void addCurveData(int index, std::vector<double> data, bool freshAxis = true) = 0;
	virtual void freshAxisRange(int index) = 0;
	virtual void addCurveDataList(int index, std::vector<std::vector<double>> data) = 0;

	virtual QString getCurveName(int i)=0;
	virtual int getCurveNum() = 0;
	virtual double getCurveWidth(int i)=0;
	virtual QColor getCurveColor(int index) = 0;
	virtual std::vector<DataType> getCurveDataType(int index)=0;

	virtual void addCurve(QString curveName, QColor color, float width = 1.0, std::vector<DataType> dataTypes = {}) = 0;
	virtual void setCurveDataType(int index, std::vector<DataType> dataTypes) = 0;
	virtual void setCurve(int index, QString curveName, QColor color, float width = 1.0)=0;
	virtual void deleteCurve(int index) = 0;

	virtual void openSetCurveWidget(int index) = 0;

	virtual bool isAxisAutoTick(int index) = 0;
	virtual bool isAxisAutoRange(int index) = 0;
	virtual std::array<double, 2> getAxisRange(int index) = 0;
	virtual TickType getAxisTickType(int index) = 0;
	virtual int getAxisTickSpaceNum(int index) = 0;
	virtual double getAxisTickSpacing(int index) = 0;
	virtual QString getAxisLabel(int index) = 0;

	virtual void setAxisAutoTick(int index, bool flag) = 0;
	virtual void setAxisAutoRange(int index, bool flag) = 0;
	virtual void setAxisRange(int index, double lower, double upper) = 0;
	virtual void setAxisTickType(int index, TickType tickType) = 0;
	virtual void setAxisTickSpaceNum(int index, int num) = 0;
	virtual void setAxisTickSpacing(int index, float tickSpacing) = 0;
	virtual void setAxisLabel(int index, QString label) = 0;

};