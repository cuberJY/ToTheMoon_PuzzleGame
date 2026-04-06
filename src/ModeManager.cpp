#include <ModeManager.h>
#include <PuzzleBoard.h>

#include <iostream>
#include <vector>

void ModeManager::setMode(int mode){
    currentMode = mode;
}
void ModeManager::setRandomDiff(int diff){
    currentDiff = diff;
}
int ModeManager::getRandomDiff(){
    return currentDiff;
}

PuzzleBoard ModeManager::createBoard(){
    switch(currentMode){
        case 0:{
            int row, col;
            bool sd, md;
            switch(currentDiff){
                case 0: row = 5; col = 5; sd = 1; md = 0; break;
                case 1: row = 6; col = 6; sd = 1; md = 0; break;
                case 2: row = 6; col = 6; sd = 1; md = 1; break;
                case 3: row = 6; col = 6; sd = 1; md = 1; break;
            }
            PuzzleBoard board(row, col, sd, md);
            board.randomBoard();
            return board;
        }
        default:
            return PuzzleBoard(1,1,0,0);
    }
}