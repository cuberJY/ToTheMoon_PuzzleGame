#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ModeManager.h"
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

    bool getIsBgmPlaying() const;
    void setIsBgmPlaying(bool value);
    bool getIsEffectPlaying() const;
    void setIsEffectPlaying(bool value);
    bool getIsAnimation() const;
    void setIsAnimation(bool value);

private slots:
    //难度选择槽函数
    void onDifficultySelected(ModeManager::diff diff);
    //返回菜单槽函数
    void onBackToMenu();
    void on_settingsBtn_clicked();
    void on_bgmBtn_clicked();
    void on_effectBtn_clicked();
    void on_animationBtn_clicked();

private:
    void initialSettingsButtons();//初始化设置按钮
    void showSettingsMenu();//显示设置菜单
    void hideSettingsMenu();//隐藏设置菜单

    QStackedWidget *stackedWidget;//堆叠窗口，用于切换界面
    ChooseDiff *chooseDiffWidget;//难度选择界面
    BoardView *boardWidget;//游戏棋盘界面

    bool isBgmPlaying;//是否正在播放bgm
    bool isEffectPlaying;//是否正在播放音效
    bool isAnimation;//是否播放动画

    QPushButton *settingsBtn;
    QPushButton *bgmBtn;
    QPushButton *effectBtn;
    QPushButton *animationBtn;
    bool settingsMenuExpanded;
    QWidget *settingsOverlay;

    QString settingsButtonStyle = "background-color: rgba(255, 255, 255, 0.1); color: rgba(255, 255, 255, 0.7); font-family:'Terminal'; font-size: 12px;";
    QString settingsClickedButtonStyle = "background-color: rgba(0, 0, 0, 0.75); color: rgba(255, 255, 255, 0.8); font-family:'Terminal'; font-size: 12px;";
    QString settingsOnButtonStyle = "background-color: rgba(255, 255, 255, 0.5); color: rgba(64, 64, 64, 0.6); font-family:'Terminal'; font-size: 12px;";
    QString settingsOffButtonStyle = "background-color: rgba(255, 255, 255, 0.1); color: rgba(255, 255, 255, 0.7); font-family:'Terminal'; font-size: 12px;";
};

#endif