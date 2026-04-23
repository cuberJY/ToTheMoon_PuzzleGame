#include "ChooseDiff.h"
#include "MainWindow.h"
#include "Tool.h"
#include "ui_ChooseDiff.h"
#include <QDir>
#include <QColor>
#include <QTimer>

//构造函数，初始化难度选择界面
ChooseDiff::ChooseDiff(MainWindow *parent)
    : QWidget(parent)//调用父类构造函数
    , ui(new Ui::ChooseDiff)//分配内存给UI对象
    , mainWindow(parent)
{
    ui->setupUi(this);

    //设置背景图片
    QString bgPath = Tool::getImgPath("background/backgroundRandom.png");
    Tool::setBackground(this, bgPath);

    ui->easyButton->setStyleSheet(chooseButtonStyle);
    ui->normalButton->setStyleSheet(chooseButtonStyle);
    ui->hardButton->setStyleSheet(chooseButtonStyle);
    ui->hardcoreButton->setStyleSheet(chooseButtonStyle);

    ui->introduction1->setStyleSheet(LabelStyle);
    ui->introduction2->setStyleSheet(LabelStyle);

    connect(ui->easyButton, &QPushButton::clicked, this, [this]() {onDiffButtonClicked(ModeManager::diff::easy);});
    connect(ui->normalButton, &QPushButton::clicked, this, [this]() {onDiffButtonClicked(ModeManager::diff::normal);});
    connect(ui->hardButton, &QPushButton::clicked, this, [this]() {onDiffButtonClicked(ModeManager::diff::hard);});
    connect(ui->hardcoreButton, &QPushButton::clicked, this, [this]() {onDiffButtonClicked(ModeManager::diff::hardcore);});
}

ChooseDiff::~ChooseDiff()
{
    delete ui;
}

void ChooseDiff::onDiffButtonClicked(ModeManager::diff diff){
    QPushButton* clickedBtn = nullptr;
    switch (diff){
        case ModeManager::diff::easy: clickedBtn = ui->easyButton; break;
        case ModeManager::diff::normal: clickedBtn = ui->normalButton; break;
        case ModeManager::diff::hard: clickedBtn = ui->hardButton; break;
        case ModeManager::diff::hardcore: clickedBtn = ui->hardcoreButton; break;
    }
    if(clickedBtn){
        Tool::clickedAnimation(clickedBtn, mainWindow->getIsAnimation(), chooseClickedButtonStyle);
    }

    int delayTime;
    if (mainWindow->getIsAnimation()) delayTime = 225;
    else delayTime = 0;
    QTimer::singleShot(delayTime, this, [this, diff](){
        emit difficultySelected(diff);
    });
}
