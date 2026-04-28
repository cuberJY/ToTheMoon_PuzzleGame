#pragma once
#include "PuzzleBoard.h"
#include <string>

struct BoardConfig{//棋盘配置
    int rows;
    int cols;
    bool isMD;
    bool isSD;
    int limitStep;
    int limitTime;
};

enum class ModeType{
    Random,
    Level
};

enum class Diff{//难度枚举
    easy,
    normal,
    hard,
    hardcore
};

class GameMode{
public:
    virtual ~GameMode() = default;//虚析构函数，确保派生类对象被正确析构

    virtual PuzzleBoard createBoard() = 0;
    virtual BoardConfig getBoardConfig() const = 0;
    virtual GameMode* clone() const = 0;
    virtual ModeType getModeType() const = 0;

    virtual void setDiff(Diff) {}
    virtual Diff getDiff() const {return Diff::easy;}

    virtual void setLevel(int) {}
    virtual int getCurrentLevel() const {return 0;}
    virtual int getTotalLevel() const {return 0;}
};