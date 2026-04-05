#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <windows.h>
using namespace std;

void bestSolve(const vector<vector<bool>>& board, bool isSD, bool isMD, bool answer){
    //board的行列数
    int bRow = board.size();
    int bCol = board[0].size();

    int opCount = bRow + bCol;//总操作数
    if (isSD) opCount++;
    if (isMD) opCount++;

//构造增广矩阵
    int mRow = bRow * bCol;
    int mCol = opCount + 1;
    vector<vector<bool>> mat(mRow, vector<bool>(mCol,0));

    for (int i=0; i<bRow; i++){//行
        for (int j=i*bCol; j<(i+1)*bCol; j++){
            mat[j][i] = 1;
        }
    }
    for(int i=0; i<bCol; i++){//列
        for (int j=i; j<mRow; j+=bCol){
            mat[j][i+bRow] = 1;
        }
    }
    if (isSD){//左下至右上
        int sdRow = (bRow-1)*bCol;
        for (int i=0; i<min(bRow,bCol); i++){
            mat[sdRow][bRow+bCol] = 1;
            sdRow -= (bCol-1);
        }
    }
    if (isMD){//左上至右下
        int mdRow = 0;
        for (int i=0; i<min(bRow,bCol); i++){
            mat[mdRow][bRow+bCol+1] = 1;
            mdRow += (bCol+1);
        }
    }
    for (int i=0; i<mRow; i++){//常数
        mat[i][mCol-1] = !board[i/bCol][i%bCol];
    }

//高斯约旦消元
    int row = 0, col = 0;
    while (row < mRow && col < mCol){
        int pivot = -1;

        //在当前列寻找值为1的行
        for (int i=row; i<mRow; i++){
            if (mat[i][col] == 1){
                pivot = i;
                break;
            }
        }
        if (pivot == -1){
            col ++;
            continue; 
        }
        //将主元行交换至当前行
        if (pivot != row) {
            for (int j=0; j<mCol; j++) {
                swap(mat[pivot][j], mat[row][j]);
            }
        }

        //消去主元所在列的其余行，异或运算: ^=
        for (int i=row+1; i<mRow; i++){
            if (mat[i][col] == 1){
                for (int j=0; j<mCol; j++){
                    mat[i][j] = mat[i][j] ^ mat[row][j];
                }
            }
        }
         for (int i=0; i<row; i++){
            if (mat[i][col] == 1){
                for (int j=0; j<mCol; j++){
                    mat[i][j] = mat[i][j] ^ mat[row][j];
                }
            }
        }
        row++; col++;
    }

//自由变量枚举
    //两个数组用于储存主元所在行列
    vector<bool> Col (opCount, 0);
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
    int minStep = opCount+1;//初始化为操作数+1
    vector<int> bestSol (opCount ,0);

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

    
    cout << "最短操作步数：" << minStep << endl;
    
    if (answer){
        cout << "答案：";
        for (int i=0; i<opCount; i++){
            if (bestSol[i] == 1){
                if (i>=0 && i<bRow) cout << "翻转第" << i+1 << "行 ";
                if (i>=bRow && i<bRow+bCol) cout << "翻转第" << i-bRow+1 << "列 ";
                if (isSD && i==bRow+bCol) cout << "翻转副对角线 ";
                if (isMD && i==bRow+bCol+1) cout << "翻转主对角线 ";
            }
        }
        cout << '\n';
    }
}

class PuzzleBoard{
public:
    //构造函数初始化 m*n
    PuzzleBoard (int m,int n){
        row = m;
        col = n;
        board.assign(row, vector<bool>(col, true));
    }

    int getRow(){return row;}
    int getCol(){return col;}
    vector<vector<bool>> getBoard(){return board;}

    //玩家操作 (m+n+2)种
    void turnRow(int rowOrd){//翻转某行
        for (int i=0; i<col; i++){
            board[rowOrd][i] = !board[rowOrd][i];
        }
    }
    void turnCol(int colOrd){//翻转某列
        for (int i=0; i<row; i++){
            board[i][colOrd] = !board[i][colOrd];
        }
    }
    void turnSD(){//翻转副对角线
        for (int i=row-1,j=0; i>=0&&j<col; i--,j++){
            board[i][j] = !board[i][j];
        }
    }
    void turnMD(){//翻转主对角线
        for (int i=0,j=0; i<row&&j<col; i++,j++){
            board[i][j] = !board[i][j];
        }
    }

    bool isFinished(){
        for (int i=0; i<row; i++){
            for (int j=0; j<col; j++){
                if (board[i][j] == 0) return false;
            }
        }
        return true;
    }

    //系统模拟玩家打乱
    void randomBoard(){
        int opCount = row+col+2;

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
    }

    //打印棋盘
    void print(){
        for (int i=0; i<row; i++){
            for (int j=0; j<col; j++){
                cout << board[i][j];
                if (j<col-1) cout << " ";
            }
            cout << '\n';
        }
    }

private:
    int row, col;
    vector<vector<bool>> board;//m行n列的棋盘

};

int main(){
    // 设置控制台输出为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
    // 可选：同时设置输入编码为 UTF-8（如果需要读中文）
    SetConsoleCP(CP_UTF8);

    PuzzleBoard board(6,6);
flag:
    board.randomBoard();
    board.print();
    
    int opNum = 0;
    char op;

    while (!board.isFinished()){
        cout << "目标:全为'1' \n qwerty反转1-6行，asdfgh反转1-6列，z反转副对角线，x反转主对角线,m显示答案" << endl;
        bestSolve(board.getBoard(), 1, 1, 0);

        cin >> op;
        opNum ++;
        if (op == 'm'){bestSolve(board.getBoard(), 1, 1, 1);}
        else{
            if (op == 'p') goto flag;
            else if (op=='q'){board.turnRow(0);}
            else if (op=='w'){board.turnRow(1);}
            else if (op=='e'){board.turnRow(2);}
            else if (op=='r'){board.turnRow(3);}
            else if (op=='t'){board.turnRow(4);}
            else if (op=='y'){board.turnRow(5);}
            else if (op=='a'){board.turnCol(0);}
            else if (op=='s'){board.turnCol(1);}
            else if (op=='d'){board.turnCol(2);}
            else if (op=='f'){board.turnCol(3);}
            else if (op=='g'){board.turnCol(4);}
            else if (op=='h'){board.turnCol(5);}
            else if (op=='z'){board.turnSD();}
            else if (op=='x'){board.turnMD();}
            else {cout << "无效操作！" << endl;}
            board.print();
            cout << "当前操作步数：" << opNum << '\t';
        }

        if (board.isFinished()){
            cout << "恭喜完成" << endl;
            cout << "按p再来一局" << endl;
            cin >> op;
            if (op == 'p') goto flag;
        }
    }

    system("pause");

    return 0;
}