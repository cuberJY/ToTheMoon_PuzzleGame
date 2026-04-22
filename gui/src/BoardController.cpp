#include "BoardController.h"
#include "PuzzleBoard.h"
#include "ModeManager.h"
#include "PlayerData.h"

BoardController::BoardController():
    puzzleBoard(nullptr),
    modeManager(new ModeManager()),
    playerData(new PlayerData())
{    
    modeManager->setMode(ModeManager::mode::random);
}

BoardController::~BoardController(){
    if (puzzleBoard) delete puzzleBoard;
    if (modeManager) delete modeManager;
    if (playerData) delete playerData;
}

void BoardController::setDifficulty(ModeManager::diff diff) {
    modeManager->setRandomDiff(diff);
    playerData->resetStreak();
}

ModeManager::diff BoardController::getDifficulty() const{
    return modeManager->getRandomDiff();
}

void BoardController::startGame(){
    if (puzzleBoard){
        delete puzzleBoard;
    }
    puzzleBoard = new PuzzleBoard(modeManager->createBoard());
}

void BoardController::restartGame(){
    if (!isGameFinished()){
        playerData->resetStreak();
    }
    startGame();
}

void BoardController::turnRow(int row){
    puzzleBoard->turnRow(row);
    checkGameState();
}

void BoardController::turnCol(int col){
    puzzleBoard->turnCol(col);
    checkGameState();
}

void BoardController::turnMD(){
    puzzleBoard->turnMD();
    checkGameState();
}

void BoardController::turnSD(){
    puzzleBoard->turnSD();
    checkGameState();
}

bool BoardController::isGameFinished() const{
    return puzzleBoard->isFinished();
}

bool BoardController::isGameFailed() const{
    if (!puzzleBoard) return false;
    
    ModeManager::diff diff = modeManager->getRandomDiff();
    int playerStep = puzzleBoard->getPlayerStep();
    int initialMinStep = puzzleBoard->getInitialMinStep();
    
    if (diff == ModeManager::diff::hard){
        return playerStep >= initialMinStep + 2;
    }
    else if (diff == ModeManager::diff::hardcore){
        return playerStep >= initialMinStep;
    }
    return false;
}

void BoardController::checkGameState(){
    if (modeManager->getRandomDiff() >= ModeManager::diff::hard){
        if (isGameFinished()){
            playerData->updateStreak(modeManager->getRandomDiff());
        }
        else if (isGameFailed()){
            playerData->resetStreak();
        }
    }
}

int BoardController::getPlayerStep() const{
    return puzzleBoard->getPlayerStep();
}

int BoardController::getInitialMinStep() const{
    return puzzleBoard->getInitialMinStep();
}

void BoardController::resetStreak(){
    playerData->resetStreak();
}

int BoardController::getCurrentStreak() const{
    return playerData->getCurrentStreak();
}

int BoardController::getMaxStreak(ModeManager::diff diff) const{
    if (diff == ModeManager::diff::hard){
        return playerData->getMaxStreakHard();
    }
    else if (diff == ModeManager::diff::hardcore){
        return playerData->getMaxStreakHardcore();
    }
    return 0;
}

std::vector<std::vector<bool>> BoardController::getBoard(){
    return puzzleBoard->getBoard();
}

ModeManager::BoardConfig BoardController::getBoardConfig() const{
    return modeManager->getBoardConfig();
}

QString BoardController::getBestSolveString() const{
    return QString::fromStdString(puzzleBoard->getBestSolveString());
}
