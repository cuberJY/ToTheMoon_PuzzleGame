#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include "BoardController.h"
#include <QWidget>
#include <QPushButton>
#include <QTimer>
#include <QPropertyAnimation>//用于创建属性动画
#include <QGraphicsOpacityEffect>//用于创建透明度效果
#include <QSequentialAnimationGroup>//用于创建顺序动画组
#include <QEasingCurve>//用于创建缓动曲线
#include <QParallelAnimationGroup>//用于创建并行动画组
#include <QPixmap>//用于图片处理
#include <vector>
#include <utility>//用于pair类型

namespace Ui{
class BoardView;
}
class MainWindow;
//游戏棋盘界面类，管理棋盘显示和玩家操作
class BoardView : public QWidget{
    Q_OBJECT

public:
    explicit BoardView(MainWindow *parent = nullptr);
    ~BoardView();

    void setDifficulty(ModeManager::diff diff);
    void startGame();

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

private:
    //图片预加载和设置函数
    void preloadPiecePixmaps();
    //更新棋盘显示
    void updateBoardDisplay();
    void updateStepDisplay();
    void updateStreakDisplay();
    //设置操作按钮的启用状态
    void setButtonsEnabled(bool enabled, bool setVisual);
    //检查游戏状态
    void checkGameState();
    //翻转动画
    void startTurnAnimation();
    void updateSingleCell(int i, int j);
    void addAnimation(QPushButton* btn);
    void animateNextCell();
    //完成动画
    void startFinishAnimation();
    QSequentialAnimationGroup* shineAnimation();
    QSequentialAnimationGroup* borderAnimation();

    Ui::BoardView *ui;//UI界面指针
    BoardController* controller;//游戏控制器指针
    std::vector<std::vector<QPushButton*>> cellButtons;//棋盘格子按钮数组
    std::vector<std::vector<bool>> previousBoard;//保存上一状态的棋盘状态
    MainWindow* mainWindow;//主窗口指针
    
    QTimer *animationTimer;//控制动画时序的定时器
    std::vector<std::pair<int, int>> cellsToAnimate;//存储待动画的格子坐标
    int currentAnimationIndex;//当前动画的格子索引
    bool isAnimating;//是否正在动画中

    //图片缓存
    QPixmap cellOnPixmap;//cellOn图片缓存
    QPixmap cellOffPixmap;//cellOff图片缓存
    QPixmap cellOnPiecePixmaps[6][6];//cellOn拼图块缓存
    QPixmap cellOffPiecePixmaps[6][6];//cellOff拼图块缓存

    QString menuButtonStyle = "background-color: rgba(255, 255, 255, 0.1); color: rgba(255, 255, 255, 0.7); font-family:'Terminal'; font-size: 12px;";
    QString menuClickedButtonStyle = "background-color: rgba(0, 0, 0, 0.1); color: rgba(255, 255, 255, 0.8); font-family:'Terminal'; font-size: 12px;";
    QString diffTextStyle = "color: rgba(255, 255, 255, 0.7); font-family:'Terminal'; font-size: 20px;";
    QString messageTextStyle = "color: rgba(255, 255, 255, 0.7); font-family:'Terminal'; font-size: 16px;";
    QString stepTextStyle = "color: rgba(255, 255, 255, 0.5); font-family:'Century Schoolbook'; font-size: 28px;";
    QString streakTextStyle = "color: rgba(255, 255, 255, 0.7); font-family:'Terminal'; font-size: 20px;";
};

#endif // BOARDVIEW_H
