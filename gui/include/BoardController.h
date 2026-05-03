#pragma once
#include "PuzzleBoard.h"
#include "GameMode.h"
#include "RandomMode.h"
#include "LevelMode.h"
#include "PlayerData.h"
#include <QString>
#include <vector>

//游戏控制器类，管理游戏逻辑和状态
class BoardController{
public:
    BoardController();
    ~BoardController();
    //游戏模式设置
    void setGameMode(GameMode* mode);
    ModeType getCurrentMode() const;
    //游戏开启
    void startGame();
    void restartGame();
    //游戏操作
    void turnRow(int row);
    void turnCol(int col);
    void turnMD();
    void turnSD();
    //游戏状态
    bool isGameFinished() const;
    bool isGameFailed() const;
    void checkGameState();
    //游戏数据
    int getPlayerStep() const;
    int getInitialMinStep() const;
    const std::vector<std::vector<bool>>& getBoard() const;
    BoardConfig getBoardConfig() const;
    QString getBestSolveString() const;
    //重置游戏数据
    void resetData() const;

    //倒计时
    bool isCountdownEnabled() const;
    void setTimedOut(bool value);
    bool isTimedOut() const;
    //剩余步数
    int getRemStep() const;

//随机模式
    //游戏难度设置
    void setDiff(Diff diff);
    Diff getCurrentDiff() const;
    //连胜次数设置
    void resetStreak();
    int getCurrentStreak() const;
    int getMaxStreak(Diff diff) const;
    
//关卡模式
    //关卡设置
    void setLevel(int level);
    int getCurrentLevel() const;
    void nextLevel();
    void prevLevel();
    //关卡数据设置
    int getTotalLevel() const;
    void updateMaxLevel();
    int getPlayerMaxLevel() const;

private:
    PuzzleBoard* puzzleBoard;
    GameMode* gameMode;
    PlayerData* playerData;
    ModeType currentMode;
    Diff currentDiff;
    
    bool timedOut;
};