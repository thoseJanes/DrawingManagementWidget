#include "GLFigure3D.h"
#include <QDebug>

//double roundToNDecimals(double value, int n) {
//    double factor = std::pow(10, n);
//    return std::round(value * factor) / factor;
//}

GLFigure3D::GLFigure3D(GLResource* resource, QWidget* parent)
    : QOpenGLWidget(parent), FigureBasic(3)
{
    this->resource = resource;
    
    //this->context()->create();
    QSurfaceFormat format;
    format.setSamples(4);  // 设置多重采样数量为4，常用值有2, 4, 8等
    setFormat(format);
    qDebug() << "Samples supported:" << format.samples();
    axis_x = new Axis(); axis_y = new Axis(); axis_z = new Axis();
    axis_list[0] = axis_x; axis_list[1] = axis_y; axis_list[2] = axis_z;

    //作为标题
    this->addTextInfo(QStringLiteral("绘图").toStdWString(), glm::vec3(0.0f, 0.8f, 0.0f), 0.005, glm::vec3(0.0f, 0.0f, 0.0f));
}
GLFigure3D::~GLFigure3D()
{
    for (int i = 0; i < 3; i++) {
        delete axis_list[i];
    }
    //initializeGL在显示时才调用，释放未初始化的会异常
    if (!isValid())
        return;
    //QOpenGLWidget

    //三个虚函数不需要makeCurrent，对应的操作已由框架完成
    //但是释放时需要设置当前上下文
    makeCurrent();
    if (createResource) {
        delete this->resource;
    }
    for (auto it = textInfoList.begin(); it != textInfoList.end(); it++) {
        delete (*it);
    }
    for (auto it = dataBlockInfoList.begin(); it != dataBlockInfoList.end(); it++) {
        DataBlockInfo* dataBlockInfo = *it;
        GLFigure3D::DataBlock* presentDataBlock = dataBlockInfo->dataBlock; GLFigure3D::DataBlock* nextDataBlock;
        while (presentDataBlock != nullptr) {
            nextDataBlock = presentDataBlock->nextDataBlock;
            glDeleteVertexArrays(1, &presentDataBlock->dataVao);
            glDeleteBuffers(1, &presentDataBlock->dataVbo);
            delete presentDataBlock;
            presentDataBlock = nextDataBlock;
        }
        delete dataBlockInfo;
    }
    delete camera;
    glDeleteVertexArrays(1, &(characterObject->vao));
    glDeleteBuffers(1, &(characterObject->vbo));
    delete characterObject;
    this->hasInit = false;
    doneCurrent();
}
void GLFigure3D::deleteDataBlockInfo(int index) {
    GLFigure3D::DataBlockInfo* dbi = this->dataBlockInfoList[index];
    
    GLFigure3D::DataBlock* presentDataBlock = dbi->dataBlock; GLFigure3D::DataBlock* nextDataBlock;
    while (presentDataBlock != nullptr) {
        nextDataBlock = presentDataBlock->nextDataBlock;
        glDeleteVertexArrays(1, &presentDataBlock->dataVao);
        glDeleteBuffers(1, &presentDataBlock->dataVbo);
        delete presentDataBlock;
        presentDataBlock = nextDataBlock;
    }
    delete dbi;
    this->dataBlockInfoList.erase(this->dataBlockInfoList.begin() + index);
}

//设置
void GLFigure3D::setResource(GLResource* resource) {
    //设置渲染器

    if (createResource) {
        delete this->resource;
        this->resource = nullptr;
        createResource = false;
    }
    this->resource = resource;
}
//初始化函数
void GLFigure3D::initializeGL()
{
    
    //注释
    if (true) {
        //【1】2014年8月12日，Khronos发布了OpenGL 4.5标准规范，
//其中ARB_direct_state_access扩展进入核心，
//其允许直接访问和修改OpenGL对象而无需绑定OpenGL对象（bind操作，例如glBindBuffer），
//提高应用程序和中间件的效率。
//【2】glGen*系列函数生成的id，内部并没有初始化那个对象的状态。只有到了glBind*的时候才会初始化。
//而Core/ARB的DSA直接提供了glCreate*系列函数，可以一步到位地建立id和初始化。

//生成顶点数组对象
//void glCreateVertexArrays(GLsizei n, GLuint *arrays);
            //生成缓冲区对象
    //void glCreateBuffers(GLsizei n, GLuint *buffers);
            //分配size个存储单元存储数据或索引
    //glNamedBufferStorage(GLuint buffer, GLsizeiptr size,
    //                     const void *data, GLbitfield flags);
    //参数1缓冲区对象
    //参数2数据块大小
    //参数3如果为NULL则是size个为初始化的数据，否则以data拷贝初始化
    //参数4数据相关的用途
            //vbo绑定到vao
    //glVertexArrayVertexBuffer(GLuint vaobj, GLuint bindingindex,
    //                          GLuint buffer, GLintptr offset, GLsizei stride);
    //参数1顶点数组对象
    //参数2vbo在vao的索引
    //参数3顶点缓冲对象
    //参数4缓冲区第一个元素的偏移
    //参数5缓冲区顶点步进，三角形一个点3个float
            //启用通用顶点 attribute 数组的 index 索引，对应layout location
    //glEnableVertexArrayAttrib(GLuint vaobj, GLuint index);
            //指定顶点数组的组织
    //glVertexArrayAttribFormat(GLuint vaobj, GLuint attribindex,
    //                          GLint size, GLenum type,
    //                          GLboolean normalized, GLuint relativeoffset);
    //参数1顶点数组对象
    //参数2通用顶点 attribute 数组，对应layout location
    //参数3每个顶点几个数据
    //参数4存储类型
    //参数5是否归一化
    //参数6顶点步进

    //关联顶点 attribute 属性和顶点缓冲区的绑定
    //glVertexArrayAttribBinding(GLuint vaobj, GLuint attribindex,
    //                           GLuint bindingindex);
    //参数1顶点数组对象
    //参数2属性 attribute 索引，对应layout location
    //参数3vbo在vao的索引


    }
    initializeOpenGLFunctions();//为当前上下文初始化opengl函数解析
    //if (!hasInit) {
    QSurfaceFormat format;
    format.setSamples(4);  // 设置多重采样数量为4，常用值有2, 4, 8等
    //lifeContext = new QOpenGLContext();
    //lifeContext->setFormat(format);
    //this->context()->setShareContext(lifeContext);
    //lifeContext->setShareContext(this->context());
    //lifeContext->create();
    if (resource==nullptr) {
        resource = new GLResource();
        qDebug() << "sd";
        //resource->axisShader = new Shader("AxisShader.vs", "AxisShader.frag", nullptr);
        //resource->dataShader = new Shader("BasicShader.vs", "BasicShader.frag", nullptr);
        //resource->characterShader = new Shader("CharacterShader.vs", "CharacterShader.frag", nullptr);
        //resource->rectShader = new Shader("RectShader.vs", "RectShader.frag", "RectShader.geo");
        resource->initializeGL();
        createResource = true;
    }
    else {
        
        //this->context()->setShareContext(this->resource->context());
        //this->context()->create();
        //this->resource->context()->create();
    }

    this->hasInit += 1;
    qDebug() << this->hasInit;

    //loadCharacters();
    initAxis();
    initRectVao();
    initCharacters();
    
    glCreateBuffers(1, &(simpleDataBlock.dataVbo));
    glNamedBufferStorage(simpleDataBlock.dataVbo, simpleDataBlock.blockSize * 3 * sizeof(float), nullptr, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
    glCreateVertexArrays(1, &(simpleDataBlock.dataVao));
    glVertexArrayVertexBuffer(simpleDataBlock.dataVao, 0, simpleDataBlock.dataVbo, 0, 3 * sizeof(float));
    glVertexArrayAttribFormat(simpleDataBlock.dataVao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(simpleDataBlock.dataVao, 0);
    glVertexArrayAttribBinding(simpleDataBlock.dataVao, 0, 0);
    //dataBlock->nextDataBlock = new DataBlock();
    //initDataBlock(dataBlock->nextDataBlock, 5);
    glEnable(GL_MULTISAMPLE);  // 确保启用多重采样
    glEnable(GL_DEPTH_TEST);//用于深度遮挡
    glEnable(GL_BLEND);//用于纹理颜色
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    
    
}
void GLFigure3D::initializeVAO() {//切换上下文时需要重新初始化vao；
    //更新axis的vao
    for (int i = 0; i < 3; i++) {
        Axis* axis_i = axis_list[i];
        glCreateVertexArrays(1, &(axis_i->axisVao));
        glVertexArrayVertexBuffer(axis_i->axisVao, 0, axis_i->axisVbo, 0, 3 * sizeof(float));//vbo绑定到vao的0位置，设定vbo的偏移量和步长
        glVertexArrayAttribFormat(axis_i->axisVao, 0, 3, GL_FLOAT, GL_FALSE, 0);//设定vao绑定的着色器属性位置，设定vao的数据大小？
        glEnableVertexArrayAttrib(axis_i->axisVao, 0);
        glVertexArrayAttribBinding(axis_i->axisVao, 0, 0);
    }
    //更新character的vao
    glGenVertexArrays(1, &(characterObject->vao));
    glBindVertexArray(characterObject->vao);
    glBindBuffer(GL_ARRAY_BUFFER, characterObject->vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)(6 * 3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //

}

void GLFigure3D::initAxis() {
    for (int i = 0; i < 3; i++) {
        float axis_line[6] = {
           0.0f,0.0f,0.0f,
           0.0f,0.0f,0.0f
        };
        axis_line[3 + i] = 1.0f;
        Axis* axis_i = axis_list[i];
        axis_i->color[i] = 1.0f;

        glCreateVertexArrays(1, &(axis_i->axisVao));
        glCreateBuffers(1, &(axis_i->axisVbo));
        glNamedBufferStorage(axis_i->axisVbo, sizeof(axis_line), axis_line, GL_DYNAMIC_STORAGE_BIT);//给vbo命名一块内存？注意是在GPU中？
        glVertexArrayVertexBuffer(axis_i->axisVao, 0, axis_i->axisVbo, 0, 3 * sizeof(float));//vbo绑定到vao的0位置，设定vbo的偏移量和步长
        glVertexArrayAttribFormat(axis_i->axisVao, 0, 3, GL_FLOAT, GL_FALSE, 0);//设定vao绑定的着色器属性位置，设定vao的数据大小？
        glEnableVertexArrayAttrib(axis_i->axisVao, 0);
        glVertexArrayAttribBinding(axis_i->axisVao, 0, 0);
    }
}
void GLFigure3D::initCharacters() {

    //创建绘图对象
    characterObject = new CharacterObject();
    glGenBuffers(1, &(characterObject->vbo));

    glGenVertexArrays(1, &(characterObject->vao));
    glBindVertexArray(characterObject->vao);

    glBindBuffer(GL_ARRAY_BUFFER, characterObject->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 5, NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)(6 * 3 * sizeof(GLfloat)));
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);

    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
}
void GLFigure3D::initDataBlock(DataBlock* data_block, GLuint block_size) {
    //创建vbo
    //lifeContext->makeCurrent(nullptr);
    //glCreateBuffers(1, &(data_block->dataVbo)); 
    ////glNamedBufferStorage(data_block->dataVbo, sizeof(data), data, GL_DYNAMIC_STORAGE_BIT);
    //glNamedBufferStorage(data_block->dataVbo, (block_size +1) * 3 * sizeof(float), nullptr, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
    ////glBufferData(data_block->dataVbo, sizeof(data), data, GL_DYNAMIC_DRAW);//10000条数据data_block->blockSize * 3 * sizeof(float)
    //lifeContext->doneCurrent();

    //创建vao
    //this->makeCurrent();
    glCreateBuffers(1, &(data_block->dataVbo));
    //glNamedBufferStorage(data_block->dataVbo, sizeof(data), data, GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferStorage(data_block->dataVbo, (block_size + 1) * 3 * sizeof(float), nullptr, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
    
    glCreateVertexArrays(1, &(data_block->dataVao));
    glVertexArrayVertexBuffer(data_block->dataVao, 0, data_block->dataVbo, 0, 3 * sizeof(float));
    glVertexArrayAttribFormat(data_block->dataVao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(data_block->dataVao, 0);
    glVertexArrayAttribBinding(data_block->dataVao, 0, 0);
}
void GLFigure3D::initDataBlockVAO(DataBlock* data_block) {
    glCreateVertexArrays(1, &(data_block->dataVao));
    glVertexArrayVertexBuffer(data_block->dataVao, 0, data_block->dataVbo, 0, 3 * sizeof(float));
    glVertexArrayAttribFormat(data_block->dataVao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(data_block->dataVao, 0);
    glVertexArrayAttribBinding(data_block->dataVao, 0, 0);
}
void GLFigure3D::initRectVao() {

    float rect[12] = {
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
    };

    GLuint rect_vbo = 0;
    glCreateVertexArrays(1, &rectVao);
    glCreateBuffers(1, &rect_vbo);
    glNamedBufferStorage(rect_vbo, sizeof(rect), rect, GL_DYNAMIC_STORAGE_BIT);//给vbo命名一块内存？注意是在GPU中？
    glVertexArrayVertexBuffer(rectVao, 0, rect_vbo, 0, 3 * sizeof(float));//vbo绑定到vao的0位置，设定vbo的偏移量和步长
    glVertexArrayAttribFormat(rectVao, 0, 3, GL_FLOAT, GL_FALSE, 0);//设定vao绑定的着色器属性位置，设定vao的数据大小？
    glEnableVertexArrayAttrib(rectVao, 0);
    glVertexArrayAttribBinding(rectVao, 0, 0);
}
//重写虚函数
void GLFigure3D::setFigureTitle(QString string) {
    this->setTextInfo(0, string.toStdWString(), glm::vec3(0.0f, 0.8f, 0.0f), 0.005, glm::vec3(0.0f, 0.0f, 0.0f));
}
void GLFigure3D::clearCurveData() {
    for (int index = 0; index < this->dataBlockInfoList.size(); index++) {
        DataBlockInfo* dbi = this->dataBlockInfoList[index];

        DataBlock* presentDataBlock = dbi->dataBlock; DataBlock* nextDataBlock;
        while (presentDataBlock != nullptr) {
            nextDataBlock = presentDataBlock->nextDataBlock;
            glDeleteVertexArrays(1, &presentDataBlock->dataVao);
            glDeleteBuffers(1, &presentDataBlock->dataVbo);
            delete presentDataBlock;
            presentDataBlock = nextDataBlock;
        }
        //delete dbi;
        dbi->dataBlock = nullptr;
        dbi->dataNum = 0;
        dbi->dataRange.init = false;
        this->update();
    }
}

void GLFigure3D::addCurveData(int index, std::vector<double> data, bool freshAxis) {
    assert(data.size() == 3);
    float newData[3] = { (float)data[0], (float)data[1], (float)data[2] };
    updateDataBlockInfo(this->dataBlockInfoList[index], newData);
}
void GLFigure3D::freshAxisRange(int index) {
    //自动皆刷新。
}
void GLFigure3D::addCurveDataList(int index, std::vector<std::vector<double>> data) {
    for (auto point : data) {
        float newData[3] = { (float)point[0], (float)point[1], (float)point[2] };
        updateDataBlockInfo(this->dataBlockInfoList[index], newData);
    }
    
}

std::vector<DataType> GLFigure3D::getCurveDataType(int index) {
    return this->lineDataType[index];
}
void GLFigure3D::addCurve(QString curveName, QColor color, float width, std::vector<DataType> dataTypes)  {
    if (dataTypes.empty()) {
        dataTypes = { {DataObject::Unknow, DataQuantity::Null}, {DataObject::Unknow, DataQuantity::Null}, {DataObject::Unknow, DataQuantity::Null} };
    }
    DataBlockInfo* dataBlockInfo = new GLFigure3D::DataBlockInfo();
    dataBlockInfo->name = curveName;
    dataBlockInfo->color = color;
    dataBlockInfo->lineWidth = width;
    this->dataBlockInfoList.push_back(dataBlockInfo);
    this->lineDataType.push_back(dataTypes);
}
QString GLFigure3D::getCurveName(int index) {
    assert(index < this->dataBlockInfoList.size());
    if (index < this->dataBlockInfoList.size()) {
        return this->dataBlockInfoList[index]->name;
    }
    return "";
};
double GLFigure3D::getCurveWidth(int index) {
    assert(index < this->dataBlockInfoList.size());
    if (index < this->dataBlockInfoList.size()) {
        return this->dataBlockInfoList[index]->lineWidth;
    }
    return 1.0;
};
void GLFigure3D::setCurveDataType(int index, std::vector<DataType> dataTypes) {
    assert(dataTypes.size() == 3);
    this->lineDataType[index] = dataTypes;
}
QColor GLFigure3D::getCurveColor(int index) {
    assert(index < this->dataBlockInfoList.size());
    if (index < this->dataBlockInfoList.size()) {
        return this->dataBlockInfoList[index]->color;
    }
    return QColor(0, 0, 0);
}
void GLFigure3D::setCurve(int index, QString curveName, QColor color, float width) {
    assert(index < this->dataBlockInfoList.size());
    this->dataBlockInfoList[index]->color = color;
    this->dataBlockInfoList[index]->name = curveName;
    this->dataBlockInfoList[index]->lineWidth = width;
}
int GLFigure3D::getCurveNum() {
    return this->dataBlockInfoList.size();
}
void  GLFigure3D::deleteCurve(int index) {
    assert(index < this->dataBlockInfoList.size());
    this->deleteDataBlockInfo(index);
}

std::array<double, 2> GLFigure3D::getAxisRange(int index) {
    Axis* axis_i = axis_list[index];
    
    return { {axis_i->interval[0]*axis_i->propValue+axis_i->mediumValue, axis_i->interval[1] * axis_i->propValue + axis_i->mediumValue}};
}
void GLFigure3D::openSetCurveWidget(int index) {
    assert(index < this->dataBlockInfoList.size());
    auto curveSetWidget = new AddCurveWidget();
    MyDialog dialog; QPushButton* createButton; QPushButton* cancelButton;
    curveSetWidget->setContent(this->getCurveName(index),
        this->getCurveWidth(index),
        this->getCurveColor(index),
        { this->lineDataType[index][0], this->lineDataType[index][1], this->lineDataType[index][2] });
    dialog.setContentAndButton(curveSetWidget,
        "修改曲线", "确定", "取消");
    dialog.resize(defaultAddCurveDialogWidth, defaultAddCurveDialogHeight);
    createButton = dialog.createButton; cancelButton = dialog.cancelButton;
    //按钮事件响应，三种退出方式。
    QObject::connect(dialog.createButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    QObject::connect(dialog.createButton, &QPushButton::clicked, &dialog, [=]() {
        QColor qcolor(curveSetWidget->colorSelectButton->text());
        this->setCurve(index, curveSetWidget->nameEditor->text()
            , qcolor, curveSetWidget->widthEditor->text().toDouble());
        this->lineDataType[index] = { {
            curveSetWidget->getDataType(0),
            curveSetWidget->getDataType(1),
            curveSetWidget->getDataType(2)
        } };
        });
    QObject::connect(dialog.cancelButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    QObject::connect(dialog.cancelButton, &QPushButton::clicked, curveSetWidget, &QWidget::deleteLater);
    QObject::connect(&dialog, &MyDialog::closedRespond, &dialog, &QDialog::accept);
    QObject::connect(&dialog, &MyDialog::closedRespond, curveSetWidget, &QWidget::deleteLater);
    dialog.exec();
}

bool GLFigure3D::isAxisAutoTick(int index)  {
    return this->axis_list[index]->autoTick;
};
bool GLFigure3D::isAxisAutoRange(int index)  {
    return this->axis_list[index]->autoRange;
};
TickType GLFigure3D::getAxisTickType(int index)  {
    return this->axis_list[index]->tickType;
};
int GLFigure3D::getAxisTickSpaceNum(int index)  {
    return this->axis_list[index]->tickSpaceNum;
};
double GLFigure3D::getAxisTickSpacing(int index)  {
    return this->axis_list[index]->tickSpacing;
};
QString GLFigure3D::getAxisLabel(int index) {
    return QString::fromStdWString(this->axis_list[index]->label);
}

void GLFigure3D::setAxisAutoTick(int index, bool flag) {
    this->axis_list[index]->autoTick = flag;
}
void GLFigure3D::setAxisAutoRange(int index, bool flag) {
    this->axis_list[index]->autoRange = flag;
}
void GLFigure3D::setAxisRange(int index, double lower, double upper) {
    if (upper == lower) {
        upper = pow(10, floor(std::log10(upper))) + upper;
        lower = lower - pow(10, floor(std::log10(upper)));
    }
    Axis* axis_i = axis_list[index];
    axis_i->intervalDecimals = std::max(getDecimal(lower), getDecimal(upper));
    axis_i->interval[0] = -1.0f;
    axis_i->interval[1] = 1.0f;
    axis_i->propValue = (upper - lower) / 2;
    axis_i->mediumValue = (upper + lower) / 2;
    double maxabs = std::max(abs(lower), abs(upper));
    axis_i->magnitude = floor(std::log10(maxabs));//计算总体数量级。不会受tick的数量级影响。10^(数量级+1)>=value>=10^(数量级)
}
void GLFigure3D::setAxisTickType(int index, TickType tickType) {
    this->axis_list[index]->tickType = tickType;
};
void GLFigure3D::setAxisTickSpaceNum(int index, int num) {
    this->axis_list[index]->tickSpaceNum = num;
};
void GLFigure3D::setAxisTickSpacing(int index, float tickSpace) {
    Axis* axis_i = axis_list[index];
    axis_i->tickSpacing = tickSpace;
    axis_i->tickDecimals = getDecimal(tickSpace);
}
void GLFigure3D::setAxisLabel(int index, QString label) {
    this->axis_list[index]->label = label.toStdWString();
};

//数据加载函数
void GLFigure3D::updateDataBlockInfo(DataBlockInfo* dataBlockInfo, float newData[3]) {
    dataBlockInfo->dataNum += 1;

    this->makeCurrent();
    //给dataBlock扩容
    int data_Num = dataBlockInfo->dataNum;
    DataBlock* data_block_ptr = dataBlockInfo->dataBlock;
    if (data_Num > 0 && data_block_ptr == nullptr) {
        dataBlockInfo->dataBlock = new DataBlock();
        initDataBlock(dataBlockInfo->dataBlock, dataBlockInfo->blockSize);
    }
    while (data_Num > dataBlockInfo->blockSize) {
        if (data_block_ptr->nextDataBlock == nullptr) {
            data_block_ptr->nextDataBlock = new DataBlock();
            initDataBlock(data_block_ptr->nextDataBlock, dataBlockInfo->blockSize);
            data_block_ptr->nextDataBlock->prevDataBlock = data_block_ptr;
            break;
        }
        data_block_ptr = data_block_ptr->nextDataBlock;
        data_Num -= dataBlockInfo->blockSize;
    }
    
    int left_num = dataBlockInfo->dataNum;
    //qDebug() << dataBlockInfo->dataNum;
    DataBlock* block_ptr = dataBlockInfo->dataBlock;
    while (left_num > dataBlockInfo->blockSize) {
        left_num -= dataBlockInfo->blockSize;
        block_ptr = block_ptr->nextDataBlock;
    }
    GLuint vbo = block_ptr->dataVbo;
    //glBufferSubData(vbo, left_num*3*sizeof(float), 3*sizeof(float), newData);
    void* ptr2 = glMapNamedBufferRange(block_ptr->dataVbo, static_cast<unsigned long long>(left_num) * 3 * sizeof(float), 3 * sizeof(float), GL_MAP_WRITE_BIT);
    if (ptr2) {
        memcpy(ptr2, newData, 3 * sizeof(float));
        glUnmapNamedBuffer(block_ptr->dataVbo);
        if (left_num == 1) {//正好有一个点不够，这种情况下需要复制上一个block的最后一个点到当前block中。注意为了留空blocksize比真实size小1个点
            float last_point_data[3] = { 10.0f, 10.0f, 10.0f };
            DataBlock* prev_ptr = block_ptr->prevDataBlock;
            if (prev_ptr == nullptr) {
                memcpy(last_point_data, newData, 3 * sizeof(float));
            }
            else {
                GLfloat* ptr = (GLfloat*)glMapNamedBufferRange(block_ptr->prevDataBlock->dataVbo, (dataBlockInfo->blockSize) * 3 * sizeof(float), 3 * sizeof(float), GL_MAP_READ_BIT);
                if (ptr) {
                    memcpy(last_point_data, ptr, 3 * sizeof(float));
                    glUnmapNamedBuffer(block_ptr->prevDataBlock->dataVbo);
                }
            }
            
            //glBufferSubData(block_ptr->dataVbo, 0, 3 * sizeof(float), last_point_data);
            void* ptr1 = glMapNamedBufferRange(block_ptr->dataVbo, 0, 3 * sizeof(float), GL_MAP_WRITE_BIT);
            if (ptr1) {
                memcpy(ptr1, last_point_data, 3 * sizeof(float));
                glUnmapNamedBuffer(block_ptr->dataVbo);
            }

        }
    }
    if (!dataBlockInfo->dataRange.init) {
        for (int j = 0; j < 3; j++) {
            dataBlockInfo->dataRange.minimum[j] = newData[j];
            dataBlockInfo->dataRange.maximum[j] = newData[j];
        }
        dataBlockInfo->dataRange.init = true;
    }
    for (int i = 0; i < 3; i++) {//更新坐标轴范围
        if (newData[i] < dataBlockInfo->dataRange.minimum[i]) {
            dataBlockInfo->dataRange.minimum[i] = newData[i];
        }
        if (newData[i] > dataBlockInfo->dataRange.maximum[i]) {
            dataBlockInfo->dataRange.maximum[i] = newData[i];
        }
        //qDebug() << "datarange:" << dataBlockInfo->dataRange.minimum[i] << ":" << dataBlockInfo->dataRange.maximum[i];
    }
    
    this->update();
    this->doneCurrent();//不能直接放进initDataBlock中，因为其它地方也有init，要是init完就done了，后续就执行不了了
}
//自动依据数据范围设置坐标轴范围和刻度，只在实时绘图且自动调整时启用。
void GLFigure3D::autoUpdateAxisRangeAndTick(Range3D dataRange) {
    assert(dataRange.init);

    //首先更新坐标刻度范围，如果轴之间尺度相差太大需要在设置刻度间隔时进行处理。
    for (int i = 0; i < 3; i++) {
        Axis* axis_i = axis_list[i];
        if (axis_i->autoRange) {//依据刻度对区间进行二次处理
            float length = dataRange.maximum[i] - dataRange.minimum[i];
            if (dataRange.minimum[i] == dataRange.maximum[i]) {
                length = 1;
                axis_i->mediumValue = (dataRange.minimum[i] + dataRange.maximum[i]) / 2;
                axis_i->propValue = 1.0f;
                axis_i->interval[0] = -1.0f;
                axis_i->interval[1] = 1.0f;
            }
            else {
                axis_i->mediumValue = (dataRange.minimum[i] + dataRange.maximum[i]) / 2;
                axis_i->propValue = (dataRange.maximum[i] - dataRange.minimum[i]) / 2;
                axis_i->interval[0] = -1.0f;
                axis_i->interval[1] = 1.0f;
            }
        }
        else {
            //注意设置interval的时候就应该尺度变换。
            if (axis_i->interval[0] == axis_i->interval[1]) {
                axis_i->mediumValue = axis_i->interval[1];
                axis_i->propValue = 1.0f;
                axis_i->interval[0] = -1.0f;
                axis_i->interval[1] = 1.0f;
            }
        }
    }
    std::list<double> space_list = { 0.2, 0.3, 0.5, 1, 1.5, 2 ,3, 5 };//目的是刻度数小于10，哪个接近10就用哪个？
    std::vector<double> prop_list = { 0.8, 0.6, 0.4, 0.2, 0.1 };//
    for (int i = 0; i < 3; i++) {
        Axis* axis_i = axis_list[i];
        assert(axis_i->interval[0] != axis_i->interval[1]);//在自己设置interval的情况下，有可能设置成axis_i->interval[0] == axis_i->interval[1]，但可以通过归一化来避免这种情况。
        //如果同时autoInterval和autoTick，则有协同作用：Interval也会根据Tick进行适当变化。
        //如果只有autoInterval，则Interval也会受设置的tick的影响。重点在于获取tick的小数位数。
        //如果只有autoTick，则策略相对简单。端点可能没有刻度？
        //如果都没有，则重点在于获取tick的小数位数？且端点可能没有刻度？否，端点应有刻度。因此需要获取tick和Interval两者的小数位数。从而计算约分的位数。
        //interval的小数位数应该在设置时获取。
        double lower = axis_i->interval[0] * axis_i->propValue + axis_i->mediumValue;
        double upper = axis_i->interval[1] * axis_i->propValue + axis_i->mediumValue;
        double length = upper - lower;
        //动态更新tickSpacing和tickDecimals。如果设置了固定值，那么这两个都是固定的，不需要更新。
        bool forceAutoTick = (length / axis_i->tickSpacing > 20 && axis_i->autoRange);
        if (axis_i->autoTick || forceAutoTick) {
            int length_e; double length_n; double space_selected = 1;
            length_e = floor(std::log10(length));//e为长度区间尺度
            length_n = length / pow(10, length_e);//n为大于1小于等于10的尺度变换后的数
            for (auto it = space_list.begin(); it != space_list.end(); it++) {
                double space_i = *it;
                if (length_n / space_i < 6) {
                    space_selected = space_i;
                    break;
                }
            }
            space_selected = space_selected * pow(10, length_e);
            axis_i->tickSpacing = space_selected;
            axis_i->tickDecimals = -length_e + 1;
        }
        if (axis_i->autoRange) {
            //根据刻度间隔自动调整范围
            if ((axis_i->autoTick || forceAutoTick || axis_i->tickType == TickType::TickSpace)) {//依据刻度对区间进行二次处理
                //qDebug() << "before" << i << ":" << lower << ":" << upper;
                //在一个区间范围内进行起始终止点的截断。
                if (abs(lower) / axis_i->tickSpacing < 1e-5) {//如果下边界绝对值接近0，则使下界为0。
                    lower = 0;
                }
                else {
                    lower = cutNumber(lower, axis_i->tickSpacing, 0);
                }
                double to_num = lower;
                while ((upper - to_num) / axis_i->tickSpacing >= 1e-6) {
                    to_num += axis_i->tickSpacing;
                }
                upper = to_num;
                //qDebug() << "set tickvalue over";
                //qDebug() << axis_i->mediumValue << ":" << axis_i->propValue << ":" << axis_i->tickSpacing;
            }
            //根据刻度数量自动调整范围
            if ((!axis_i->autoTick) && axis_i->tickType == TickType::TickNum) {
                //重点在于将区间划分为特定倍数，然后再将余量分配到起始区间处。
                float tempSpace;
                //这里的tickNum实际是tickSpaceNum
                if (axis_i->tickSpaceNum > 0) {
                    tempSpace = length / axis_i->tickSpaceNum;
                    //截断tempSpace,只能大不能小。
                    tempSpace = cutNumber(tempSpace, length * 0.1 / axis_i->tickSpaceNum, 1);
                    axis_i->tickSpacing = tempSpace;
                    length = tempSpace * axis_i->tickSpaceNum;
                    //约边界约头，尾部直接加。
                    //截断lower
                    lower = cutNumber(lower, length * 0.1, 0);
                    upper = lower + length;
                    axis_i->tickDecimals = getDecimal(tempSpace);
                    axis_i->intervalDecimals = getDecimal(lower);
                }
                else {//刻度数量为0，直接约边界。
                    //截断lower
                    lower = cutNumber(lower, length * 0.1, 0);
                    upper = cutNumber(upper, length * 1, 1);
                    axis_i->intervalDecimals = std::max(getDecimal(lower), getDecimal(upper));
                    axis_i->tickDecimals = axis_i->intervalDecimals;
                    axis_i->tickSpacing = upper - lower;
                }
            }
            //更新范围数据
            axis_i->mediumValue = (upper + lower) / 2;
            axis_i->propValue = (upper - lower) / 2;
            axis_i->interval[0] = -1.0f;
            axis_i->interval[1] = 1.0f;
            double maxabs = std::max(abs(lower), abs(upper));
            axis_i->magnitude = floor(std::log10(maxabs));//计算总体数量级。不会受tick的数量级影响。10^(数量级+1)>=value>=10^(数量级)
        }
        if (!axis_i->autoRange && !axis_i->autoTick && axis_i->tickType == TickType::TickNum) {
            if (axis_i->tickSpaceNum != 0) {
                axis_i->tickSpacing = length / axis_i->tickSpaceNum;
            }
            else {
                axis_i->tickSpacing = length;
            }
            axis_i->tickDecimals = getDecimal(axis_i->tickSpacing);
            if (axis_i->tickDecimals > axis_i->intervalDecimals+2) {
                axis_i->tickDecimals = axis_i->intervalDecimals + 2;
            }
            axis_i->displayDecimals = std::max(axis_i->tickDecimals, axis_i->intervalDecimals);
        }
        //qDebug() << "after" << axis_i->interval[0] << ":" << axis_i->interval[1];
        //qDebug() << "space" << axis_i->tickSpacing;
        //设置显示的小数位数
        bool setDisplayDecimalsProcess = true;
        if (setDisplayDecimalsProcess) {
            if (!axis_i->autoRange && axis_i->autoTick) {
                axis_i->displayDecimals = std::max(axis_i->tickDecimals, axis_i->intervalDecimals);
            }
            else if (!axis_i->autoTick && axis_i->tickType==TickType::TickNum) {
                axis_i->displayDecimals = std::max(axis_i->tickDecimals, axis_i->intervalDecimals);
            }
            else if ((!axis_i->autoTick && axis_i->tickType == TickType::TickSpace)|| axis_i->autoTick) {
                axis_i->displayDecimals = std::max(axis_i->tickDecimals, 0);
            }
        }
    }
}
//自动设置坐标轴
void GLFigure3D::autoUpdateAxisFormat() {
    int temp; 
    //hand是一个校正因子，用于校正pitch不同的情况
    int hand = 1;
    //坐标轴的顺序，0代表x轴，1代表y轴，2代表z轴，位置0代表左下侧坐标轴，位置1代表右下侧坐标轴，位置2代表上侧坐标轴(在这里一直为z轴)
    int pxyz[3] = { 0,1,2 };
    //在自动调整坐标轴位置时判断坐标轴等效到立方体背面时的正方向。
    int axyz[3] = { -1, -1, 1 };
    //qDebug() << axyz[0] << "," << axyz[1] << "," << axyz[2] << ":" << pxyz[0] << "," << pxyz[1] << "," << pxyz[2];;
    if (camera->Yaw < 90.0 && camera->Yaw >= -90.0) {
        axyz[0] = 1;
    }
    if (camera->Yaw < 180.0 && camera->Yaw >= 0.0) {
        axyz[1] = 1;
    }
    //qDebug() << axyz[0] << "," << axyz[1] << "," << axyz[2] << ":" << pxyz[0] << "," << pxyz[1] << "," << pxyz[2];;
    if ((camera->Yaw < 180.0 && camera->Yaw >= 90.0) || (camera->Yaw < 0 && camera->Yaw >= -90)) {
        pxyz[0] = 1;
        pxyz[1] = 0;
    }
    //qDebug() << axyz[0] << "," << axyz[1] << "," << axyz[2] << ":" << pxyz[0] << "," << pxyz[1] << "," << pxyz[2];;
    if (camera->Pitch >= 0.0) {
        axyz[0] = -axyz[0];
        axyz[1] = -axyz[1];
        temp = pxyz[0]; pxyz[0] = pxyz[1]; pxyz[1] = temp;
        hand = -1;
    }
    setAxisFormat(axyz, pxyz, hand);
}
//设置坐标轴的位置、刻度和背景网格方向。
void GLFigure3D::setAxisFormat(int axyz[3], int pxyz[3], int hand) {
    //设置坐标轴平移方向，0表示平移到interval[0]，1表示平移到interval[1]
    for (int i = 0; i < 3; i++) {
        axis_list[i]->offsetSide = { 0, 0, 0 };//ch//默认情况下皆朝负方向移动，因为原轴从零开始缩放，所以在原轴方向上补偿上负值
    }
    //qDebug() << axyz[0]<<","<< axyz [1]<<","<< axyz[2] << ":" << pxyz[0] << "," << pxyz[1] << "," << pxyz[2];
    //从原轴移动到盒子外侧轴。
    axis_list[pxyz[0]]->offsetSide[pxyz[1]] = (axyz[pxyz[1]] + 1) / 2;//方盒背部(仰视则为正部)左侧轴，向右侧轴方向移动。
    axis_list[pxyz[1]]->offsetSide[pxyz[0]] = (axyz[pxyz[0]] + 1) / 2;//方盒背部(仰视则为正部)右侧轴，向左侧轴方向移动。
    axis_list[pxyz[2]]->offsetSide[pxyz[0]] = (axyz[pxyz[0]] + 1) / 2;//方盒背部(仰视则为正部)上轴(z)，向左侧轴方向移动。
    axis_list[pxyz[2]]->offsetSide[pxyz[1]] = (-axyz[pxyz[1]] + 1) / 2;//方盒背部(仰视则为正部)上轴(z)，向右侧轴负方向移动。

    //从原轴移动到盒子中心轴-即使没有自动更新也需要计算。
    for (int i = 0; i < 3; i++) {
        axis_list[i]->offsetCenter[0] = (-axyz[0] * hand + 1) / 2;
        axis_list[i]->offsetCenter[1] = (-axyz[1] * hand + 1) / 2;
        axis_list[i]->offsetCenter[2] = (-axyz[2] * hand + 1) / 2;
        axis_list[i]->offsetCenter[i] = 0;//因为原轴从零开始缩放，所以在原轴方向上补偿上负值
    }

    //原轴方向变换到刻度方向
    //注意仰视若还看作是方盒背部，则坐标系变为左手系。平移时不用考虑手性，但旋转时需要考虑
    //若仰视看作方盒正面，则旋转时按相反规则，也相当于乘一个手性因子。
    
    axis_list[pxyz[0]]->axisToTick = glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(0, 0, axyz[pxyz[2]] * axyz[pxyz[0]] * hand));//方盒背部(仰视则为正部)左侧轴，绕上轴旋转90°，受左侧轴和上轴方向影响。
    axis_list[pxyz[1]]->axisToTick = glm::rotate(glm::mat4(1.0), glm::radians(-90.0f), glm::vec3(0, 0, axyz[pxyz[2]] * axyz[pxyz[1]] * hand));//方盒背部(仰视则为正部)右侧轴，绕上轴旋转-90°，受右侧轴和上轴方向影响。
    if (abs(camera->Pitch) < 20.0) {
        glm::vec3 rotate_axis = { 0.0,0.0,0.0 };
        if ((camera->Yaw < 90.0-(1-hand)*22.5 && camera->Yaw >= 45.0 - (1 - hand) * 22.5) ||
            (camera->Yaw < 0.0 - (1 - hand) * 22.5 && camera->Yaw >= -45.0 - (1 - hand) * 22.5)||
            (camera->Yaw < 180.0 - (1 - hand) * 22.5 && camera->Yaw >= 135.0 - (1 - hand) * 22.5) || 
            (camera->Yaw < -90 - (1 - hand) * 22.5 && camera->Yaw >= -135 - (1 - hand) * 22.5)) {
            rotate_axis[pxyz[0]] = axyz[pxyz[0]] * hand;
            axis_list[pxyz[0]]->axisToTick = glm::rotate(glm::mat4(1.0), glm::radians(-90.0f), rotate_axis) * axis_list[pxyz[0]]->axisToTick;
        }
        else {
            rotate_axis[pxyz[1]] = axyz[pxyz[1]] * hand;
            axis_list[pxyz[1]]->axisToTick = glm::rotate(glm::mat4(1.0), glm::radians(90.0f), rotate_axis) * axis_list[pxyz[1]]->axisToTick;
        }
        
    }
    glm::vec3 rotate_axis = { 0.0,0.0,0.0 };
    rotate_axis[pxyz[0]] = axyz[pxyz[0]] * axyz[pxyz[2]] * hand;
    axis_list[pxyz[2]]->axisToTick = glm::rotate(glm::mat4(1.0), glm::radians(90.0f), rotate_axis);//方盒背部(仰视则为正部)上轴，绕左侧轴旋转90°，受左轴和上轴方向影响。

    //int j = 0; Axis* axis_i; glm::vec4 vec;
    // vec = glm::vec4(0.0, 0.0, 0.0, 1.0); vec[j] = 1.0;
    // axis_i = axis_list[j++];
    //qDebug() << (axis_i->axisToTick* vec).x << "," << (axis_i->axisToTick * vec).y << "," << (axis_i->axisToTick * vec).z ;
    //vec = glm::vec4(0.0, 0.0, 0.0, 1.0); vec[j] = 1.0;
    //axis_i = axis_list[j++];
    //qDebug() << (axis_i->axisToTick * vec).x << "," << (axis_i->axisToTick * vec).y << "," << (axis_i->axisToTick * vec).z;
    //vec = glm::vec4(0.0, 0.0, 0.0, 1.0); vec[j] = 1.0;
    //axis_i = axis_list[j++];
    //qDebug() << (axis_i->axisToTick * vec).x << "," << (axis_i->axisToTick * vec).y << "," << (axis_i->axisToTick * vec).z;
}

//绘制函数
void GLFigure3D::paintGL()
{
    //qDebug() << "is sharing:" << context()->areSharing(context(), resource->context());
    //this->makeCurrent();
    //qDebug() << "paint";
    //更新数据
    Range3D total = getDataBlockInfoListRange();
    if (total.init) {
        autoUpdateAxisRangeAndTick(total);
    }

    autoUpdateAxisFormat();
    view = camera->GetViewMatrix();
    //qDebug() <<"axis" << axis_list[0]->interval[0]* axis_list[0]->propValue+ axis_list[0]->mediumValue <<","
    //    << axis_list[0]->interval[1] * axis_list[0]->propValue + axis_list[0]->mediumValue << ":" <<
    //    axis_list[1]->interval[0] * axis_list[1]->propValue + axis_list[1]->mediumValue <<","
    //    << axis_list[1]->interval[1] * axis_list[1]->propValue + axis_list[1]->mediumValue << ":" <<
    //    axis_list[2]->interval[0] * axis_list[2]->propValue + axis_list[2]->mediumValue <<","
    //    << axis_list[2]->interval[1] * axis_list[2]->propValue + axis_list[2]->mediumValue;
    Range3D range = getBoxRange(view, axis_list[0]->interval, axis_list[1]->interval, axis_list[2]->interval);

    ////窗口大小改变视图比例不变——
    //int window_size_factor = 1000;
    //figureWidth = camera->Size * this->width() / window_size_factor;
    //figureHeight = camera->Size * this->height() / window_size_factor;
    //projection0 = glm::ortho(-figureWidth,figureWidth,
    //    -figureHeight,figureHeight, range.minimum.z-50, range.maximum.z+50);//glm::perspective(camera->Zoom, (float)this->width() / (float)this->height(), 0.1f, 1000.0f);//这种情况是透视投影
    //info.figureRectSize.x = figureWidth;
    //info.figureRectSize.y = figureHeight;
    //窗口大小改变占满比例不变——
    windowOffset[0][0] = 0.2 * (range.maximum.x - range.minimum.x) * 1.2f;
    windowOffset[0][1] = 0.2 * (range.maximum.x - range.minimum.x) * 0.8f;
    windowOffset[1][0] = 0.2 * (range.maximum.y - range.minimum.y) * 1.2f;
    windowOffset[1][1] = 0.2 * (range.maximum.y - range.minimum.y) * 0.8f;

    figureWidth = range.maximum.x - range.minimum.x + windowOffset[0][0]+ windowOffset[0][1];
    figureHeight = range.maximum.y - range.minimum.y + windowOffset[1][0] + windowOffset[1][1];
    projection = glm::ortho(range.minimum.x- windowOffset[0][0], range.maximum.x + windowOffset[0][1],
        range.minimum.y - windowOffset[1][0], range.maximum.y + windowOffset[1][1], range.minimum.z - 50, range.maximum.z + 50);//glm::perspective(camera->Zoom, (float)this->width() / (float)this->height(), 0.1f, 1000.0f);//这种情况是透视投影
    info.figureRectSize.x = figureWidth;
    info.figureRectSize.y = figureHeight;
    //qDebug() <<"range:" << range.minimum.x << ":" << range.minimum.y << ":" << range.minimum.z;
    //qDebug() <<"range:" << range.maximum.x << ":" << range.maximum.y << ":" << range.maximum.z;
    windowTextScale.x = 500.0 / this->width();
    windowTextScale.y = 500.0 / this->height();
    //更新刻度值文字的显示大小，保持缩放时刻度数字大小不变
    //for (int i = 0; i < 3; i++) {
        //axis_list[i]->tickValueScale = 0.005f * camera->Size / 2;
    //}
    
    GLenum err;
    //绘图
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //qDebug() << "clear over";
    paintAxis();
    //qDebug() << "paint axis over";
    paintDataBlocksList();
    //qDebug() << "paint blockList over";
    paintTextList();
    //qDebug() << "paintTextList over";
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << std::endl;
        qDebug() << "OpenGL error: " << err;
        qDebug() << "error";
    }
    //qDebug() << "gl update complete";
    emit updateCompleted();
    //qDebug() << "paint over";
}
void GLFigure3D::paintAxis() {
    float gl_line_width = 3.0f;
    int jumpAxis = 3;
    float tolerance = 1.0f;
    if (abs(abs(camera->Pitch) - 90.0f) < tolerance) {
        jumpAxis = 2;
    }
    else {
        if (abs(camera->Pitch) < tolerance) {
            if (abs(abs(camera->Yaw) - 180.0f) < tolerance ||abs(camera->Yaw)< tolerance) {
                jumpAxis = 0;
            }
            else if (abs(abs(camera->Yaw) - 90.0f) < tolerance) {
                jumpAxis = 1;
            }
        }
    }
    for (int i = 0; i < 3; i++) {
        if (i == jumpAxis) {
            continue;
        }

        glUseProgram(resource->axisShader->Program);//不能移到前面，因为绘制字符时使用的是另一个Program
        glUniformMatrix4fv(glGetUniformLocation(resource->axisShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(resource->axisShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        Axis* axis_i = axis_list[i]; float length = axis_i->interval[1] - axis_i->interval[0]; glm::mat4 axis_model;
        //绘制坐标轴
        if (true) {
            glUniform1f(glGetUniformLocation(resource->axisShader->Program, "axisLength"), axis_i->interval[1] - axis_i->interval[0]);
            glUniform3f(glGetUniformLocation(resource->axisShader->Program, "myColor"), axis_i->color.x, axis_i->color.y, axis_i->color.z);
            
            glBindVertexArray(axis_i->axisVao);
            glLineWidth(gl_line_width);
            glPointSize(gl_line_width);
            
            glm::vec3 axis_trans = { 0.0f, 0.0f, 0.0f };
            for (int p = 0; p < 3; p++) {
                axis_trans[p] = axis_list[p]->interval[axis_i->offsetSide[p]];
            }
            axis_model = glm::translate(glm::mat4(1.0f), axis_trans);
            glUniformMatrix4fv(glGetUniformLocation(resource->axisShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(axis_model));
            glDrawArrays(GL_LINES, 0, 2);
        }
        //qDebug() << "axisline over";
        //GLenum err;
        //while ((err = glGetError()) != GL_NO_ERROR) {
        //    std::cerr << "OpenGL error: " << err << std::endl;
        //    qDebug() << "OpenGL error: " << err;
        //    qDebug() << "error";
        //}
        //qDebug() << "detex over";
        //背景线
        glUniform3f(glGetUniformLocation(resource->axisShader->Program, "myColor"), 0.5, 0.5, 0.5);
        glLineWidth(1.0f);
        for (int j = 0; j < 3; j++) {
            if (j == i) { continue; }
            int k = 3 - i - j;
            glUniform1f(glGetUniformLocation(resource->axisShader->Program, "axisLength"), length);
            glm::vec3 tick_trans = {0.0f,0.0f,0.0f};
            float length_j = axis_list[j]->interval[1] - axis_list[j]->interval[0];
            float length_k = axis_list[k]->interval[1] - axis_list[k]->interval[0];
            tick_trans[i] = axis_list[i]->interval[0];//原轴移动到原位
            //tick_trans[j] = axis_list[j]->interval[axis_list[i]->offsetCenter[j]];//移动轴不需要补偿
            tick_trans[k] = axis_list[k]->interval[axis_list[i]->offsetCenter[k]];
            tick_trans[j] = axis_list[j]->interval[0];
            //由于i为坐标轴，j为移动轴(i=0->x轴,=1->y轴,=2->z轴)，所以绘制的线总在与ioj平行平面上，而k为垂直于该平面的轴。
            //tick_trans[k] = (j+i == 2 || j+i==3) ? axis_list[k]->interval[1] : axis_list[k]->interval[0];//即把与xoz平行平面上的线向y正方向移动；yoz向着x正方向移动
            //qDebug() << axis_list[j]->interval[1] << ":" << length_j << ":" << axis_list[j]->tickSpacing / axis_list[j]->propValue;
            while ((axis_list[j]->interval[1] - tick_trans[j]) / (axis_list[j]->tickSpacing/axis_list[j]->propValue) > -1e-3) {
                glUniformMatrix4fv(glGetUniformLocation(resource->axisShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(glm::translate(glm::mat4(1.0f), tick_trans)));
                glDrawArrays(GL_LINES, 0, 2);

                tick_trans[j] += axis_list[j]->tickSpacing / axis_list[j]->propValue;
            }
        }
        //qDebug() << "backline over";
        //while ((err = glGetError()) != GL_NO_ERROR) {
        //    std::cerr << "OpenGL error: " << err << std::endl;
        //    qDebug() << "OpenGL error: " << err;
        //    qDebug() << "error";
        //}
        glm::vec3 tick_trans; glm::vec3 tickVec;
        //绘制刻度
        
        glLineWidth(gl_line_width);
        glPointSize(gl_line_width);
        glUniform3f(glGetUniformLocation(resource->axisShader->Program, "myColor"), axis_i->color.x, axis_i->color.y, axis_i->color.z);
        

        glm::vec3 axisVec = { 0.0f,0.0f,0.0f }; axisVec[i] = 1.0f;
        tickVec = projection*view * axis_i->axisToTick * glm::vec4(axisVec, 1.0f) - projection * view * axis_i->axisToTick * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);//未标准化
        //使图上显示距离为0.1。由于我只想要xOy平面显示的距离为0.1，而不是让三维真实距离为0.1。
        // 且由于axisLength最终会受projection影响(在渲染器内)
        axis_i->tickLength = 0.05 / sqrt(pow(tickVec.x / windowTextScale.x, 2) + pow(tickVec.y / windowTextScale.y, 2));
        glUniform1f(glGetUniformLocation(resource->axisShader->Program, "axisLength"), axis_i->tickLength);//刻度的长度

        axisVec = view * glm::vec4(axisVec, 1.0f) - view * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);//轴方向，未标准化
        glm::vec3 paxisVec = projection * glm::vec4(axisVec,0.0f);

        tick_trans = { 0.0f, 0.0f, 0.0f };
        while ((length - tick_trans[i]) / (axis_i->tickSpacing/axis_i->propValue) > -1e-3) {
            glUniformMatrix4fv(glGetUniformLocation(resource->axisShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(glm::translate(axis_model, tick_trans) * axis_i->axisToTick));
            glDrawArrays(GL_LINES, 0, 2);
            tick_trans[i] += axis_i->tickSpacing / axis_i->propValue;
        }
        //绘制文字
        axis_list[2]->tickValueLeft = 100; axis_list[0]->tickValueLeft = 0; axis_list[1]->tickValueLeft = 0;
        tick_trans = { 0.0f, 0.0f, 0.0f };
        glm::vec3 init_pos = glm::vec3(0.0, 0.0, 0.0); init_pos[i] = 1.0; init_pos = axis_i->axisToTick * glm::vec4(init_pos, 1.0);//刻度轴方向
        while ((length - tick_trans[i]) / (axis_i->tickSpacing / axis_i->propValue) > -1e-3) {
            //QString::number(value, 'f', n)只能用来省略小数点之后的，如果n小于0，那么会不进行格式化
            float value = (tick_trans[i] + axis_i->interval[0]) * axis_i->propValue + axis_i->mediumValue;
            std::wstring tickText = 
                QString::number(
                    value, 'f', axis_i->displayDecimals
                ).toStdWString();
            //科学计数法表示如0.0003的数
            if (axis_i->magnitude < -1) {//当数字尺度小于-1时，如0.021尺度为-2
                size_t decimalPos = tickText.find(L'.');
                if (decimalPos != std::wstring::npos) {
                    size_t position = decimalPos -axis_i->magnitude;//第一个数字的位置，如-0.00123,第一个数字位置为5
                    if(position+1<=tickText.size())
                        tickText = tickText.substr(position, 1)+L"."+ tickText.substr(position+1)
                            +L"e"+QString::number(axis_i->magnitude).toStdWString();
                    else
                        tickText = tickText.substr(position, 1) + L"e" + QString::number(axis_i->magnitude).toStdWString();
                    if (value < 0) {
                        tickText = L"-" + tickText;
                    }
                    
                }
            }

            glm::vec2 string_size = getStringSize(tickText, axis_i->tickValueScale);
            glm::vec3 offset_pos = glm::vec3(-string_size.x * windowTextScale.x / 2, -string_size.y * windowTextScale.y / 2, 0);
            
            glm::vec3 trans_pos = projection * view * glm::translate(axis_model, tick_trans) * glm::vec4(init_pos * axis_i->tickLength * 1.2f, 1.0f);
            glm::vec3 axis_pos = projection * view * glm::translate(axis_model, tick_trans) * glm::vec4(0.0f,0.0f,0.0f, 1.0f);

            if (i == 2) {
                trans_pos += glm::vec3(-string_size.x / 2 * windowTextScale.x, 0.0f, 0.0f);
                axis_i->tickValueLeft = std::min(trans_pos.x + offset_pos.x, axis_i->tickValueLeft);
            }
            else {
                if (string_size.x / string_size.y > abs(tickVec.x / tickVec.y)) {
                    trans_pos += glm::vec3(string_size.y / 2 / abs(tickVec.y) * tickVec.x*windowTextScale.x, -string_size.y/2*windowTextScale.y, 0.0f);
                }
                else {
                    trans_pos += glm::vec3(string_size.y / 2 / abs(string_size.y) * tickVec.x/abs(tickVec.x)* string_size.x * windowTextScale.x, -string_size.y / 2 * windowTextScale.y, 0.0f);
                }
                axis_i->tickValueLeft = std::max(abs((trans_pos.y + offset_pos.y - axis_pos.y) -paxisVec.y/paxisVec.x* (trans_pos.x - offset_pos.x - axis_pos.x)), axis_i->tickValueLeft);
                axis_i->tickValueLeft = std::max(abs((trans_pos.y + offset_pos.y - axis_pos.y) - paxisVec.y / paxisVec.x * (trans_pos.x + offset_pos.x - axis_pos.x)), axis_i->tickValueLeft);
            }
            //glm::vec3 trans_pos = view * glm::translate(axis_model, tick_trans / length) * glm::vec4(init_pos * axis_i->tickLength * 1.2f
            //    + init_pos * string_size.x * windowTextScale.x / 2.0f, 1.0f);//再向刻度轴方向移动一段距离
            renderText(tickText.c_str(), trans_pos, offset_pos, axis_i->tickValueScale, glm::vec3(0.0, 0.0f, 0.0f), glm::mat4(1.0f));

            tick_trans[i] += (axis_i->tickSpacing / axis_i->propValue);
        }
        //qDebug() << axis_i->tickValueLeft * abs(paxisVec.x * paxisVec.y) / (float)sqrt(pow(paxisVec.x, 2) + pow(paxisVec.y, 2));
        //qDebug() << "tickvalue over";
        //while ((err = glGetError()) != GL_NO_ERROR) {
        //    std::cerr << "OpenGL error: " << err << std::endl;
        //    qDebug() << "OpenGL error: " << err;
        //    qDebug() << "error";
        //}
        //{z轴按刻度平面，平行绘制标签
        //tick_trans = { 0.0f, 0.0f, 0.0f }; tick_trans[i] = 1.0f+axis_i->labelOffset.x; 
        //tickVec = projection * glm::vec4(tickVec, 1.0f); tickVec = glm::normalize(tickVec);
        //glm::vec3 trans_pos = projection * view * glm::translate(axis_model, tick_trans) * glm::vec4(init_pos * axis_i->tickLength * 8.0f, 1.0f);
        //trans_pos += tickVec * axis_i->labelOffset.y;
        //glm::vec2 string_size = getStringSize(axis_i->label, axis_i->tickValueScale);
        //glm::vec3 offset_pos = glm::vec3(-string_size.x * windowTextScale.x / 2, -string_size.y * windowTextScale.y / 2, 0);
        //renderText(axis_i->label.c_str(), trans_pos, offset_pos, axis_i->labelScale, axis_i->labelColor);
        //}
        //按视平面，垂直绘制z轴标签
        if (i == 2) {
            tick_trans = { 0.0f, 0.0f, 0.0f }; tick_trans[i] = 1.0f + axis_i->labelOffset.x;
            glm::vec2 string_size0 = getStringSize(L"轴", axis_i->tickValueScale);

            glm::vec3 axis_middle = projection * view * glm::translate(axis_model, tick_trans) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
            axis_middle.x = axis_i->tickValueLeft - string_size0.x * windowTextScale.x / 2;// 2 * windowOffset[0][0] / figureWidth
            axis_middle.y += axis_i->labelOffset.y;

            double offsetY = string_size0.y * (axis_i->label.size() / 2.0) * windowTextScale.y;
            for (int ti = 0; ti < axis_i->label.size(); ti++) {
                std::wstring axisc = axis_i->label.substr(ti, 1);
                glm::vec2 string_size = getStringSize(axisc, axis_i->tickValueScale);
                glm::vec3 offset_pos = glm::vec3(-string_size.x * windowTextScale.x / 2.0, offsetY, 0);
                renderText(axisc.c_str(), axis_middle, offset_pos, axis_i->labelScale, axis_i->labelColor, glm::mat4(1.0f));
                offsetY -= string_size.y * windowTextScale.y;
            }
        }
        else {
            tick_trans = { 0.0f, 0.0f, 0.0f }; tick_trans[i] = 1.0f + axis_i->labelOffset.x;
            //tickVec = projection * glm::vec4(tickVec, 1.0f); 
            tickVec.z = 0;
            //axisVec = projection * glm::vec4(axisVec, 1.0f);
            glm::vec3 perpVec;
            if (paxisVec.x < 0) {
                perpVec = glm::normalize(glm::vec3(-paxisVec.y, paxisVec.x, 0.0f));
            }
            else {
                perpVec = glm::normalize(glm::vec3(paxisVec.y, -paxisVec.x, 0.0f));
            }
            glm::vec2 string_size = getStringSize(axis_i->label, axis_i->tickValueScale);
            glm::vec3 trans_pos = projection * view * glm::translate(axis_model, tick_trans) * glm::vec4(init_pos * axis_i->tickLength * 0.0f, 1.0f);
            trans_pos += perpVec * (axis_i->tickValueLeft * (float)(abs(paxisVec.x) / sqrt(pow(paxisVec.x, 2) + pow(paxisVec.y, 2))));
            trans_pos += tickVec*axis_i->labelOffset.y;
            //trans_pos += tickVec * 0.1f;

            glm::vec3 offset_pos = glm::vec3(-string_size.x / 2.0, -string_size.y*1.2f, 0);
            //renderText(axis_i->label.c_str(), trans_pos, offset_pos, axis_i->labelScale, axis_i->labelColor, glm::mat4(1.0f));
            //axisVec = view * glm::vec4(axisVec, 1.0f)- view * glm::vec4(0.0f,0.0f,0.0f, 1.0f);
            renderTextWithoutWindowScale(axis_i->label.c_str(), trans_pos, offset_pos, axis_i->labelScale, axis_i->labelColor,
                glm::rotate(glm::scale(glm::mat4(1.0f), glm::vec3(windowTextScale.x, windowTextScale.y, 1.0f)), glm::atan((axisVec.y / windowTextScale.y)/(axisVec.x / windowTextScale.x)), glm::vec3(0, 0, 1)));
        }
    }
}
void GLFigure3D::paintDataBlocksList() {
    for (auto it = dataBlockInfoList.begin(); it != dataBlockInfoList.end(); it++) {
        if((*it)->dataBlock!=nullptr)
            paintDataBlocks((*it));
    }
}
void GLFigure3D::paintDataBlocks(DataBlockInfo* dataBlockInfo) {
    glm::vec3 scale_vector = glm::vec3(0.0f);
    glm::vec3 offset_vector = glm::vec3(0.0f);
    for (int i = 0; i < 3; i++) {
        scale_vector[i] = 1/axis_list[i]->propValue;
        offset_vector[i] = -axis_list[i]->mediumValue;
    }
    glUseProgram(resource->dataShader->Program);
    glm::mat4 mvp = projection * view * glm::translate(glm::scale(glm::mat4(1.0f), scale_vector), offset_vector);
    glUniformMatrix4fv(glGetUniformLocation(resource->dataShader->Program, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
    //glUniformMatrix4fv(glGetUniformLocation(resource->dataShader->Program, "mvp2"), 1, GL_FALSE, glm::value_ptr(mvp));
    glm::vec3 upperBound;
    glm::vec3 lowerBound;
    for (int i = 0; i < 3; i++) {
        Axis* axis_i = axis_list[i];
        upperBound[i] = axis_i->interval[1] * axis_i->propValue + axis_i->mediumValue;
        lowerBound[i] = axis_i->interval[0] * axis_i->propValue + axis_i->mediumValue;
        qDebug() <<"data bound box:" << upperBound[i] << ":" << lowerBound[i];
    }
    glUniform3f(glGetUniformLocation(resource->dataShader->Program, "maxBounds"), upperBound[0], upperBound[1], upperBound[2]);
    glUniform3f(glGetUniformLocation(resource->dataShader->Program, "minBounds"), lowerBound[0], lowerBound[1], lowerBound[2]);

    int left_num = dataBlockInfo->dataNum;
    DataBlock* block_ptr = dataBlockInfo->dataBlock;
    if (left_num == 1) {//如果全部只有一个点，那么就绘制一个点。
        left_num = 0;
        glBindVertexArray(block_ptr->dataVao);
        glPointSize(dataBlockInfo->lineWidth);
        glUniform3f(glGetUniformLocation(resource->dataShader->Program, "myColor"),
            dataBlockInfo->color.red() / 255.01, 
            dataBlockInfo->color.green() / 255.01, 
            dataBlockInfo->color.blue() / 255.01);
        glDrawArrays(GL_LINE_STRIP, 0, left_num + dataBlockInfo->blockSize + 1);//第一个点
    }
    while (left_num > 0) {
        left_num -= dataBlockInfo->blockSize;
        glBindVertexArray(block_ptr->dataVao);
        glLineWidth(dataBlockInfo->lineWidth);
        //glPointSize(gl_line_width);
        glUniform3f(glGetUniformLocation(resource->dataShader->Program, "myColor"),
            dataBlockInfo->color.red() / 255.01,
            dataBlockInfo->color.green() / 255.01,
            dataBlockInfo->color.blue() / 255.01);
        if (left_num >= 0) {
            glDrawArrays(GL_LINE_STRIP, 0, dataBlockInfo->blockSize+1);//加上第一个点
        }
        else {
            glDrawArrays(GL_LINE_STRIP, 0, left_num + dataBlockInfo->blockSize + 1);//加上第一个点
        }
        block_ptr = block_ptr->nextDataBlock;
    }
}
void GLFigure3D::paintSimpleData(glm::vec3 p1, glm::vec3 p2, glm::vec3 color) {
    void* ptr1 = glMapNamedBufferRange(simpleDataBlock.dataVbo, 0, 6 * sizeof(float), GL_MAP_WRITE_BIT);
    if (ptr1) {
        float last_point_data[6] = { p1.x,p1.y,p1.z,p2.x,p2.y,p2.z };
        memcpy(ptr1, last_point_data, 6 * sizeof(float));
        glUnmapNamedBuffer(simpleDataBlock.dataVbo);
    }

    glUseProgram(resource->simpleShader->Program);

    int left_num = 2;
    glBindVertexArray(simpleDataBlock.dataVao);
    glLineWidth(1.0f);
    glPointSize(2.0f);
    glUniform3f(glGetUniformLocation(resource->simpleShader->Program, "myColor"), color.x, color.y, color.z);
    glDrawArrays(GL_LINE_STRIP, 0, left_num);
}

void GLFigure3D::renderText(const wchar_t* text, glm::vec3 pos, glm::vec3 offset, GLfloat scale, glm::vec3 color, glm::mat4 transM)
{
    // 激活合适的渲染状态
    glUseProgram(resource->characterShader->Program);
    //glUniformMatrix4fv(glGetUniformLocation(characterShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(resource->characterShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(transM));
    glUniform1i(glGetUniformLocation(resource->characterShader->Program, "text"), 0);
    glUniform3f(glGetUniformLocation(resource->characterShader->Program, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    
    glBindVertexArray(characterObject->vao);
    //qDebug() << "posx:" << pos.x;
    // 对文本中的所有字符迭代
    for (int i=0;i< wcslen(text);i++)
    {
        //auto it = resource->characters.find(text[i]);
        //if (it == resource->characters.end()) {
        //    if (text[i] > 0x7F) {
        //        loadUnicodeCharacters(text);
        //    }
        //    else {
        //        loadASCIICharacters(text);
        //    }
        //    it = resource->characters.find(text[i]);
        //}
        //Character ch = it->second;
        Character ch = resource->findCharacter(text, i);
        GLfloat xpos = pos.x + ch.Bearing.x * scale * windowTextScale.x +offset.x ;
        GLfloat ypos = pos.y - (ch.Size.y - ch.Bearing.y) * scale * windowTextScale.y + offset.y;
        GLfloat zpos = pos.z+offset.z;

        GLfloat w = ch.Size.x * scale * windowTextScale.x;
        GLfloat h = ch.Size.y * scale * windowTextScale.y;
        // 当前字符的VBO
        GLfloat position[6][3] = {
            { xpos,     ypos + h,   zpos},
            { xpos,     ypos,       zpos},
            { xpos + w, ypos,       zpos},

            { xpos,     ypos + h,   zpos},
            { xpos + w, ypos,       zpos},
            { xpos + w, ypos + h,   zpos}
        };
        GLfloat tex[6][2] = {
            {0.0, 0.0},{0.0, 1.0},{1.0, 1.0},{0.0, 0.0},{1.0, 1.0},{1.0, 0.0}
        };
        // 在方块上绘制字形纹理
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glUniform1i(glGetUniformLocation(resource->characterShader->Program, "text"), 0);
        // 更新当前字符的VBO
        glBindBuffer(GL_ARRAY_BUFFER, characterObject->vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(position), position);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(position), sizeof(tex), tex);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // 绘制方块
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // 更新位置到下一个字形的原点，注意单位是1/64像素
        pos.x += (ch.Advance >> 6) * scale * windowTextScale.x; //(2^6 = 64)
        //if (i == 0) {
        //    qDebug() << "ch.size.y*scale:" << ch.Size.y*scale;
        //}
    }
    //qDebug() << "posx:" << pos.x;
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
void GLFigure3D::renderTextWithoutWindowScale(const wchar_t* text, glm::vec3 pos, glm::vec3 offset, GLfloat scale, glm::vec3 color, glm::mat4 transM)
{
    // 激活合适的渲染状态
    glUseProgram(resource->characterShader->Program);
    //glUniformMatrix4fv(glGetUniformLocation(characterShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(resource->characterShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(glm::translate(glm::mat4(1.0f), pos)* transM));
    glUniform1i(glGetUniformLocation(resource->characterShader->Program, "text"), 0);
    glUniform3f(glGetUniformLocation(resource->characterShader->Program, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(characterObject->vao);
    //qDebug() << "posx:" << pos.x;
    // 对文本中的所有字符迭代
    glm::vec3 init_pos = glm::vec3(0.0f);
    for (int i = 0; i < wcslen(text); i++)
    {
        //auto it = resource->characters.find(text[i]);
        //if (it == resource->characters.end()) {
        //    if (text[i] > 0x7F) {
        //        loadUnicodeCharacters(text);
        //    }
        //    else {
        //        loadASCIICharacters(text);
        //    }
        //    it = resource->characters.find(text[i]);
        //}
        //Character ch = it->second;
        Character ch = resource->findCharacter(text, i);
        GLfloat xpos = init_pos.x + ch.Bearing.x * scale + offset.x;
        GLfloat ypos = init_pos.y - (ch.Size.y - ch.Bearing.y) * scale + offset.y;
        GLfloat zpos = init_pos.z + offset.z;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // 当前字符的VBO
        GLfloat position[6][3] = {
            { xpos,     ypos + h,   zpos},
            { xpos,     ypos,       zpos},
            { xpos + w, ypos,       zpos},

            { xpos,     ypos + h,   zpos},
            { xpos + w, ypos,       zpos},
            { xpos + w, ypos + h,   zpos}
        };
        GLfloat tex[6][2] = {
            {0.0, 0.0},{0.0, 1.0},{1.0, 1.0},{0.0, 0.0},{1.0, 1.0},{1.0, 0.0}
        };
        // 在方块上绘制字形纹理
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glUniform1i(glGetUniformLocation(resource->characterShader->Program, "text"), 0);
        // 更新当前字符的VBO
        glBindBuffer(GL_ARRAY_BUFFER, characterObject->vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(position), position);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(position), sizeof(tex), tex);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // 绘制方块
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // 更新位置到下一个字形的原点，注意单位是1/64像素
        init_pos.x += (ch.Advance >> 6) * scale; //(2^6 = 64)
        //if (i == 0) {
        //    qDebug() << "ch.size.y*scale:" << ch.Size.y*scale;
        //}
    }
    //qDebug() << "posx:" << pos.x;
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLFigure3D::paintTextList()
{
    //绘制除tick外的所有字符
    for (auto it = textInfoList.begin(); it != textInfoList.end(); it++) {
        TextInfo* textInfo = *it;
        if (textInfo->rangeInit != true) {
            textInfo->rangeInit = true;
            textInfo->range = getStringSize(textInfo->text, textInfo->scale);
        }
        //qDebug() << QString::fromStdString(textInfo->text);
        renderText(textInfo->text.c_str(), textInfo->pos, glm::vec3(-textInfo->range.x * windowTextScale.x /2, -textInfo->range.y * windowTextScale.y/2, 0.0), textInfo->scale, textInfo->color, glm::mat4(1.0f));
        if (textInfo->selected) {//如果文字被选中，则在文字周围绘制文字选框。
            glUseProgram(resource->rectShader->Program);//不能移到前面，因为绘制字符时使用的是另一个Program
            glUniformMatrix4fv(glGetUniformLocation(resource->rectShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
            glUniformMatrix4fv(glGetUniformLocation(resource->rectShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
            glUniformMatrix4fv(glGetUniformLocation(resource->rectShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
            glUniform3f(glGetUniformLocation(resource->rectShader->Program, "myColor"), 0, 0, 1);
            //glUniform1f(glGetUniformLocation(axisShader->Program, "axisLength"), axis_i->tickLength);//刻度的长度
            glUniform2f(glGetUniformLocation(resource->rectShader->Program, "size"),
                textInfo->range.x * windowTextScale.x + textInfo->range.y * windowTextScale.y * 0.2,
                textInfo->range.y * windowTextScale.y *1.2);
            // 定义一个点的位置
            GLfloat point[] = { textInfo->pos.x - textInfo->range.x * windowTextScale.x/2 - textInfo->range.y * windowTextScale.y * 0.1,
                textInfo->pos.y - textInfo->range.y * windowTextScale.y * 0.6,textInfo->pos.z }; // (0, 0) 处的点

            // 绘制点
            glPointSize(1.0f); // 设置点的大小
            glVertexPointer(2, GL_FLOAT, 0, point); // 设置顶点指针
            glEnableClientState(GL_VERTEX_ARRAY); // 启用顶点数组
            glDrawArrays(GL_POINTS, 0, 1); // 绘制单个点
            glDisableClientState(GL_VERTEX_ARRAY); // 禁用顶点数组

            glFlush();
        }
    }
}
void GLFigure3D::addTextInfo(std::wstring text, glm::vec3 pos, GLfloat scale, glm::vec3 color) {
    TextInfo* textInfo = new TextInfo();
    textInfo->text = text;
    textInfo->pos = pos;
    textInfo->scale = scale;
    textInfo->color = color;
    //textInfo->range = getStringSize(text, scale);//放在paintTextList中进行初始化了。
    textInfoList.push_back(textInfo);
};
void GLFigure3D::setTextInfo(int index, std::wstring text, glm::vec3 pos, GLfloat scale, glm::vec3 color) {
    assert(index < this->textInfoList.size());
    TextInfo* textInfo =this->textInfoList.at(index);
    textInfo->text = text;
    textInfo->pos = pos;
    textInfo->scale = scale;
    textInfo->color = color;
}
void GLFigure3D::removeTextInfo(TextInfo* textInfo) {
    std::remove(textInfoList.begin(), textInfoList.end(), textInfo);
    textInfoList.pop_back();//push_back的作用？
    delete textInfo;
}

int GLFigure3D::getDecimal(float value) {//只能获取小数点后面的。如果没有则为0。
    int decimal = 0;
    QString numStr = QString::number(value);
    auto esplit = numStr.split("e");
    if (esplit.size() > 1) {
        numStr = esplit.at(0);
        decimal += -esplit.at(1).toInt();
    }
    auto dsplit = numStr.split(".");
    if (dsplit.size() > 1) {
        decimal += dsplit.at(1).size();
    }
    return decimal;
}
float GLFigure3D::cutNumber(float value, float range, int side) {
    //side为0表示向小截断，为1表示向大截断。
    int e; float n; float to_num; int decimal = 0;
    if (side == 0) {
        e = floor(std::log10(abs(value)));//lower的尺度。如果太接近0可能会出问题
        n = value / pow(10, e);//为1-10之间的数。或者-1到-10之间
        to_num = floor(n) * pow(10, e);//to_num此时必定小于lower
        while (value - to_num >= range) {
            decimal += 1;
            to_num = floor(n * pow(10, decimal)) * pow(10, e - decimal);//看看截断axis_i->interval[0]多少位合适。截断余数不能超过区间%10。
        };
        value = to_num;
    }
    else {
        //截断upper
        e = floor(std::log10(abs(value)));//upper的尺度。如果太接近0可能会出问题
        n = value / pow(10, e);//为1-10之间的数。或者-1到-10之间
        to_num = (floor(n) + 1) * pow(10, e);//to_num此时必定大于upper
        while (to_num - value >= range) {
            decimal += 1;
            to_num = (floor(n * pow(10, decimal)) + 1) * pow(10, e - decimal);
        };
        value = to_num;
    }
    return value;


}

glm::vec2 GLFigure3D::getStringSize(std::wstring text, GLfloat scale) {
    std::wstring::const_iterator c;
    std::wstring::const_iterator::value_type zero = *"0";
    GLfloat string_height = resource->characters[zero].Size.y * scale;//以0的高度中心作为字符高度中心的基准
    GLfloat string_width = 0;
    //for (c = text.begin(); c != text.end(); c++)
    //{
    //    auto it = characters.find(*c);
    //    if (it == characters.end()) {
    //        if (*c > 0x7F) {
    //            loadUnicodeCharacters(text.c_str());
    //        }
    //        else {
    //            loadASCIICharacters(text.c_str());
    //        }
    //        it = characters.find(*c);
    //    }
    //    Character ch = it->second;
    //    string_width += (ch.Advance >> 6) * scale;
    //}
    for (int i = 0; i < wcslen(text.c_str()); i++) {
        Character ch = resource->findCharacter(text.c_str(), i);
        string_width += (ch.Advance >> 6) * scale;
    }
    return glm::vec2(string_width, string_height);
}
GLFigure3D::Range3D GLFigure3D::getDataBlockInfoListRange() {
    Range3D total_range;
    for (auto it = dataBlockInfoList.begin(); it != dataBlockInfoList.end(); it++) {
        if (!total_range.init) {
            for (int i = 0; i < 3; i++) {
                total_range.maximum[i] = (*it)->dataRange.maximum[i];
                total_range.minimum[i] = (*it)->dataRange.minimum[i];
            }
            total_range.init = true;
        }
        else {
            for (int i = 0; i < 3; i++) {
                if (total_range.maximum[i] < (*it)->dataRange.maximum[i]) {
                    total_range.maximum[i] = (*it)->dataRange.maximum[i];
                }
                if (total_range.minimum[i] > (*it)->dataRange.minimum[i]) {
                    total_range.minimum[i] = (*it)->dataRange.minimum[i];
                }
            }
        }
    }
    
    return total_range;
}
GLFigure3D::Range3D GLFigure3D::getBoxRange(glm::mat4 view, float x_range[2], float y_range[2], float z_range[2]) {
    Range3D range;
    for (int i = 0; i < 2;i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                glm::vec4 point = glm::vec4(x_range[i],
                    y_range[j],
                    z_range[k], 1.0f);
                glm::vec3 translated_point = view * point;
                if (!range.init) {
                    for (int u = 0; u < 3; u++) {
                        range.maximum[u] = translated_point[u];
                        range.minimum[u] = translated_point[u];
                    }
                    range.init = true;
                }
                //qDebug() << (view * point).w;
                for (int u = 0; u < 3; u++) {//更新坐标轴范围
                    if (translated_point[u] < range.minimum[u]) {
                        range.minimum[u] = translated_point[u];
                    }
                    if (translated_point[u] > range.maximum[u]) {
                        range.maximum[u] = translated_point[u];
                    }
                }
            }
        }
    }
    return range;
}
//不明作用
void GLFigure3D::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
}

//鼠标控制事件
void GLFigure3D::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        lastX = event->pos().x();
        lastY = event->pos().y();
        info.lastClickPosition.x = lastX;
        info.lastClickPosition.y = lastY;
        leftButtonPressed = true;

        //clear selectedInfo
        selectedInfo.clearSelectedInfo();
        
        float lastXInFigure = lastX*1.0 / this->width()*2-1;//lastX * figureWidth / this->width() - figureWidth/2;
        float lastYInFigure = 1-lastY*1.0 / this->height() * 2;//figureHeight/2 - lastY * figureHeight / this->height();
        //for (auto i = 0; i < 3; i++) {
        //    TextInfo* textInfo = &axis_list[i]->label;
        //    if (lastXInFigure >= textInfo->pos.x && lastXInFigure < textInfo->pos.x + textInfo->range.x * windowTextScale.x &&
        //        lastYInFigure >= textInfo->pos.y && lastYInFigure < textInfo->pos.y + textInfo->range.y * windowTextScale.y && (selectedInfo.selectedText==nullptr)) {

        //        textInfo->selected = true;
        //        selectedInfo.selectedText = textInfo;
        //        selectedInfo.isAxisLabel = true;
        //        selectedInfo.axisLabelId = i;
        //        break;//即只能选中一个。
        //    }
        //}
        for (auto it= textInfoList.begin(); it != textInfoList.end(); it++) {
            TextInfo* textInfo = *it;
            qDebug() << lastXInFigure<<":"<< lastYInFigure << "," << textInfo->pos.x << ":" << textInfo->pos.y <<","<< textInfo->range.x<<":"<< textInfo->range.y;
            if (lastXInFigure >= textInfo->pos.x - textInfo->range.x * windowTextScale.x / 2 && lastXInFigure < textInfo->pos.x + textInfo->range.x * windowTextScale.x/2 &&
                lastYInFigure >= textInfo->pos.y - textInfo->range.y * windowTextScale.y / 2 && lastYInFigure < textInfo->pos.y + textInfo->range.y * windowTextScale.y/2
                && (selectedInfo.selectedText == nullptr)) {

                textInfo->selected = true;
                selectedInfo.selectedText = textInfo;
                break;//即只能选中一个。
            }
        }
        this->update();
    }
    if (event->button() == Qt::MidButton) {
        lastX = event->pos().x();
        lastY = event->pos().y();
        middleButtonPressed = true;
    }
    
}
void GLFigure3D::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        leftButtonPressed = false;
    }
    if (event->button() == Qt::MidButton) {
        middleButtonPressed = false;
    }
}

/*默认情况下，QWidget 只在按下鼠标按钮时接收 mouseMoveEvent 事件。要在没有按下鼠标按钮时也能接收到移动事件，需要调用 setMouseTracking(true)。*/
void GLFigure3D::mouseMoveEvent(QMouseEvent* event)
{
    if (leftButtonPressed)
    {
        if (selectedInfo.selectedText != nullptr) {//有文字被选中，变换文字位置
            
            GLfloat xoffset = lastX - event->pos().x();
            GLfloat yoffset = lastY - event->pos().y();  // Reversed since y-coordinates go from bottom to left
            lastX = event->pos().x();
            lastY = event->pos().y();
            GLfloat xoffsetInFigure = xoffset / this->width() * 2;//xoffset * figureWidth / this->width();
            GLfloat yoffsetInFigure = yoffset / this->height() * 2;//yoffset * figureHeight / this->height();
            selectedInfo.selectedText->pos.x -= xoffsetInFigure;
            selectedInfo.selectedText->pos.y += yoffsetInFigure;
            
        }
        else {//没有文字被选中，变换视角
            //setCursor(Qt::CrossCursor);
            GLfloat xoffset = lastX - event->pos().x();
            GLfloat yoffset = lastY - event->pos().y();  // Reversed since y-coordinates go from bottom to left

            lastX = event->pos().x();
            lastY = event->pos().y();

            glm::vec3 target;
            for (int i = 0; i < 3; i++) {//更新坐标轴范围
                Axis* axis_i = axis_list[i];
                target[i] = (axis_i->interval[0] + axis_i->interval[1]) / 2/ (axis_i->interval[1] - axis_i->interval[0]);
            }
            camera->ProcessAttitudeMovementByPoint(xoffset, yoffset, true, target);
            //setCursor(Qt::CrossCursor);
        }
        this->update();
    }
    else if (middleButtonPressed) {
        //setCursor(Qt::CrossCursor);
        GLfloat xoffset = lastX - event->pos().x();
        GLfloat yoffset = event->pos().y() - lastY;  // Reversed since y-coordinates go from bottom to left

        lastX = event->pos().x();
        lastY = event->pos().y();

        camera->ProcessPositionMovement(xoffset, yoffset);
    }
}
void GLFigure3D::wheelEvent(QWheelEvent* event) {
    camera->ProcessMouseScroll(event->angleDelta().y());
    this->update();
}