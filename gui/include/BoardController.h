#pragma once
#include "PuzzleBoard.h"
#include "ModeManager.h"
#include "PlayerData.h"
#include <QString>
#include <vector>

class BoardController{
public:
    BoardController();
    ~BoardController();

    void setDifficulty(ModeManager::diff diff);
    ModeManager::diff getDifficulty() const;

    void startGame();
    void restartGame();

    void turnRow(int row);
    void turnCol(int col);
    void turnMD();
    void turnSD();

    bool isGameFinished() const;
    bool isGameFailed() const;
    void checkGameState();

    int getPlayerStep() const;
    int getInitialMinStep() const;
    void resetStreak();
    int getCurrentStreak() const;
    int getMaxStreak(ModeManager::diff diff) const;

    std::vector<std::vector<bool>> getBoard();
    ModeManager::BoardConfig getBoardConfig() const;
    QString getBestSolveString() const;

private:
    PuzzleBoard* puzzleBoard;
    ModeManager* modeManager;
    PlayerData* playerData;
};