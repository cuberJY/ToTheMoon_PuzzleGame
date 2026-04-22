#include "ModeManager.h"
#include "PuzzleBoard.h"

void ModeManager::setMode(mode chooseMode){
    currentMode = chooseMode;
}
void ModeManager::setRandomDiff(diff chooseDiff){
    currentDiff = chooseDiff;
}
ModeManager::diff ModeManager::getRandomDiff() const{
    return currentDiff;
}

ModeManager::BoardConfig ModeManager::getBoardConfig() const{
    BoardConfig config;

    switch(currentDiff){
        case diff::easy:
            config.rows = 4;
            config.cols = 4;
            config.isMD = true;
            config.isSD = false;
            break;
        case diff::normal:
            config.rows = 5;
            config.cols = 4;
            config.isMD = true;
            config.isSD = false;
            break;
        case diff::hard:
            config.rows = 6;
            config.cols = 5;
            config.isMD = true;
            config.isSD = true;
            break;
        case diff::hardcore:
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
        case mode::random:{
            BoardConfig config = getBoardConfig();
            PuzzleBoard board(config.rows, config.cols, config.isMD, config.isSD);
            board.randomBoard();
            return board;
        }
        default:
            return PuzzleBoard(1,1,0,0);
    }
}