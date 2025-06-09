#pragma once
#include <QtWidgets/QOpenGLWidget>
#include <QtGui/QOpenGLFunctions_4_5_Compatibility>
#include <QtGui/QKeyEvent>
#include <QtCore/QDebug>
#include <QtCore/QTime>
#include <QtCore/QTimer>
#include <QtGui/QCursor>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include <cmath>
#include <array>
#include FT_FREETYPE_H
#include "GLResource.h"
#include "Shader.h"
#include "Camera.h"
#include "figureInfo.h"
#include "FigureBasic.h"

#include "MyDialog.h"
#include "AddCurveWidget.h"
//OpenGL4.5画三角
//QOpenGLWidget窗口上下文
//QOpenGLFunctions访问OpenGL接口，可以不继承作为成员变量使用
class GLFigure3D
    : public QOpenGLWidget
    , protected QOpenGLFunctions_4_5_Compatibility
    , public FigureBasic
{
    Q_OBJECT
signals:
    void updateCompleted();
public:
    
    explicit GLFigure3D(GLResource* resource, QWidget* parent = nullptr);
    ~GLFigure3D();
    
    void setResource(GLResource* resource);
//private:
    std::string unicodeTff = "";
    std::string asciiTff = "";
    
    GLResource* resource = nullptr;
    bool createResource = false;

    int hasInit = 0;
    //camera的distance会影响绘制？？为什么调成200之后无法绘制出图像？
    // 为什么调成40时绘图满足某种条件之后会卡顿？即使调用了paintGL也没有绘制出图像？
    //为什么调成1之后又正常了？
    //当前绘制坐标轴存在问题。坐标轴的自动更新存在问题。
    Camera* camera = new Camera(3, { 0.0f, 0.0f ,0.0f }, 5.0f, { 0.0f, 0.0f ,1.0f }, { -1.0f, 0.0f ,0.0f }, 225.0f, -45.0f);
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 projection0;
    bool leftButtonPressed = false;
    bool middleButtonPressed = false;
    int lastX;
    int lastY;
    float figureHeight;
    float figureWidth;
    glm::vec2 windowTextScale;
    /*坐标刻度的生成方式：如果自动tick，则计算区间长度，然后除以tick数，得到一个值，
    该值向较大的整值先进行截断，如果无法截断了则四舍五入，方式为差值不超过该值的1/10.*/
    struct Range3D {
        glm::vec3 minimum = { 0.0f, 0.0f, 0.0f };
        glm::vec3 maximum = { 0.0f, 0.0f, 0.0f };
        bool init = false;
    };
    struct DataBlock {
        GLuint dataVbo = 0;
        GLuint dataVao = 0;
        
        DataBlock* nextDataBlock = nullptr;
        DataBlock* prevDataBlock = nullptr;
    };
    struct SimpleDataBlock {
        GLuint dataVbo = 0;
        GLuint dataVao = 0;
        GLuint blockSize = 100;
        GLuint64 dataNum = 0;
    }simpleDataBlock;
    struct DataBlockInfo {
        DataBlock* dataBlock = nullptr;
        GLuint64 dataNum = 0;
        Range3D dataRange;
        GLuint blockSize = 5000;//注意，单block最大点数量必须和几何着色器中规定的一致！！！或者动态规定？
        QString name = "";
        //QOpenGLWidget* context;
        float lineWidth = 1.0f;
        QColor color;
        //glm::vec3 minimum = { 0.0f, 0.0f, 0.0f };
        //glm::vec3 maximum = { 0.0f, 0.0f, 0.0f };
    };
    struct CharacterObject {
        GLuint vao = 0;
        GLuint vbo = 0;
    }* characterObject;
    //GLuint initBlockSize = 1000;

    std::vector<DataBlockInfo*> dataBlockInfoList;


    struct TextInfo {
        std::wstring text;
        glm::vec3 color;
        glm::vec3 pos;
        float scale;
        glm::vec2 range;
        bool rangeInit = false;
        bool selected = false;
    };
    std::vector<TextInfo*> textInfoList;
    GLuint rectVao = 0;
    glm::mat2 windowOffset;
    struct Axis {
        float interval[2] = { -1.0f, 1.0f };//实际上该项不需要修改。
        float mediumValue = 0.0f;
        float propValue = 1.0f;
        bool autoRange = true;
        std::vector<int> offsetSide = { 0, 0, 0 };//从原轴移动到盒子外侧轴的路。
        std::vector<int> offsetCenter = { 0,0,0 };//从原轴移动到盒子背侧中心轴的路。

        std::wstring label = L"轴zz轴";
        glm::vec3 labelColor = { 0.0f,0.0f,0.0f };
        float labelScale = 0.005f;
        glm::vec2 labelOffset = { 0.0f, 0.0f };//第一个值是沿轴距离(归一化)，第二个值是远轴距离。
        //glm::vec4 labelPlane = { 0.0f, 0.0f ,0.0f,0.0f};
        
        //需要从视平面投影到坐标平面。即垂直于视平面与投影平面相交。

        float tickSpacing = 0.4f;
        float tickSpaceNum = 0;
        TickType tickType = TickType::TickSpace;
        float autoTick = true;
        float tickLength = 0.1f;
        float tickValueScale = 0.005f;
        float tickValueLeft = 0.0f;
        int tickDecimals = 1;//小数点最高精度的小数位数，如0.0023,则为4。
        //intervalDecimals用于自定义范围时，约去末尾零，或者防止区间转换数据转换回原始数据时精度损失导致的小数位数增加。
        //应当在设置范围时即确定。
        int intervalDecimals = 1;//数据范围最高精度的小数位数，
        int displayDecimals = 1;//显示数据的小数位数。
        int magnitude = 0;//数量级，为负表示在小数点后。为0表示n.abc的标准格式.不会受tick影响。
        glm::mat4 axisToTick;

        glm::vec3 color = { 0.0f, 0.0f, 0.0f };
        GLuint axisVao = 0;
        GLuint axisVbo = 0;
    }*axis_x, * axis_y, * axis_z;
    Axis* axis_list[3];
    struct SelectedInfo {
        TextInfo* selectedText = nullptr;
        bool isAxisLabel = false;
        int axisLabelId = 0;
        void clearSelectedInfo() {
            if (this->selectedText != nullptr) {
                this->selectedText->selected = false;
                this->selectedText = nullptr;
            }
            this->isAxisLabel = false;
        }
    }selectedInfo;
    struct infoStruct {
        glm::vec2 lastClickPosition = { 0.0,0.0 };
        glm::vec2 figureRectSize = { 0.0,0.0 };
        TextInfo* selectedText = nullptr;
    }info;

    void deleteDataBlockInfo(int index);
    //【】继承QOpenGLWidget后重写这三个虚函数
    //设置OpenGL资源和状态。在第一次调用resizeGL或paintGL之前被调用一次
    //void loadCharacters();
    //void loadUnicodeCharacters(const wchar_t* text);
    //void loadASCIICharacters(const wchar_t* text);
    void initializeGL() override;
    void initializeVAO();
    void initAxis();
    void initCharacters();
    void initDataBlock(DataBlock* data_block, GLuint block_size);
    void initDataBlockVAO(DataBlock* data_block);
    void initRectVao();
    //渲染OpenGL场景，每当需要更新小部件时使用
    void paintGL() override;
    void paintDataBlocks(DataBlockInfo* dataBlockInfo);
    void paintDataBlocksList();
    void paintSimpleData(glm::vec3 p1, glm::vec3 p2, glm::vec3 color);
    void paintAxis();
    void paintTextList();
    void renderText(const wchar_t* text, glm::vec3 pos, glm::vec3 offset, GLfloat scale, glm::vec3 color, glm::mat4 transM);
    void renderTextWithoutWindowScale(const wchar_t* text, glm::vec3 pos, glm::vec3 offset, GLfloat scale, glm::vec3 color, glm::mat4 transM);
    
    //void axisTickAndRangeUpdate();
    int getDecimal(float value);
    float cutNumber(float value, float range, int side);
    void addTextInfo(std::wstring text, glm::vec3 pos, GLfloat scale, glm::vec3 color);

    void setFigureTitle(QString string) override;
    void clearCurveData() override;
    void addCurveData(int index, std::vector<double> data, bool freshAxis = true) override;
    void addCurveDataList(int index, std::vector<std::vector<double>> data) override;
    void freshAxisRange(int index) override;

    std::vector<DataType> getCurveDataType(int index) override;
    void addCurve(QString curveName, QColor color, float width = 1.0, std::vector<DataType> dataTypes = {}) override;
    QString getCurveName(int index) override;
    double getCurveWidth(int index) override;
    void setCurveDataType(int index, std::vector<DataType> dataTypes) override;
    void openSetCurveWidget(int index) override;
    QColor getCurveColor(int index) override;
    void setCurve(int index, QString curveName, QColor color, float width = 1.0) override;
    int getCurveNum() override;
    QString getAxisLabel(int index) override;
    void  deleteCurve(int index) override;


    bool isAxisAutoTick(int index) override;
    bool isAxisAutoRange(int index) override;
    TickType getAxisTickType(int index) override;
    int getAxisTickSpaceNum(int index) override;
    double getAxisTickSpacing(int index) override;
    std::array<double, 2> getAxisRange(int index) override;

     void setAxisAutoTick(int index, bool flag) override;
     void setAxisAutoRange(int index, bool flag) override;
     void setAxisRange(int index, double lower, double upper) override;
     void setAxisTickType(int index, TickType tickType) override;
     void setAxisTickSpaceNum(int index, int num) override;
     void setAxisTickSpacing(int index, float tickSpacing) override;
     void setAxisLabel(int index, QString label) override;

     void setTextInfo(int index, std::wstring text, glm::vec3 pos, GLfloat scale, glm::vec3 color);
    void removeTextInfo(TextInfo* textInfo);
    glm::vec2 getStringSize(std::wstring text, GLfloat scale);
    Range3D getBoxRange(glm::mat4 view, float x_range[2], float y_range[2], float z_range[2]);
    Range3D getDataBlockInfoListRange();
    //设置OpenGL视口、投影等，每当尺寸大小改变时调用
    void resizeGL(int width, int height) override;
    //void keyPressEvent(QKeyEvent* event);
    
    void updateDataBlockInfo(DataBlockInfo* dataBlockInfo, float newData[3]);
    void autoUpdateAxisRangeAndTick(Range3D dataRange);
    void autoUpdateAxisFormat();
    void setAxisFormat(int axyz[3], int pxyz[3], int hand);
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    
private:

    

};