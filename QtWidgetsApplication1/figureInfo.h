#pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1600)    
# pragma execution_character_set("utf-8")    
#endif
#include <vector>
#include <tuple>
#include <string>
#include <map>
#include "StatusDockWidget.h"

enum class FigureType {
    TwoDimCurveFigure,
    ThreeDimCurveFigure,
};
static std::map<FigureType, QString> figureTypeToText = {
    {FigureType::TwoDimCurveFigure, "二维曲线图"},
    {FigureType::ThreeDimCurveFigure, "三维曲线图"}
};

enum class PaintType {
    RealTime,
    RunningData,
    LoadData,
};
static std::map<PaintType, QString> paintTypeToText = {
    {PaintType::RealTime, "运行时自动绘图"},
    {PaintType::RunningData, "运行结束后自动绘图"},
    {PaintType::LoadData, "手动载入数据"},
};

enum class figureStatus {
    hide,
    show
};
struct BasicFigureData {
    FigureType figureType;
    QString figureName;
    //bool isRealTime;
    PaintType paintType;
    figureStatus status = figureStatus::hide;
    void* figureData;
    StatusDockWidget* dockWindow = nullptr;
};

enum class TickType {
    TickSpace,
    TickNum,
};
static std::map<TickType, QString> tickTypeToText = {
    {TickType::TickSpace, "刻度间隔"},
    {TickType::TickNum, "刻度数量"}
};

enum class StopCondition {
    MaxTime, MaxStep, MaxShaft
};
static std::map< StopCondition, QString> stopConditionToText = {
    {StopCondition::MaxTime, "模拟时间(s)"},
    {StopCondition::MaxStep, "运行步数"},
    {StopCondition::MaxShaft, "内圈旋转圈数"},
};

static const std::vector<QString> axisText = { "x轴" , "y轴" ,"z轴" };

enum class DataObject {
    Unknow,
    Num,
    Time,
    InnerRace,
    OuterRace,
    Roller,
    RollerCage,
    RollerInnerRace,
    RollerOuterRace,
    Cage,
    CageRace
};
enum class DataQuantity {
    Null,

    Deformation,

    TranslationP,
    TranslationV,
    TranslationA,
    RotationP,
    RotationV,
    RotationA,
    ThermalDisp,

    RaceContactTemperature,
    HeatGeneration,
    QBC,
    MBb,
    Delta,
    ContactV,
    ContactQ,
    ContactR,
    ContactPMax,
    ContactAngle,
    RollerInnerContactInnerV,

    PocketFrontQ,
    PocketBackQ,
    PocketQ,

    CentrifugalForce,//离心力
   // InteractionForce,//相互作用力
};
struct DataType {
    DataObject dataObject;
    DataQuantity dataQuantity;
    int dimIndex = 0;//影响列位置，但注意这和列位置不同！
    int objectIndex = 0;//影响文件
    QString fileName;
    int columnIndex = 0;
};
struct DataNameAndFileName {
    //DataObject dataObject;
    //DataQuantity dataQuantity;
    QString paramName;
    QString fileName;
    std::vector<QString> extra;
    //int dimNum;
    //int columnIndex = -1;
    //int objectIndex = -1;
};

enum class BearingType {
    BallRoller,
    CylindricalRoller,
    Null
};

extern const std::map<DataObject, QString> dataObjectToText;

//!!!!!!!!!!!待添加，检查项目：检查文件中的列与该数据中对应的维数是否一致、检查逐次得到的参数与可填入数据是否一致
static std::vector<QString> cartesianCoordinate = { "x", "y", "z" };
static std::vector<QString> patialCartesianCoordinate = { "y", "z" };
static std::vector<QString> cylindricalCoordinate = { "x", "r", "θ" };
static std::vector<QString> angleCoordinate = { "η", "ξ", "λ" };
//static std::vector<QString> rollerNumberVec = { "1", "2", "3","4","5","6","7","8","9","10"};
extern const std::vector<DataObject> cylindricalRollerDataObjects;
extern const std::map<DataObject, std::vector<DataQuantity>> cylindricalRollerDataObjectToDataQuantity;
extern const std::map<DataObject, std::map<DataQuantity, DataNameAndFileName>> cylindricalRollerDataTypeToFileName;

extern const std::vector<DataObject> ballRollerDataObjects;
extern const std::map<DataObject, std::vector<DataQuantity>> ballRollerDataObjectToDataQuantity;
extern const std::map<DataObject, std::map<DataQuantity, DataNameAndFileName>> ballRollerDataTypeToFileName;

//以下是给其它程序设置及调用的
extern const std::vector<DataObject>* bearingDataObjects;
extern const std::map<DataObject, std::vector<DataQuantity>>* dataObjectToDataQuantity;
extern const std::map<DataObject, std::map<DataQuantity, DataNameAndFileName>>*  dataTypeToFileName;
extern BearingType presentBearingType;
extern std::map<DataObject, int> dataObjectEntityNum;

//console_info替换符
extern const std::map<QString, QString> consoleStringSubstitute;

//读取文件时使用的数据结构。
struct FileDataInfo {
    QFile* qfile;
    QTextStream* textStream;
    std::vector<double> data;
};

struct FigureCommonDataInfo {
    int curveIndex;
    int dimIndex;
    int objectIndex;
    int columnIndex;
    QString file;
};




