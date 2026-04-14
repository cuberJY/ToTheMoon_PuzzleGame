#include "choosediff.h"
#include "ui_choosediff.h"

#include <QPixmap>
#include <QDir>
#include <QColor>

QString getPath(const QString& filename) {
    //获取应用程序可执行文件所在目录
    QString appDir = QCoreApplication::applicationDirPath();
    QString resourcesPath = appDir + "/../.."  + "/resources/";

    return resourcesPath + filename;
}

//设置背景图片
void setBackground(QWidget* widget, const QString& imgPath){
    QPixmap pixmap(imgPath);
    if (!pixmap.isNull()){
        // 创建一个与窗口大小匹配的新 pixmap
        QPixmap scaledPixmap = pixmap.scaled(widget->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        
        QPalette palette;
        // 使用 scaledPixmap 作为背景，并设置为 NoRepeat
        palette.setBrush(QPalette::Window, QBrush(scaledPixmap));
        widget->setPalette(palette);
        widget->setAutoFillBackground(true);
    }
}

//构造函数，初始化难度选择界面
ChooseDiff::ChooseDiff(QWidget *parent)
    : QWidget(parent)//调用父类构造函数
    , ui(new Ui::ChooseDiff)//分配内存给UI对象
{
    ui->setupUi(this);

    //设置背景图片
    QString bgPath = getPath("backgroundRandom.png");
    setBackground(this, bgPath);

    //使用lambda表达式连接按钮信号
    connect(ui->easyButton, &QPushButton::clicked, this, [this]() { emit difficultySelected(0); });
    connect(ui->normalButton, &QPushButton::clicked, this, [this]() { emit difficultySelected(1); });
    connect(ui->hardButton, &QPushButton::clicked, this, [this]() { emit difficultySelected(2); });
    connect(ui->hardcoreButton, &QPushButton::clicked, this, [this]() { emit difficultySelected(3); });

    ui->easyButton->setStyleSheet("background-color: rgba(255, 255, 255, 0.05); color: rgba(255, 255, 255, 0.7); font-family:'Century Schoolbook'; font-size: 32px;");
    ui->normalButton->setStyleSheet("background-color: rgba(255, 255, 255, 0.05); color: rgba(255, 255, 255, 0.7); font-family:'Century Schoolbook'; font-size: 32px;");
    ui->hardButton->setStyleSheet("background-color: rgba(255, 255, 255, 0.05); color: rgba(255, 255, 255, 0.7); font-family:'Century Schoolbook'; font-size: 32px;");
    ui->hardcoreButton->setStyleSheet("background-color: rgba(255, 255, 255, 0.05); color: rgba(255, 255, 255, 0.7); font-family:'Century Schoolbook'; font-size: 32px;");

    //设置标签样式
    ui->introduction1->setStyleSheet("color: rgba(255, 255, 255, 0.7); font-family:'Century Schoolbook'; font-size: 8px;");
    ui->introduction2->setStyleSheet("color: rgba(255, 255, 255, 0.7); font-family:'Century Schoolbook'; font-size: 8px;");
}

//析构函数，释放资源
ChooseDiff::~ChooseDiff()
{
    delete ui;
}
