#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "BoardController.h"
#include "Home.h"
#include "ChooseLevel.h"
#include "ChooseDiff.h"
#include "BoardView.h"
#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QWidget>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

//管理难度选择和游戏棋盘界面的切换
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //构造函数
    explicit MainWindow(QWidget *parent = nullptr); 
    ~MainWindow();

    bool getIsBgmPlaying() const;//获取背景音乐播放状态
    void setIsBgmPlaying(bool value);//设置背景音乐播放状态
    bool getIsEffectPlaying() const;//获取音效播放状态
    void setIsEffectPlaying(bool value);//设置音效播放状态
    bool getIsAnimation() const;//获取动画播放状态
    void setIsAnimation(bool value);//设置动画播放状态

    int getPlayerMaxLevel() const;
    BoardController* getController() const;

private slots:
    //模式选择槽函数
    void onRandomModeSelected();
    void onLevelModeSelected();
    void onDifficultySelected(Diff diff);
    void onLevelSelected(int level);
    //返回菜单槽函数
    void onBackToMenu();
    //设置相关槽函数
    void on_backButton_clicked();
    void on_settingsBtn_clicked();
    void on_bgmBtn_clicked();
    void on_effectBtn_clicked();
    void on_animationBtn_clicked();

private:
    //设置菜单相关函数
    void initialSettingsButtons();
    void showSettingsMenu();
    void hideSettingsMenu();

    QStackedWidget *stackedWidget;//堆叠窗口，用于切换界面
    Home *homeWidget;
    ChooseLevel *chooseLevelWidget;
    ChooseDiff *chooseDiffWidget;
    BoardView *boardWidget;
    BoardController *controller;
    //设置菜单相关状态
    bool isBgmPlaying;
    bool isEffectPlaying;
    bool isAnimation;
    bool settingsMenuExpanded;
    //设置菜单相关组件
    QPushButton *backButton;
    QPushButton *settingsBtn;
    QPushButton *bgmBtn;
    QPushButton *effectBtn;
    QPushButton *animationBtn;
    QWidget *settingsOverlay;
};

#endif