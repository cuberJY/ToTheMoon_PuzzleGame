#include "MainWindow.h"

//构造函数，初始化主窗口
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //创建堆叠窗口，用于界面切换
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    //创建难度选择界面和游戏棋盘界面
    chooseDiffWidget = new ChooseDiff(this);
    boardWidget = new Board(this);

    //将两个界面添加到堆叠窗口
    stackedWidget->addWidget(chooseDiffWidget);
    stackedWidget->addWidget(boardWidget);

    //初始显示难度选择界面
    stackedWidget->setCurrentWidget(chooseDiffWidget);

    //连接信号和槽
    connect(chooseDiffWidget, &ChooseDiff::difficultySelected, this, &MainWindow::onDifficultySelected);
    connect(boardWidget, &Board::backToMenu, this, &MainWindow::onBackToMenu);

    //设置窗口标题和大小
    setWindowTitle("To The Moon Puzzle Game");
    resize(800, 600);
}

//难度选择槽函数：切换到游戏棋盘界面
void MainWindow::onDifficultySelected(int diff)
{
    boardWidget->setDifficulty(diff);//设置难度
    boardWidget->startGame();//开始游戏
    stackedWidget->setCurrentWidget(boardWidget);//切换到棋盘界面
}

//返回菜单槽函数：切换回难度选择界面
void MainWindow::onBackToMenu()
{
    stackedWidget->setCurrentWidget(chooseDiffWidget);
}