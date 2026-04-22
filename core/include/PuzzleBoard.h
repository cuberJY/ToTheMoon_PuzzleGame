#pragma once
#include <vector>
#include <string>

class PuzzleBoard{
public:
    //构造函数初始化 m*n
    PuzzleBoard(int m, int n, bool md, bool sd);

    int getRow() const;
    int getCol() const;
    std::vector<std::vector<bool>> getBoard() const;
    bool isFinished() const;

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
    bool isSD, isMD;//是否启用主副对角线
    int opCount;
    std::vector<std::vector<bool>> board;//m行n列的棋盘

    void bestSolve();
    int minStep;
    int playerStep;
    int initialMinStep;
    std::vector<int> bestSol;
};