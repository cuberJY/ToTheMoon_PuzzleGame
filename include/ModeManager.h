#pragma once
#include <PuzzleBoard.h>

#include <vector>
#include <string>

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
    
    void setMode(int mode);
    void setRandomDiff(int diff);
    int getRandomDiff();

    PuzzleBoard createBoard();
    
private:
    int currentMode, currentDiff;
};