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
    //创建堆叠窗口，用于界面切换
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    //创建难度选择界面和游戏棋盘界面
    chooseDiffWidget = new ChooseDiff(this);
    boardWidget = new BoardView(this);

    //将两个界面添加到堆叠窗口
    stackedWidget->addWidget(chooseDiffWidget);
    stackedWidget->addWidget(boardWidget);

    //初始显示难度选择界面
    stackedWidget->setCurrentWidget(chooseDiffWidget);

    //连接信号和槽
    connect(chooseDiffWidget, &ChooseDiff::difficultySelected, this, &MainWindow::onDifficultySelected);
    connect(boardWidget, &BoardView::backToMenu, this, &MainWindow::onBackToMenu);

    initialSettingsButtons();

    setWindowTitle("To The Moon Puzzle Game");
    resize(800, 600);

    //播放背景音乐
    Tool::playAudio(Tool::getAudioPath("bgm/For River Piano(Johnny's Version).mp3"), Tool::bgm, true);
}

MainWindow::~MainWindow(){
    delete settingsBtn;
    delete bgmBtn;
    delete effectBtn;
    delete animationBtn;
}

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

//难度选择槽函数：切换到游戏棋盘界面
void MainWindow::onDifficultySelected(ModeManager::diff diff){
    hideSettingsMenu();
    settingsMenuExpanded = false;
    
    boardWidget->setDifficulty(diff);//设置难度
    boardWidget->startGame();//开始游戏
    stackedWidget->setCurrentWidget(boardWidget);//切换到棋盘界面
    Tool::setAudioVolume(Tool::bgm, 30);
}
//返回菜单槽函数：切换回难度选择界面
void MainWindow::onBackToMenu(){
    hideSettingsMenu();
    settingsMenuExpanded = false;
    
    stackedWidget->setCurrentWidget(chooseDiffWidget);
    Tool::setAudioVolume(Tool::bgm, 50);
}
void MainWindow::on_settingsBtn_clicked(){
    Tool::clickedAnimation(settingsBtn, isAnimation, settingsClickedButtonStyle);
    if (!settingsMenuExpanded){
        showSettingsMenu();
    }
    else{
        hideSettingsMenu();
    }
    settingsMenuExpanded = !settingsMenuExpanded;
}

void MainWindow::on_bgmBtn_clicked(){
    if (isBgmPlaying){
        bgmBtn->setText("bgm\n关闭");
        bgmBtn->setStyleSheet(settingsOffButtonStyle);
        Tool::pauseAudio(Tool::bgm);
    }
    else{
        bgmBtn->setText("bgm\n开启");
        bgmBtn->setStyleSheet(settingsOnButtonStyle);
        Tool::resumeAudio(Tool::bgm);
    }
    isBgmPlaying = !isBgmPlaying;
}

void MainWindow::on_effectBtn_clicked(){
    if (isEffectPlaying){
        effectBtn->setText("音效\n关闭");
        effectBtn->setStyleSheet(settingsOffButtonStyle);
    }
    else{
        effectBtn->setText("音效\n开启");
        effectBtn->setStyleSheet(settingsOnButtonStyle);
    }
    isEffectPlaying = !isEffectPlaying;
}

void MainWindow::on_animationBtn_clicked(){
    if (isAnimation){
        animationBtn->setText("特效\n关闭");
        animationBtn->setStyleSheet(settingsOffButtonStyle);
    }
    else{
        animationBtn->setText("特效\n开启");
        animationBtn->setStyleSheet(settingsOnButtonStyle);
    }
    isAnimation = !isAnimation;
}

void MainWindow::initialSettingsButtons(){
    settingsBtn = new QPushButton(this);
    settingsBtn->setGeometry(750, 10, 40, 40);
    settingsBtn->setText("设置");
    settingsBtn->setStyleSheet(settingsButtonStyle);
    settingsBtn->show();
    connect(settingsBtn, &QPushButton::clicked, this, &MainWindow::on_settingsBtn_clicked);

    bgmBtn = new QPushButton(this);
    bgmBtn->setGeometry(750, 54, 40, 40);
    bgmBtn->setText("bgm\n开启");
    bgmBtn->setStyleSheet(settingsOnButtonStyle);
    bgmBtn->setVisible(false);
    connect(bgmBtn, &QPushButton::clicked, this, &MainWindow::on_bgmBtn_clicked);

    effectBtn = new QPushButton(this);
    effectBtn->setGeometry(750, 98, 40, 40);
    effectBtn->setText("音效\n开启");
    effectBtn->setStyleSheet(settingsOnButtonStyle);
    effectBtn->setVisible(false);
    connect(effectBtn, &QPushButton::clicked, this, &MainWindow::on_effectBtn_clicked);

    animationBtn = new QPushButton(this);
    animationBtn->setGeometry(750, 142, 40, 40);
    animationBtn->setText("特效\n开启");
    animationBtn->setStyleSheet(settingsOnButtonStyle);
    animationBtn->setVisible(false);
    connect(animationBtn, &QPushButton::clicked, this, &MainWindow::on_animationBtn_clicked);
}

void MainWindow::showSettingsMenu(){

    int delayTime;
    if (isAnimation) delayTime = 250;
    else delayTime = 50;
    QTimer::singleShot(delayTime, this, [=](){
        bgmBtn->setVisible(true);
        effectBtn->setVisible(true);
        animationBtn->setVisible(true);
    });

    QRegion excludedRegion;
    QRect settingsBtnRect = QRect(settingsBtn->mapTo(this, QPoint(0, 0)), settingsBtn->size()).adjusted(2, 2, -2, -2);
    QRect bgmBtnRect = QRect(bgmBtn->mapTo(this, QPoint(0, 0)), bgmBtn->size()).adjusted(2, 2, -2, -2);
    QRect effectBtnRect = QRect(effectBtn->mapTo(this, QPoint(0, 0)), effectBtn->size()).adjusted(2, 2, -2, -2);
    QRect animationBtnRect = QRect(animationBtn->mapTo(this, QPoint(0, 0)), animationBtn->size()).adjusted(2, 2, -2, -2);
    excludedRegion += settingsBtnRect;
    excludedRegion += bgmBtnRect;
    excludedRegion += effectBtnRect;
    excludedRegion += animationBtnRect;

    if (!settingsOverlay){
        QTimer::singleShot(delayTime-20, this, [=](){
            QRegion mask = QRegion(rect()) - excludedRegion;
            settingsOverlay = Tool::createOverlay(this, rect(), "background-color: #000000;", mask);
            settingsOverlay->setAttribute(Qt::WA_TransparentForMouseEvents, false);
            settingsOverlay->show();

            QGraphicsOpacityEffect* opacityEffect = static_cast<QGraphicsOpacityEffect*>(settingsOverlay->graphicsEffect());
            QPropertyAnimation* anim = Tool::createOpacityAnimation(opacityEffect, 0.0, 0.5, 100, QEasingCurve::InQuad).release();
            anim->start(QAbstractAnimation::DeleteWhenStopped);
        });
    }
}

void MainWindow::hideSettingsMenu(){

    int delayTime;
    if (isAnimation) delayTime = 200;
    else delayTime = 50;
    QTimer::singleShot(delayTime, this, [=](){
        bgmBtn->setVisible(false);
        effectBtn->setVisible(false);
        animationBtn->setVisible(false);
    });

    if (settingsOverlay){
        QGraphicsOpacityEffect* opacityEffect = static_cast<QGraphicsOpacityEffect*>(settingsOverlay->graphicsEffect());
        QPropertyAnimation* anim = Tool::createOpacityAnimation(opacityEffect, 0.5, 0.0, 300, QEasingCurve::OutQuad).release();
        connect(anim, &QPropertyAnimation::finished, settingsOverlay, &QWidget::deleteLater);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
        settingsOverlay = nullptr;
    }
}