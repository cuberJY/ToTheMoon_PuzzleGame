#include "ChooseDiff.h"
#include "MainWindow.h"
#include "Tool.h"
#include "ui_ChooseDiff.h"
#include <QTimer>

//构造函数，初始化难度选择界面
ChooseDiff::ChooseDiff(MainWindow *parent)
    : QWidget(parent)//调用父类构造函数
    , ui(new Ui::ChooseDiff)//分配内存给UI对象
    , mainWindow(parent)
{
    ui->setupUi(this);

    //设置背景图片
    QString bgPath = Tool::getImgPath("background/backgroundHome.png");
    Tool::setBackground(this, bgPath);
    //设置按钮样式
    ui->easyButton->setStyleSheet(Tool::chooseButtonStyle);
    ui->normalButton->setStyleSheet(Tool::chooseButtonStyle);
    ui->hardButton->setStyleSheet(Tool::chooseButtonStyle);
    ui->hardcoreButton->setStyleSheet(Tool::chooseButtonStyle);
    //设置标签样式
    ui->introduction1->setStyleSheet(Tool::LabelStyle);
    ui->introduction2->setStyleSheet(Tool::LabelStyle);
    //连接按钮点击信号槽
    connect(ui->easyButton, &QPushButton::clicked, this, [this]() {onDiffButtonClicked(Diff::easy);});
    connect(ui->normalButton, &QPushButton::clicked, this, [this]() {onDiffButtonClicked(Diff::normal);});
    connect(ui->hardButton, &QPushButton::clicked, this, [this]() {onDiffButtonClicked(Diff::hard);});
    connect(ui->hardcoreButton, &QPushButton::clicked, this, [this]() {onDiffButtonClicked(Diff::hardcore);});

    setTabOrder(ui->easyButton, ui->normalButton);
    setTabOrder(ui->normalButton, ui->hardButton);
    setTabOrder(ui->hardButton, ui->hardcoreButton);
}

ChooseDiff::~ChooseDiff()
{
    delete ui;
}
//难度按钮点击槽函数
void ChooseDiff::onDiffButtonClicked(Diff diff){
    QPushButton* clickedBtn = nullptr;
    switch (diff){//根据难度选择按钮
        case Diff::easy: clickedBtn = ui->easyButton; break;
        case Diff::normal: clickedBtn = ui->normalButton; break;
        case Diff::hard: clickedBtn = ui->hardButton; break;
        case Diff::hardcore: clickedBtn = ui->hardcoreButton; break;
    }
    if(clickedBtn){
        Tool::clickedAnimation(clickedBtn, mainWindow->getIsAnimation(), Tool::chooseClickedButtonStyle);
    }
    //设置按钮延迟时间，等待动画播放完成
    int delayTime;
    if (mainWindow->getIsAnimation()) delayTime = 225;
    else delayTime = 0;
    QTimer::singleShot(delayTime, this, [this, diff](){
        emit difficultySelected(diff);
    });
}
