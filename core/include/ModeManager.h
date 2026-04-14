#pragma once
#include "PuzzleBoard.h"

class ModeManager{
public:
    struct mode{
        static const int random = 0;
    };
    struct diff{
        static const int symple = 0;
        static const int normal = 1;
        static const int hard = 2;
        static const int hardcore = 3;
    };
    
    struct BoardConfig {
        int rows;
        int cols;
        bool isMD;
        bool isSD;
    };
    
    void setMode(int mode);
    void setRandomDiff(int diff);
    int getRandomDiff() const;
    
    BoardConfig getBoardConfig() const;
    PuzzleBoard createBoard();
    
    void updateStreak();
    void resetStreak();
    int getCurrentStreak() const;
    int getMaxStreak() const;
    
private:
    int currentMode, currentDiff;
    int currentStreak, maxStreak;//连赢局数
};