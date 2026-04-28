#pragma once
#include "GameMode.h"

class RandomMode : public GameMode{
public:
    RandomMode(Diff chooseDiff);
    
    void setDiff(Diff chooseDiff) override;
    Diff getDiff() const override;

    PuzzleBoard createBoard() override;
    BoardConfig getBoardConfig() const override;
    GameMode* clone() const override;
    ModeType getModeType() const override;

private:
    Diff currentDiff;
};