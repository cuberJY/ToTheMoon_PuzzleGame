#pragma once
#include <vector>

class PuzzleBoard{
public:
    //构造函数初始化 m*n
    PuzzleBoard(int m, int n, bool sd, bool md);

    int getRow();
    int getCol();
    std::vector<std::vector<bool>> getBoard();
    bool isFinished();
    void printBoard();

    //玩家操作 (m+n+2)种
    void turnRow(int rowOrd);
    void turnCol(int colOrd);
    void turnSD();
    void turnMD();

    //系统模拟玩家打乱
    void randomBoard();

    int getMinStep();
    int getPlayerStep();
    void printBestSolve();

private:
    int row, col;
    bool isSD, isMD;//是否启用主副对角线
    int opCount;
    std::vector<std::vector<bool>> board;//m行n列的棋盘

    void bestSolve();
    int minStep;
    int playerStep;
    std::vector<int> bestSol;
};