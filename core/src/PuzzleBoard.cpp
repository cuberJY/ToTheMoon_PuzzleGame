#include "PuzzleBoard.h"
#include <vector>
#include <string>
#include <random>
using namespace std;

//构造函数初始化 m*n
PuzzleBoard::PuzzleBoard(int m, int n, bool md, bool sd, int limitstep, int limittime)
    : row(m)
    , col(n)
    , isMD(md)
    , isSD(sd)
    , limitStep(limitstep)
    , limitTime(limittime)
    , initialMinStep(0){
    
    opCount = row + col;
    if (isSD) opCount++;
    if (isMD) opCount++;
    board.assign(row, vector<bool>(col, true));
}

void PuzzleBoard::setBoard(const std::vector<std::vector<bool>>& newBoard){
    board = newBoard;
    playerStep = 0;
    initialMinStep = getMinStep();
    if (limitStep >= 0) limitStep += initialMinStep;
}

int PuzzleBoard::getRow() const {return row;}
int PuzzleBoard::getCol() const {return col;}
const vector<vector<bool>>& PuzzleBoard::getBoard() const {return board;}


void PuzzleBoard::setLimitStep(int limitStep){
    this->limitStep = limitStep;
}
int PuzzleBoard::getLimitStep() const {return limitStep;}
void PuzzleBoard::setLimitTime(int limitTime){
    this->limitTime = limitTime;
}
int PuzzleBoard::getLimitTime() const {return limitTime;}

bool PuzzleBoard::isFinished() const {
    for (int i=0; i<row; i++){
        for (int j=0; j<col; j++){
            if (board[i][j] == 0) return false;
        }
    }
    return true;
}
bool PuzzleBoard::isFailed() const{
    if (limitStep < initialMinStep) return false;
    if (playerStep >= limitStep) return true;
    return false;
}

//玩家操作 (m+n+2)种
void PuzzleBoard::turnRow(int rowOrd){//翻转某行
    for (int i=0; i<col; i++){
        board[rowOrd][i] = !board[rowOrd][i];
    }
    playerStep++;
}
void PuzzleBoard::turnCol(int colOrd){//翻转某列
    for (int i=0; i<row; i++){
        board[i][colOrd] = !board[i][colOrd];
    }
    playerStep++;
}
void PuzzleBoard::turnMD(){//翻转主对角线
    for (int i=0,j=0; i<row&&j<col; i++,j++){
        board[i][j] = !board[i][j];
    }
    playerStep++;   
}
void PuzzleBoard::turnSD(){//翻转副对角线
    for (int i=row-1,j=0; i>=0&&j<col; i--,j++){
        board[i][j] = !board[i][j];
    }
    playerStep++;   
}

//系统模拟玩家打乱
void PuzzleBoard::randomBoard(){
    //随机数：随机设备+梅森旋转引擎+均匀分布(DeepSeek V3)
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, opCount+18);

    for (int i=0; i<30; i++){
        int n = dis(gen);

        if (n>0 && n<=row)
            turnRow(n-1);
        if (n>row && n<=row+col)
            turnCol(n-row-1);
        if (n>row+col && n<=row+col+8)
            if (isMD) turnMD();
        if (n>row+col+8)
            if (isSD) turnSD();
    }
    
    playerStep = 0;
    initialMinStep = getMinStep();
    if (initialMinStep < 3){
        randomBoard();
    }
    else{
        limitStep += initialMinStep;
    }
}

int PuzzleBoard::getMinStep(){
    bestSolve();
    return minStep;
}
int PuzzleBoard::getPlayerStep() const {
    return playerStep;
}
int PuzzleBoard::getInitialMinStep() const {
    return initialMinStep;
}

std::string PuzzleBoard::getBestSolveString(){
    bestSolve();
    if (minStep == 0) return "您已完成游戏\n请点击[再来一局]";
    std::string answer = "答案：\n";
    // 处理行操作
    for (int i=row-1; i>=0; i--){
        if (bestSol[i] == 1){
            answer += "翻转第" + std::to_string(row-i) + "行\n";
        }
    }
    // 处理列操作
    for (int i=row; i <row+col; i++){
        if (bestSol[i] == 1){
            answer += "翻转第" + std::to_string(i-row+1) + "列\n";
        }
    }
    // 处理对角线操作
    if (isMD && bestSol[row+col] == 1){
        answer += "点击左下角\n";
    }
    if (isSD && bestSol[row+col+1] == 1){
        answer += "点击左上角\n";
    }
    
    return answer;
}

//最优解
void PuzzleBoard::bestSolve(){

    //构造增广矩阵
    int mRow = row * col;
    int mCol = opCount + 1;
    vector<vector<bool>> mat(mRow, vector<bool>(mCol,0));

    for (int i=0; i<row; i++){//行
        for (int j=i*col; j<(i+1)*col; j++){
            mat[j][i] = 1;
        }
    }
    for(int i=0; i<col; i++){//列
        for (int j=i; j<mRow; j+=col){
            mat[j][i+row] = 1;
        }
    }
    if (isMD){//左上至右下
        int mdRow = 0;
        for (int i=0; i<min(row,col); i++){
            mat[mdRow][row+col] = 1;
            mdRow += (col+1);
        }
    }
    if (isSD){//左下至右上
        int sdRow = (row-1)*col;
        for (int i=0; i<min(row,col); i++){
            mat[sdRow][row+col+1] = 1;
            sdRow -= (col-1);
        }
    }
    for (int i=0; i<mRow; i++){//常数
        mat[i][mCol-1] = !board[i/col][i%col];
    }

    //高斯约旦消元
    int pRow = 0, pCol = 0;
    while (pRow < mRow && pCol < mCol){
        int pivot = -1;

        //在当前列寻找值为1的行
        for (int i=pRow; i<mRow; i++){
            if (mat[i][pCol] == 1){
                pivot = i;
                break;
            }
        }
        if (pivot == -1){
            pCol ++;
            continue; 
        }
        //将主元行交换至当前行
        if (pivot != pRow) {
            for (int j=0; j<mCol; j++) {
                swap(mat[pivot][j], mat[pRow][j]);
            }
        }

        //消去主元所在列的其余行，异或运算: ^=
        for (int i=pRow+1; i<mRow; i++){
            if (mat[i][pCol] == 1){
                for (int j=0; j<mCol; j++){
                    mat[i][j] = mat[i][j] ^ mat[pRow][j];
                }
            }
        }
        for (int i=0; i<pRow; i++){
            if (mat[i][pCol] == 1){
                for (int j=0; j<mCol; j++){
                    mat[i][j] = mat[i][j] ^ mat[pRow][j];
                }
            }
        }
        pRow++; pCol++;
    }

    //自由变量枚举
    vector<bool> Col (opCount, 0);//两个数组用于储存主元所在行列
    vector<int> Row (opCount, -1);
    for (int i=0; i<mRow; i++) {
        for (int j=0; j<opCount; j++) {
            if (mat[i][j]==1) {
                Col[j] = 1;//该列是主元所在的列
                Row[j] = i;//主元列中主元所在的行数
                break;
            }
        }
    }

    //数组存储自由变量所在的列
    vector<int> Free;
    for (int i=0; i<opCount; i++){
        if (!Col[i]){
            Free.push_back(i);
        }
    }
    int freeCount = Free.size();

    //无穷多解枚举，共2^freeCount种
    minStep = opCount + 1;//初始化为操作数+1
    bestSol.assign(opCount ,0);

    //位掩码：mask从低到高的每一个二进制位表示一个自由变量的取0还是1
    for (int mask=0; mask<(1<<freeCount); mask++){//1<<freeCount等价于pow(2,freeCount)
        int step = 0;
        vector<int> sol (opCount, 0);

        for (int i=0; i<freeCount; i++){
            sol[Free[i]] = (mask >> i) & 1;//提取mask的第i位
            if (sol[Free[i]]==1) step++;
        }
        if (step >= minStep) continue;//剪枝

        for (int i=0; i<mCol-1; i++){//遍历所有有变量的列
            if (!Col[i]) continue;//跳过自由变量

            int sum = 0;
            for (int j=0; j<mCol-1; j++){
                if (j!=i && mat[Row[i]][j]==1){//若主元所在行有其他的1，异或
                    sum ^= sol[j];
                }
            }
            sum ^= mat[Row[i]][mCol-1];//主元所在行的常数项
            sol[i] = sum;

            if (sum) step++;
            if (step >= minStep) break;//剪枝
        }
        if (step >= minStep) continue;//剪枝

        if (step < minStep) {//若步数更短，替换
            minStep = step;
            for (int i=0; i<opCount; i++){
                bestSol[i] = sol[i];
            }
        }
    }
}