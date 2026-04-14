#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QPushButton>
#include <vector>
#include <QTimer>
#include "PuzzleBoard.h"
#include "ModeManager.h"

namespace Ui {
class Board;
}

//游戏棋盘界面类，管理棋盘显示和玩家操作
class Board : public QWidget
{
    Q_OBJECT

public:
    //构造函数
    explicit Board(QWidget *parent = nullptr);
    //析构函数
    ~Board();

    //开始游戏
    void startGame();
    //设置游戏难度
    void setDifficulty(int diff);

signals:
    //返回菜单信号
    void backToMenu();

private slots:
    //通用行按钮点击槽函数
    void onRowButtonClicked(int row);
    //通用列按钮点击槽函数
    void onColButtonClicked(int col);
    //对角线按钮点击槽函数
    void on_mdBtn_clicked();
    void on_sdBtn_clicked();
    //显示答案按钮点击槽函数
    void on_answerBtn_clicked();
    //重新开始按钮点击槽函数
    void on_restartBtn_clicked();
    //返回按钮点击槽函数
    void on_backBtn_clicked();
    //下一个cell动画槽函数
    void animateNextCell();

private:
    //更新棋盘显示
    void updateBoardDisplay();
    //检查游戏状态（胜利/失败）
    void checkGameState();
    //设置操作按钮的启用状态
    void setButtonsEnabled(bool enabled, bool bannedVisual);
    //更新连胜统计显示
    void updateStreakDisplay();
    //开始执行动画
    void startAnimation();
    //更新单个cell的显示
    void updateSingleCell(int i, int j);

    Ui::Board *ui;//UI界面指针
    PuzzleBoard *puzzleBoard;//棋盘核心逻辑类
    ModeManager *manager;//游戏模式管理器
    std::vector<std::vector<QPushButton*>> cellButtons;//棋盘格子按钮数组

    std::vector<std::vector<bool>> previousBoard;//保存上一状态的棋盘
    void addAnimation(QPushButton* btn);
    
    QTimer *animationTimer;//控制动画时序的定时器
    std::vector<std::pair<int, int>> cellsToAnimate;//存储待动画的格子坐标
    int currentAnimationIndex;//当前动画的格子索引
    bool isAnimating;//是否正在动画中
};

#endif // BOARD_H
