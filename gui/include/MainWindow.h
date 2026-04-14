#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "choosediff.h"
#include "board.h"

//管理难度选择和游戏棋盘界面的切换
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //构造函数
    explicit MainWindow(QWidget *parent = nullptr); 

private slots:
    //难度选择槽函数
    void onDifficultySelected(int diff);
    //返回菜单槽函数
    void onBackToMenu();

private:
    QStackedWidget *stackedWidget;//堆叠窗口，用于切换界面
    ChooseDiff *chooseDiffWidget;//难度选择界面
    Board *boardWidget;//游戏棋盘界面
};

#endif