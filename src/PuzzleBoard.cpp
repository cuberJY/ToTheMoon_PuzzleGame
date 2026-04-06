#include "PuzzleBoard.h"

#include <iostream>
#include <vector>
#include <random>
using namespace std;

//构造函数初始化 m*n
PuzzleBoard::PuzzleBoard(int m, int n, bool sd, bool md):row(m), col(n){
    isSD = sd;
    isMD = md;
    opCount = row + col;
    if (isSD) opCount++;
    if (isMD) opCount++;
    board.assign(row, vector<bool>(col, true));
}

int PuzzleBoard::getRow(){return row;}
int PuzzleBoard::getCol(){return col;}
vector<vector<bool>> PuzzleBoard::getBoard(){return board;}

bool PuzzleBoard::isFinished(){
    for (int i=0; i<row; i++){
        for (int j=0; j<col; j++){
            if (board[i][j] == 0) return false;
        }
    }
    return true;
}

void PuzzleBoard::printBoard(){
    for (int i=0; i<row; i++){
        for (int j=0; j<col; j++){
            cout << board[i][j];
            if (j<col-1) cout << " ";
        }
        cout << '\n';
    }
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
void PuzzleBoard::turnSD(){//翻转副对角线
    for (int i=row-1,j=0; i>=0&&j<col; i--,j++){
        board[i][j] = !board[i][j];
    }
    playerStep++;   
}
void PuzzleBoard::turnMD(){//翻转主对角线
    for (int i=0,j=0; i<row&&j<col; i++,j++){
        board[i][j] = !board[i][j];
    }
    playerStep++;   
}

//系统模拟玩家打乱
void PuzzleBoard::randomBoard(){
    //随机数：随机设备+梅森旋转引擎+均匀分布(DeepSeek V3)
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, opCount);

    for (int i=0; i<30; i++){
        int n = dis(gen);

        if (n>0 && n<=row)
            turnRow(n-1);
        if (n>row && n<=row+col)
            turnCol(n-row-1);
        if (n==row+col+1)
            turnSD();
        if (n==row+col+2)
            turnMD();
    }
    playerStep = 0;
}

int PuzzleBoard::getMinStep(){
    bestSolve();
    return minStep;
}
int PuzzleBoard::getPlayerStep(){
    return playerStep;
}

void PuzzleBoard::printBestSolve(){
    bestSolve();
    cout << "答案：";
        for (int i=0; i<opCount; i++){
            if (bestSol[i] == 1){
                if (i>=0 && i<row) cout << "翻转第" << i+1 << "行 ";
                if (i>=row && i<row+col) cout << "翻转第" << i-row+1 << "列 ";
                if (isSD && i==row+col) cout << "翻转副对角线 ";
                if (isMD && i==row+col+1) cout << "翻转主对角线 ";
            }
        }
    cout << '\n';
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
    if (isSD){//左下至右上
        int sdRow = (row-1)*col;
        for (int i=0; i<min(row,col); i++){
            mat[sdRow][row+col] = 1;
            sdRow -= (col-1);
        }
    }
    if (isMD){//左上至右下
        int mdRow = 0;
        for (int i=0; i<min(row,col); i++){
            mat[mdRow][row+col+1] = 1;
            mdRow += (col+1);
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

        for (int i=0; i<mCol-1; i++){//遍历所有有变量的列
            if (!Col[i]) continue;//跳过自由变量
            int sum = mat[Row[i]][mCol-1];//主元所在行的常数项

            for (int j=0; j<mCol-1; j++){
                if (j!=i && mat[Row[i]][j]==1){//若主元所在行有其他的1，异或
                    sum ^= sol[j];
                }
            }
            sol[i] = sum;
            if (sum) step++;
        }

        if (step < minStep) {//若步数更短，替换
            minStep = step;
            for (int i=0; i<opCount; i++){
                bestSol[i] = sol[i];
            }
        }
    }
}