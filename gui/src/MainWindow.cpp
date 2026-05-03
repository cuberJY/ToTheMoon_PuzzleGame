#include "MainWindow.h"
#include "Tool.h"
#include <QTimer>
#include <memory>

//构造函数，初始化主窗口
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , isBgmPlaying(true)
    , isEffectPlaying(true)
    , isAnimation(true)
    , settingsMenuExpanded(false)
    , settingsOverlay(nullptr)
{
    controller = new BoardController();
    //创建堆叠窗口，用于界面切换
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    //创建难度选择界面和游戏棋盘界面
    homeWidget = new Home(this);
    chooseLevelWidget = new ChooseLevel(this);
    chooseDiffWidget = new ChooseDiff(this);
    boardWidget = new BoardView(this);
    boardWidget->setController(controller);

    //将界面添加到堆叠窗口
    stackedWidget->addWidget(homeWidget);
    stackedWidget->addWidget(chooseLevelWidget);
    stackedWidget->addWidget(chooseDiffWidget);
    stackedWidget->addWidget(boardWidget);

    //初始显示界面
    stackedWidget->setCurrentWidget(homeWidget);

    //连接信号和槽
    connect(homeWidget, &Home::randomModeSelected, this, &MainWindow::onRandomModeSelected);
    connect(homeWidget, &Home::levelModeSelected, this, &MainWindow::onLevelModeSelected);
    connect(chooseLevelWidget, &ChooseLevel::levelSelected, this, &MainWindow::onLevelSelected);
    connect(chooseDiffWidget, &ChooseDiff::difficultySelected, this, &MainWindow::onDifficultySelected);
    connect(boardWidget, &BoardView::backToMenu, this, &MainWindow::onBackToMenu);
    //初始化设置按钮
    initialSettingsButtons();

    setWindowTitle("To The Moon Puzzle Game");
    setFixedSize(800, 600);

    //播放背景音乐
    Tool::playAudio(Tool::getAudioPath("bgm/For River Piano(Johnny's Version).mp3"), Tool::bgm, true);
}

MainWindow::~MainWindow(){
    delete controller;
}

//获取是否正在播放背景音乐
bool MainWindow::getIsBgmPlaying() const{
    return isBgmPlaying;
}
//设置是否正在播放背景音乐
void MainWindow::setIsBgmPlaying(bool value){
    isBgmPlaying = value;
}
//获取是否正在播放音效
bool MainWindow::getIsEffectPlaying() const{
    return isEffectPlaying;
}
//设置是否正在播放音效
void MainWindow::setIsEffectPlaying(bool value){
    isEffectPlaying = value;
}
//获取是否正在动画中
bool MainWindow::getIsAnimation() const{
    return isAnimation;
}
//设置是否正在动画中
void MainWindow::setIsAnimation(bool value){
    isAnimation = value;
}

int MainWindow::getPlayerMaxLevel() const{
    return controller->getPlayerMaxLevel();
}

BoardController* MainWindow::getController() const{
    return controller;
}

//模式选择槽函数：切换到难度选择界面
void MainWindow::onRandomModeSelected(){
    hideSettingsMenu();
    settingsMenuExpanded = false;

    stackedWidget->setCurrentWidget(chooseDiffWidget);
}
//模式选择槽函数：切换到关卡选择界面
void MainWindow::onLevelModeSelected(){
    hideSettingsMenu();
    settingsMenuExpanded = false;

    chooseLevelWidget->refresh();
    stackedWidget->setCurrentWidget(chooseLevelWidget);
}
//难度选择槽函数：切换到游戏棋盘界面
void MainWindow::onDifficultySelected(Diff diff){
    hideSettingsMenu();
    settingsMenuExpanded = false;

    RandomMode* randomMode = new RandomMode(diff);
    boardWidget->setGameMode(randomMode);
    boardWidget->setDiff(diff);
    boardWidget->startGame();
    stackedWidget->setCurrentWidget(boardWidget);//切换到棋盘界面
    Tool::setAudioVolume(Tool::bgm, 30);
}
//关卡选择槽函数：切换到游戏棋盘界面
void MainWindow::onLevelSelected(int level){
    hideSettingsMenu();
    settingsMenuExpanded = false;

    LevelMode* levelMode = new LevelMode(level);
    boardWidget->setGameMode(levelMode);
    boardWidget->startGame();
    stackedWidget->setCurrentWidget(boardWidget);
    Tool::setAudioVolume(Tool::bgm, 30);
}

//返回菜单槽函数：切换回上一级界面
void MainWindow::onBackToMenu(){
    hideSettingsMenu();
    settingsMenuExpanded = false;
    
    if (boardWidget->getCurrentMode() == ModeType::Random){
        stackedWidget->setCurrentWidget(chooseDiffWidget);
    }
    else if (boardWidget->getCurrentMode() == ModeType::Level){
        chooseLevelWidget->refresh();
        stackedWidget->setCurrentWidget(chooseLevelWidget);
    }
    Tool::setAudioVolume(Tool::bgm, 50);
}
//返回主页按钮点击槽函数
void MainWindow::on_backButton_clicked(){
    Tool::clickedAnimation(backButton, isAnimation, Tool::settingsClickedButtonStyle);
    stackedWidget->setCurrentWidget(homeWidget);
}
//设置按钮点击槽函数
void MainWindow::on_settingsBtn_clicked(){
    Tool::clickedAnimation(settingsBtn, isAnimation, Tool::settingsClickedButtonStyle);
    if (!settingsMenuExpanded){
        showSettingsMenu();
    }
    else{
        hideSettingsMenu();
    }
    settingsMenuExpanded = !settingsMenuExpanded;
}
//BGM按钮点击槽函数
void MainWindow::on_bgmBtn_clicked(){
    if (isBgmPlaying){
        bgmBtn->setText("BGM\n关闭");
        bgmBtn->setStyleSheet(Tool::settingsOffButtonStyle);
        Tool::pauseAudio(Tool::bgm);
    }
    else{
        bgmBtn->setText("BGM\n开启");
        bgmBtn->setStyleSheet(Tool::settingsOnButtonStyle);
        Tool::resumeAudio(Tool::bgm);
    }
    isBgmPlaying = !isBgmPlaying;
}
//音效按钮点击槽函数
void MainWindow::on_effectBtn_clicked(){
    if (isEffectPlaying){
        effectBtn->setText("音效\n关闭");
        effectBtn->setStyleSheet(Tool::settingsOffButtonStyle);
    }
    else{
        effectBtn->setText("音效\n开启");
        effectBtn->setStyleSheet(Tool::settingsOnButtonStyle);
    }
    isEffectPlaying = !isEffectPlaying;
}
//特效按钮点击槽函数
void MainWindow::on_animationBtn_clicked(){
    if (isAnimation){
        animationBtn->setText("特效\n关闭");
        animationBtn->setStyleSheet(Tool::settingsOffButtonStyle);
    }
    else{
        animationBtn->setText("特效\n开启");
        animationBtn->setStyleSheet(Tool::settingsOnButtonStyle);
    }
    isAnimation = !isAnimation;
}
//重置按钮点击槽函数
void MainWindow::on_resetBtn_clicked(){
    if (isAnimation){
        Tool::clickedAnimation(resetBtn, isAnimation, Tool::settingsClickedButtonStyle);
    }
    controller->resetData();
    if (!boardWidget) return;
    if (boardWidget->getCurrentMode() == ModeType::Level){
        chooseLevelWidget->refresh();
    }
    else if (boardWidget->getCurrentMode() == ModeType::Random){
        boardWidget->updateStreakDisplay();
    }
}
//初始化设置按钮
void MainWindow::initialSettingsButtons(){
    //设置按钮
    settingsBtn = new QPushButton(this);
    settingsBtn->setGeometry(750, 10, 40, 40);//设置按钮位置
    settingsBtn->setText("设置");//设置按钮文本
    settingsBtn->setStyleSheet(Tool::menuButtonStyle);//设置按钮样式
    settingsBtn->show();
    connect(settingsBtn, &QPushButton::clicked, this, &MainWindow::on_settingsBtn_clicked);//连接信号和槽
    //返回主页按钮
    backButton = new QPushButton(this);
    backButton->setGeometry(750, 54, 40, 40);
    backButton->setText("返回\n主页");
    backButton->setStyleSheet(Tool::menuButtonStyle);
    backButton->show();
    connect(backButton, &QPushButton::clicked, this, &MainWindow::on_backButton_clicked);
    //BGM按钮
    bgmBtn = new QPushButton(this);
    bgmBtn->setGeometry(750, 54, 40, 40);
    bgmBtn->setText("BGM\n开启");
    bgmBtn->setStyleSheet(Tool::settingsOnButtonStyle);
    bgmBtn->setVisible(false);
    connect(bgmBtn, &QPushButton::clicked, this, &MainWindow::on_bgmBtn_clicked);
    //音效按钮
    effectBtn = new QPushButton(this);
    effectBtn->setGeometry(750, 98, 40, 40);
    effectBtn->setText("音效\n开启");
    effectBtn->setStyleSheet(Tool::settingsOnButtonStyle);
    effectBtn->setVisible(false);
    connect(effectBtn, &QPushButton::clicked, this, &MainWindow::on_effectBtn_clicked);
    //特效按钮
    animationBtn = new QPushButton(this);
    animationBtn->setGeometry(750, 142, 40, 40);
    animationBtn->setText("特效\n开启");
    animationBtn->setStyleSheet(Tool::settingsOnButtonStyle);
    animationBtn->setVisible(false);
    connect(animationBtn, &QPushButton::clicked, this, &MainWindow::on_animationBtn_clicked);
    //重置按钮
    resetBtn = new QPushButton(this);
    resetBtn->setGeometry(750, 186, 40, 40);
    resetBtn->setText("重置");
    resetBtn->setStyleSheet(Tool::settingsOnButtonStyle);
    resetBtn->setVisible(false);
    connect(resetBtn, &QPushButton::clicked, this, &MainWindow::on_resetBtn_clicked);
}
//显示设置菜单
void MainWindow::showSettingsMenu(){
    //设置按钮延迟时间，等待动画播放完成
    int delayTime;
    if (isAnimation) delayTime = 250;
    else delayTime = 50;
    QTimer::singleShot(delayTime, this, [=](){
        backButton->setVisible(false);
        bgmBtn->setVisible(true);
        effectBtn->setVisible(true);
        animationBtn->setVisible(true);
        resetBtn->setVisible(true);
    });
    //设置遮罩排除区域
    QRegion excludedRegion;
    QRect settingsBtnRect = QRect(settingsBtn->mapTo(this, QPoint(0, 0)), settingsBtn->size()).adjusted(2, 2, -2, -2);//设置按钮区域
    QRect bgmBtnRect = QRect(bgmBtn->mapTo(this, QPoint(0, 0)), bgmBtn->size()).adjusted(2, 2, -2, -2);//BGM按钮区域
    QRect effectBtnRect = QRect(effectBtn->mapTo(this, QPoint(0, 0)), effectBtn->size()).adjusted(2, 2, -2, -2);//音效按钮区域
    QRect animationBtnRect = QRect(animationBtn->mapTo(this, QPoint(0, 0)), animationBtn->size()).adjusted(2, 2, -2, -2);//特效按钮区域
    QRect resetBtnRect = QRect(resetBtn->mapTo(this, QPoint(0, 0)), resetBtn->size()).adjusted(2, 2, -2, -2);//重置按钮区域
    excludedRegion += settingsBtnRect;
    excludedRegion += bgmBtnRect;
    excludedRegion += effectBtnRect;
    excludedRegion += animationBtnRect;
    excludedRegion += resetBtnRect;
    //创建遮罩层
    if (!settingsOverlay){
        QTimer::singleShot(delayTime-20, this, [=](){
            QRegion mask = QRegion(rect()) - excludedRegion;//创建遮罩区域
            settingsOverlay = Tool::createOverlay(this, rect(), "background-color: #000000;", mask);
            settingsOverlay->setAttribute(Qt::WA_TransparentForMouseEvents, false);//设置遮罩层区域不响应鼠标事件
            settingsOverlay->show();//显示遮罩层

            QGraphicsOpacityEffect* opacityEffect = static_cast<QGraphicsOpacityEffect*>(settingsOverlay->graphicsEffect());//获取遮罩层透明度效果
            QPropertyAnimation* anim = Tool::createOpacityAnimation(opacityEffect, 0.0, 0.5, 100, QEasingCurve::InQuad).release();//创建透明度动画
            anim->start(QAbstractAnimation::DeleteWhenStopped);//启动动画
        });
    }
}
//隐藏设置菜单
void MainWindow::hideSettingsMenu(){
    //设置按钮延迟时间，等待动画播放完成
    int delayTime;
    if (isAnimation) delayTime = 200;
    else delayTime = 50;
    QTimer::singleShot(delayTime, this, [=](){
        backButton->setVisible(true);
        bgmBtn->setVisible(false);
        effectBtn->setVisible(false);
        animationBtn->setVisible(false);
        resetBtn->setVisible(false);
    });
    //隐藏遮罩层
    if (settingsOverlay){
        QGraphicsOpacityEffect* opacityEffect = static_cast<QGraphicsOpacityEffect*>(settingsOverlay->graphicsEffect());
        QPropertyAnimation* anim = Tool::createOpacityAnimation(opacityEffect, 0.5, 0.0, 300, QEasingCurve::OutQuad).release();
        connect(anim, &QPropertyAnimation::finished, settingsOverlay, &QWidget::deleteLater);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
        settingsOverlay = nullptr;
    }
}