#include "ModeManager.h"
#include "PuzzleBoard.h"

void ModeManager::setMode(int mode){
    currentMode = mode;
}
void ModeManager::setRandomDiff(int diff){
    currentDiff = diff;
    currentStreak = 0;
    maxStreak = 0;
}
int ModeManager::getRandomDiff() const{
    return currentDiff;
}

ModeManager::BoardConfig ModeManager::getBoardConfig() const {
    BoardConfig config;

    switch(currentDiff){
        case 0:
            config.rows = 4;
            config.cols = 4;
            config.isMD = true;
            config.isSD = false;
            break;
        case 1:
            config.rows = 5;
            config.cols = 4;
            config.isMD = true;
            config.isSD = false;
            break;
        case 2:
            config.rows = 6;
            config.cols = 5;
            config.isMD = true;
            config.isSD = true;
            break;
        case 3:
            config.rows = 6;
            config.cols = 6;
            config.isMD = true;
            config.isSD = true;
            break;
        default:
            config.rows = 6;
            config.cols = 6;
            config.isMD = true;
            config.isSD = true;
            break;
    }

    return config;
}

PuzzleBoard ModeManager::createBoard(){
    switch(currentMode){
        case 0:{
            ModeManager::BoardConfig config = getBoardConfig();
            PuzzleBoard board(config.rows, config.cols, config.isMD, config.isSD);
            board.randomBoard();
            return board;
        }
        default:
            return PuzzleBoard(1,1,0,0);
    }
}

void ModeManager::updateStreak(){
    currentStreak++;
    if (currentStreak > maxStreak){
        maxStreak = currentStreak;
    }
}

void ModeManager::resetStreak(){
    currentStreak = 0;
}

int ModeManager::getCurrentStreak() const{
    return currentStreak;
}

int ModeManager::getMaxStreak() const{
    return maxStreak;
}