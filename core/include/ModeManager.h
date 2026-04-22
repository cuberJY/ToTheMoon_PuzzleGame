#pragma once
#include "PuzzleBoard.h"

class ModeManager{
public:
    enum class mode{
        random,
    };
    enum class diff{
        easy,
        normal,
        hard,
        hardcore,
    };
    
    struct BoardConfig {
        int rows;
        int cols;
        bool isMD;
        bool isSD;
    };
    
    void setMode(mode chooseMode);
    void setRandomDiff(diff chooseDiff);
    diff getRandomDiff() const;
    
    BoardConfig getBoardConfig() const;
    PuzzleBoard createBoard();
    
private:
    mode currentMode;
    diff currentDiff;
};