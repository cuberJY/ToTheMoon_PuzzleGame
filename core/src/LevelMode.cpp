#include "LevelMode.h"
#include "PuzzleBoard.h"

LevelMode::LevelMode(int level){
    currentLevel = level;
    setLevelStore();
}

void LevelMode::setLevel(int level){
    currentLevel = level;
}

int LevelMode::getCurrentLevel() const{
    return currentLevel;
}

int LevelMode::getTotalLevel() const{
    return levelStore.size();
}

PuzzleBoard LevelMode::createBoard(){
    LevelData& levelData = levelStore[currentLevel];
    BoardConfig config = levelData.config;

    PuzzleBoard board(config.rows, config.cols, config.isMD, config.isSD, config.limitStep, config.limitTime);
    board.setBoard(levelData.board);
    levelData.initialMinStep = board.getInitialMinStep();

    return board;
}

BoardConfig LevelMode::getBoardConfig() const{
    return levelStore[currentLevel].config;
}

GameMode* LevelMode::clone() const{
    return new LevelMode(currentLevel);
}

ModeType LevelMode::getModeType() const{
    return ModeType::Level;
}

void LevelMode::setLevelStore(){
    levelStore = {
        {
            {//第1关
                {0, 1, 1},
                {1, 0, 1},
                {1, 1, 0},
                {0, 0, 0}
            },
            {4, 3, true, false, -1, -1},
            0,
        },
        {
            {//第2关
                {1, 0, 1},
                {1, 0, 1},
                {0, 1, 0},
                {0, 1, 0}
            },
            {4, 3, true, false, -1, -1},
            0,
        },
        {
            {//第3关
                {1, 1, 1},
                {1, 1, 0},
                {1, 0, 1},
                {0, 1, 1}
            },
            {4, 3, true, false, -1, -1},
            0,
        },
        {
            {//第4关
                {1, 1, 1},
                {1, 1, 0},
                {0, 1, 0},
                {1, 0, 0}
            },
            {4, 3, true, false, -1, -1},
            0,
        },
        {
            {//第5关
                {1, 1, 0, 0},
                {1, 1, 1, 1},
                {0, 1, 1, 0},
                {1, 0, 1, 0}
            },
            {4, 4, true, false, -1, -1},
            0,
        },
        {
            {//第6关
                {0, 0, 1, 0},
                {0, 0, 0, 1},
                {0, 1, 1, 1},
                {1, 0, 1, 1},
                {0, 1, 0, 1}
            },
            {5, 4, true, false, 6, -1},
            0,
        },
        {
            {//第7关
                {0, 1, 0, 0},
                {0, 1, 0, 0},
                {1, 0, 1, 0},
                {0, 1, 1, 0},
                {1, 1, 1, 1},
                {1, 1, 0, 0}
            },
            {6, 4, true, true, 4, -1},
            0,
        },
        {
            {//第8关
                {0, 1, 1, 0, 0},
                {1, 0, 1, 0, 0},
                {0, 0, 1, 1, 1},
                {1, 1, 1, 1, 0},
                {0, 0, 0, 1, 0},
                {1, 1, 1, 0, 0}
            },
            {6, 5, true, true, 4, -1},
            0,
        },
        {
            {//第9关
                {0, 1, 1, 0, 0},
                {0, 1, 0, 1, 0},
                {1, 1, 0, 1, 0},
                {0, 0, 1, 0, 1},
                {0, 1, 0, 1, 0},
                {0, 1, 1, 0, 0}
            },
            {6, 5, true, true, 2, -1},
            0,
        },
        {
            {//第10关
                {0, 0, 0, 1, 0, 0},
                {0, 0, 0, 1, 1, 1},
                {0, 0, 0, 0, 0, 1},
                {1, 1, 0, 0, 1, 0},
                {0, 1, 0, 1, 0, 1},
                {1, 0, 0, 1, 0, 1}
            },
            {6, 6, true, true, 0, -1},
            0,
        },
        {
            {//第11关
                {0, 1, 0, 1, 0, 1},
                {0, 1, 1, 0, 0, 1},
                {1, 1, 1, 0, 0, 0},
                {0, 0, 0, 1, 1, 1},
                {1, 0, 0, 1, 1, 0},
                {0, 1, 0, 1, 0, 1}
            },
            {6, 6, true, true, 4, 25},
            0,
        },
        {
            {//第12关
                {1, 0, 1, 0, 0, 1},
                {0, 1, 1, 0, 1, 0},
                {0, 0, 0, 1, 0, 0},
                {1, 1, 1, 0, 1, 1},
                {1, 0, 0, 1, 0, 1},
                {0, 1, 0, 1, 1, 0}
            },
            {6, 6, true, true, 2, 20},
            0,
        },
        {
            {//第13关
                {0, 0, 0, 1, 0, 0},
                {0, 0, 0, 1, 1, 1},
                {1, 1, 1, 1, 1, 0},
                {0, 0, 1, 1, 0, 1},
                {1, 0, 1, 0, 1, 0},
                {1, 0, 0, 1, 0, 1}
            },
            {6, 6, true, true, 0, 20},
            0,
        },
        {
            {//第14关
                {0, 0, 1, 1, 1},
                {1, 1, 0, 0, 1},
                {1, 1, 0, 1, 0},
                {0, 0, 0, 1, 1},
                {0, 1, 1, 1, 1},
                {0, 1, 0, 0, 0}
            },
            {6, 5, true, true, 0, 15},
            0,
        },
        {
            {//第15关
                {1, 1, 1, 0, 1},
                {0, 0, 1, 0, 1},
                {1, 0, 1, 1, 0},
                {1, 0, 0, 0, 0},
                {1, 0, 0, 1, 1},
                {0, 1, 1, 0, 1}
            },
            {6, 5, true, true, 0, 10},
            0,
        },
    };
}
