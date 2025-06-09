#if defined(_MSC_VER) && (_MSC_VER >= 1600)    
# pragma execution_character_set("utf-8")    
#endif

#include <QtWidgets/QApplication>

#include <QtCore/QLibrary> 
#include <QtCore/qtextcodec.h>        //解决中文显示所需的库
#include <QtCore/QThread>
#include <iostream>
#include <windows.h>
#include "FigureWidget.h"
#include "FigureListWidget.h"
#include "GLResource.h"
#include "MyDialog.h"

#include <QTextCodec>
using namespace std;

void loadFileDataToFigures(QString str) {
    printf("%s", str.data());
}


int main(int argc, char* argv[])
{
    QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);//设置完后，改变绘图的父控件不会再
    QApplication a(argc, argv);

    //设置中文字体  
    a.setFont(QFont("Microsoft Yahei", 9));
    //设置中文编码
    #if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
        #if _MSC_VER
            QTextCodec* codec = QTextCodec::codecForName("gbk");
        #else
            QTextCodec* codec = QTextCodec::codecForName("utf-8");
        #endif
            QTextCodec::setCodecForLocale(codec);
            QTextCodec::setCodecForCStrings(codec);
            QTextCodec::setCodecForTr(codec);
    #elif (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
        QTextCodec* codec = QTextCodec::codecForName("utf-8");
        QTextCodec::setCodecForLocale(codec);
    #endif
    

    bearingDataObjects = &cylindricalRollerDataObjects;
    dataObjectToDataQuantity = &cylindricalRollerDataObjectToDataQuantity;
    dataTypeToFileName = &cylindricalRollerDataTypeToFileName;

    QMainWindow* Form = new QMainWindow();
    QDockWidget* figureListDockWidget = new QDockWidget("绘图列表", Form);
    figureListDockWidget->setFeatures(figureListDockWidget->features() & ~QDockWidget::DockWidgetClosable);
    FigureListWidget* figureList = new FigureListWidget(Form, figureListDockWidget);
    figureListDockWidget->setWidget(figureList);
    Form->addDockWidget(Qt::RightDockWidgetArea, figureListDockWidget);
    Form->resize(500, 500);
    Form->show();
    qDebug() << "add figureListDockWidget over";

    QObject::connect(figureList, &FigureListWidget::loadFigureData, Form, [=](BasicFigureData* bfd) {
        auto commonFigure = figureList->getFigureCommon(bfd);
        commonFigure->addCurve("testCurve", QColor::fromRgb(100, 0, 0), 3.0);
        int curveIndex = commonFigure->getCurveNum() - 1;
        for (int i = 0; i < 500; i++) {
            double var = i / 10.0;
            double x = sin(var);
            double y = cos(var);
            double z = var/2;
            if (bfd->figureType == FigureType::ThreeDimCurveFigure) {
                commonFigure->addCurveData(curveIndex, { x, y, z }, false);
            }
            else {
                commonFigure->addCurveData(curveIndex, { x, z }, false);
            }
        }
        commonFigure->freshAxisRange(curveIndex);
        
        });
    //QObject::connect(figureList, &FigureListWidget::loadFigureData, Form, [=](BasicFigureData* bfd) {
    //    QWidget* loadDataSetting = new QWidget();
    //    QVBoxLayout* contentLayout = new QVBoxLayout(loadDataSetting);
    //    loadDataSetting->setLayout(contentLayout);

    //    QLabel* dirLabel = new QLabel(loadDataSetting);
    //    dirLabel->setText("载入数据目录(默认为当前数据保存目录)");
    //    QLineEdit* dirEditor = new QLineEdit(loadDataSetting);
    //    //dirEditor->setPlaceholderText(this->ui.saveDataDirEditor->placeholderText());
    //    //dirEditor->setText(this->ui.saveDataDirEditor->text());
    //    contentLayout->addWidget(dirLabel);
    //    contentLayout->addWidget(dirEditor);

    //    MyDialog dialog;
    //    dialog.setWindowTitle("设置载入数据目录");
    //    dialog.setContentAndButton(loadDataSetting,
    //        "设置载入数据目录", "载入", "取消");
    //    //设置响应函数
    //    QObject::connect(dialog.createButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    //    QObject::connect(dialog.createButton, &QPushButton::clicked, Form, [=]() {
    //        QString dataDirString;
    //        dataDirString = dirEditor->text();
    //        loadFileDataToFigures(dataDirString);
    //        });
    //    QObject::connect(dialog.cancelButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    //    QObject::connect(&dialog, &MyDialog::closedRespond, &dialog, &QDialog::accept);
    //    dialog.exec();
    //    return;
    //});

    return a.exec();
}