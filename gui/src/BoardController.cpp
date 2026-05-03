#include "BoardController.h"
#include "PuzzleBoard.h"
#include "GameMode.h"
#include "RandomMode.h"
#include "LevelMode.h"
#include "PlayerData.h"

BoardController::BoardController():
    puzzleBoard(nullptr),
    gameMode(nullptr),
    playerData(new PlayerData()),
    currentDiff(Diff::easy),
    timedOut(false)
{
}

BoardController::~BoardController(){
    if (puzzleBoard) delete puzzleBoard;
    if (gameMode) delete gameMode;
    if (playerData) delete playerData;
}

//游戏模式设置
void BoardController::setGameMode(GameMode* mode){
    if (gameMode) delete gameMode;
    gameMode = mode;
}
ModeType BoardController::getCurrentMode() const{
    return currentMode;
}

//开始游戏
void BoardController::startGame(){
    if (puzzleBoard){
        delete puzzleBoard;
    }
    currentMode = gameMode->getModeType();
    puzzleBoard = new PuzzleBoard(gameMode->createBoard());
    timedOut = false;
}
//重新开始游戏
void BoardController::restartGame(){
    if (currentMode == ModeType::Random && !isGameFinished()){
        playerData->resetStreak();
    }
    startGame();
}

//游戏操作
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

//判断游戏是否完成
bool BoardController::isGameFinished() const{
    return puzzleBoard->isFinished();
}
//判断游戏是否失败
bool BoardController::isGameFailed() const{
    return timedOut || puzzleBoard->isFailed();
}
//检查游戏状态
void BoardController::checkGameState(){
    if (currentMode == ModeType::Random){
        if (isGameFinished()){
            playerData->updateStreak(currentDiff);
        }
        else if (isGameFailed()){
            playerData->resetStreak();
        }
    }
    else if (currentMode == ModeType::Level){
        if (isGameFinished()){
            updateMaxLevel();
        }
    }
}

//获取当前玩家操作次数
int BoardController::getPlayerStep() const{
    return puzzleBoard->getPlayerStep();
}
//获取初始最小操作次数
int BoardController::getInitialMinStep() const{
    return puzzleBoard->getInitialMinStep();
}
//获取当前拼图板状态
const std::vector<std::vector<bool>>& BoardController::getBoard() const{
    return puzzleBoard->getBoard();
}
//获取当前拼图板配置
BoardConfig BoardController::getBoardConfig() const{
    return gameMode->getBoardConfig();
}
//获取最佳解决路径字符串
QString BoardController::getBestSolveString() const{
    return QString::fromStdString(puzzleBoard->getBestSolveString());
}
//倒计时
bool BoardController::isCountdownEnabled() const{
    return puzzleBoard->getLimitTime() > 0;
}
void BoardController::setTimedOut(bool value){
    timedOut = value;
}
bool BoardController::isTimedOut() const{
    return timedOut;
}
//剩余步数
int BoardController::getRemStep() const{
    return puzzleBoard->getLimitStep() - puzzleBoard->getPlayerStep();
}
//重置游戏数据
void BoardController::resetData() const{
    playerData->resetData();
}

//====================随机模式====================
//设置游戏难度
void BoardController::setDiff(Diff diff){
    currentDiff = diff;
    gameMode->setDiff(diff);
    playerData->resetStreak();
}
//获取游戏难度
Diff BoardController::getCurrentDiff() const{
    return currentDiff;
}
//重置当前连胜次数
void BoardController::resetStreak(){
    playerData->resetStreak();
}
//获取当前连胜次数
int BoardController::getCurrentStreak() const{
    return playerData->getCurrentStreak();
}
//获取最大连胜次数
int BoardController::getMaxStreak(Diff diff) const{
    if (diff == Diff::easy){
        return playerData->getMaxStreakEasy();
    }
    else if (diff == Diff::normal){
        return playerData->getMaxStreakNormal();
    }
    else if (diff == Diff::hard){
        return playerData->getMaxStreakHard();
    }
    else if (diff == Diff::hardcore){
        return playerData->getMaxStreakHardcore();
    }
    return 0;
}

//====================关卡模式====================
//设置当前关卡
void BoardController::setLevel(int level){
    gameMode->setLevel(level);
}
//获取当前关卡
int BoardController::getCurrentLevel() const{
    return gameMode->getCurrentLevel();
}
//下一关
void BoardController::nextLevel(){
    int next = gameMode->getCurrentLevel() + 1;
    if (next >= gameMode->getTotalLevel()){
        return;
    }
    gameMode->setLevel(next);
    startGame();
}
//上一关
void BoardController::prevLevel(){
    int prev = gameMode->getCurrentLevel() - 1;
    if (prev < 0){
        return;
    }
    gameMode->setLevel(prev);
    startGame();
}
//获取总关卡数
int BoardController::getTotalLevel() const{
    return gameMode->getTotalLevel();
}
//更新玩家最大关卡
void BoardController::updateMaxLevel(){
    if (gameMode->getCurrentLevel() == playerData->getMaxLevel()){
        playerData->setMaxLevel(gameMode->getCurrentLevel() + 1);
    }
}
//获取玩家最大关卡
int BoardController::getPlayerMaxLevel() const{
    return playerData->getMaxLevel();
}
