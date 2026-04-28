#pragma once
#include "GameMode.h"
#include <vector>
#include <string>

struct LevelData{
    std::vector<std::vector<bool>> board;
    BoardConfig config;
    int initialMinStep;
};

class LevelMode : public GameMode{
public:
    LevelMode(int level);

    void setLevel(int level) override;
    int getCurrentLevel() const override;
    int getTotalLevel() const override;

    PuzzleBoard createBoard() override;
    BoardConfig getBoardConfig() const override;
    GameMode* clone() const override;
    ModeType getModeType() const override;
    
private:
    int currentLevel;
    std::vector<LevelData> levelStore;

    void setLevelStore();
};