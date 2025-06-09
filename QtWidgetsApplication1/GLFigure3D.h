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
//OpenGL4.5������
//QOpenGLWidget����������
//QOpenGLFunctions����OpenGL�ӿڣ����Բ��̳���Ϊ��Ա����ʹ��
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
    //camera��distance��Ӱ����ƣ���Ϊʲô����200֮���޷����Ƴ�ͼ��
    // Ϊʲô����40ʱ��ͼ����ĳ������֮��Ῠ�٣���ʹ������paintGLҲû�л��Ƴ�ͼ��
    //Ϊʲô����1֮���������ˣ�
    //��ǰ����������������⡣��������Զ����´������⡣
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
    /*����̶ȵ����ɷ�ʽ������Զ�tick����������䳤�ȣ�Ȼ�����tick�����õ�һ��ֵ��
    ��ֵ��ϴ����ֵ�Ƚ��нضϣ�����޷��ض������������룬��ʽΪ��ֵ��������ֵ��1/10.*/
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
        GLuint blockSize = 5000;//ע�⣬��block������������ͼ�����ɫ���й涨��һ�£��������߶�̬�涨��
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
        float interval[2] = { -1.0f, 1.0f };//ʵ���ϸ����Ҫ�޸ġ�
        float mediumValue = 0.0f;
        float propValue = 1.0f;
        bool autoRange = true;
        std::vector<int> offsetSide = { 0, 0, 0 };//��ԭ���ƶ�������������·��
        std::vector<int> offsetCenter = { 0,0,0 };//��ԭ���ƶ������ӱ����������·��

        std::wstring label = L"��zz��";
        glm::vec3 labelColor = { 0.0f,0.0f,0.0f };
        float labelScale = 0.005f;
        glm::vec2 labelOffset = { 0.0f, 0.0f };//��һ��ֵ���������(��һ��)���ڶ���ֵ��Զ����롣
        //glm::vec4 labelPlane = { 0.0f, 0.0f ,0.0f,0.0f};
        
        //��Ҫ����ƽ��ͶӰ������ƽ�档����ֱ����ƽ����ͶӰƽ���ཻ��

        float tickSpacing = 0.4f;
        float tickSpaceNum = 0;
        TickType tickType = TickType::TickSpace;
        float autoTick = true;
        float tickLength = 0.1f;
        float tickValueScale = 0.005f;
        float tickValueLeft = 0.0f;
        int tickDecimals = 1;//С������߾��ȵ�С��λ������0.0023,��Ϊ4��
        //intervalDecimals�����Զ��巶Χʱ��Լȥĩβ�㣬���߷�ֹ����ת������ת����ԭʼ����ʱ������ʧ���µ�С��λ�����ӡ�
        //Ӧ�������÷�Χʱ��ȷ����
        int intervalDecimals = 1;//���ݷ�Χ��߾��ȵ�С��λ����
        int displayDecimals = 1;//��ʾ���ݵ�С��λ����
        int magnitude = 0;//��������Ϊ����ʾ��С�����Ϊ0��ʾn.abc�ı�׼��ʽ.������tickӰ�졣
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
    //�����̳�QOpenGLWidget����д�������麯��
    //����OpenGL��Դ��״̬���ڵ�һ�ε���resizeGL��paintGL֮ǰ������һ��
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
    //��ȾOpenGL������ÿ����Ҫ����С����ʱʹ��
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
    //����OpenGL�ӿڡ�ͶӰ�ȣ�ÿ���ߴ��С�ı�ʱ����
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