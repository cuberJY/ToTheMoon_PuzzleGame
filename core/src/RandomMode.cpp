#include "RandomMode.h"

RandomMode::RandomMode(Diff chooseDiff){
    currentDiff = chooseDiff;
}

void RandomMode::setDiff(Diff chooseDiff){
    currentDiff = chooseDiff;
}
Diff RandomMode::getDiff() const{
    return currentDiff;
}

BoardConfig RandomMode::getBoardConfig() const{
    BoardConfig config;
    config.limitStep = -1;
    config.limitTime = -1;
    switch(currentDiff){
        case Diff::easy:
            config.rows = 4;
            config.cols = 4;
            config.isMD = true;
            config.isSD = false;
            config.limitTime = 30;
            break;
        case Diff::normal:
            config.rows = 5;
            config.cols = 4;
            config.isMD = true;
            config.isSD = false;
            config.limitTime = 25;
            break;
        case Diff::hard:
            config.rows = 6;
            config.cols = 5;
            config.isMD = true;
            config.isSD = true;
            config.limitStep = 2;
            config.limitTime = 20;
            break;
        case Diff::hardcore:
            config.rows = 6;
            config.cols = 6;
            config.isMD = true;
            config.isSD = true;
            config.limitStep = 0;
            config.limitTime = 20;
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

PuzzleBoard RandomMode::createBoard(){
    BoardConfig config = getBoardConfig();
    PuzzleBoard board(config.rows, config.cols, config.isMD, config.isSD, config.limitStep, config.limitTime);
    board.randomBoard();
    return board;
}

GameMode* RandomMode::clone() const{
    return new RandomMode(currentDiff);
}

ModeType RandomMode::getModeType() const{
    return ModeType::Random;
}