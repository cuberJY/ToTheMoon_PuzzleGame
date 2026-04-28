#pragma once
#include <vector>
#include <string>

class PuzzleBoard{
public:
    //构造函数初始化 m*n
    PuzzleBoard(int m, int n, bool md, bool sd, int limitStep, int limitTime);

    void setBoard(const std::vector<std::vector<bool>>& newBoard);

    int getRow() const;
    int getCol() const;
    const std::vector<std::vector<bool>>& getBoard() const;

    void setLimitStep(int limitStep);
    int getLimitStep() const;
    void setLimitTime(int limitTime);
    int getLimitTime() const;
    
    bool isFinished() const;
    bool isFailed() const;
    
    //玩家操作 (m+n+2)种
    void turnRow(int rowOrd);
    void turnCol(int colOrd);
    void turnMD();
    void turnSD();

    //系统模拟玩家打乱
    void randomBoard();

    int getMinStep();
    int getPlayerStep() const;
    std::string getBestSolveString();

    int getInitialMinStep() const;//初始最佳步数

private:
    int row, col;
    bool isMD, isSD;//是否启用主副对角线
    int limitStep;//最大步数限制
    int limitTime;//最大时间限制
    int opCount;
    std::vector<std::vector<bool>> board;//m行n列的棋盘

    void bestSolve();
    int minStep;
    int playerStep;
    int initialMinStep;
    std::vector<int> bestSol;
};