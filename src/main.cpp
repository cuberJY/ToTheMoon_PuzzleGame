#include <PuzzleBoard.h>
#include <ModeManager.h>

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <windows.h>
using namespace std;

int main(){
    // 设置控制台输出为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
    // 可选：同时设置输入编码为 UTF-8（如果需要读中文）
    SetConsoleCP(CP_UTF8);

    while (true){
        ModeManager manager;
        manager.setMode(0);
        //选择难度
        int diff;
        cout << "请选择难度：0-简单 1-普通 2-困难 3-硬核（必须以最少步数完成）" << endl;
        cin >> diff;
        manager.setRandomDiff(diff);

        PuzzleBoard board = manager.createBoard();
        
        board.printBoard();
        int minStep = board.getMinStep();
        cout << "最佳步数：" << minStep << endl;

        //输入操作
        char input;

        while (!board.isFinished()){
            cout << "qwerty翻转1-6行，asdfgh翻转1-6列，zx翻转副对角线和主对角线，n获得答案" << endl;
            cin >> input;
            //操作
            switch (input){
                case 'q': board.turnRow(0); break;
                case 'w': board.turnRow(1); break;
                case 'e': board.turnRow(2); break;
                case 'r': board.turnRow(3); break;
                case 't': board.turnRow(4); break;
                case 'y': board.turnRow(5); break;

                case 'a': board.turnCol(0); break;
                case 's': board.turnCol(1); break;
                case 'd': board.turnCol(2); break;
                case 'f': board.turnCol(3); break;
                case 'g': board.turnCol(4); break;
                case 'h': board.turnCol(5); break;

                case 'z': board.turnSD(); break;
                case 'x': board.turnMD(); break;

                case 'n': board.printBestSolve(); break;
                default: cout << "无效输入，请重新输入" << endl; continue;
            }

            board.printBoard();
            cout << "最佳步数：" << minStep << "，当前步数：" << board.getPlayerStep() << endl;

            if (manager.getRandomDiff() == 3 && board.getMinStep() > (minStep - board.getPlayerStep())){
                cout << "当前步数超过最佳步数，游戏失败" << endl;
                break;
            }
        }

        cout << "游戏胜利，共" << board.getPlayerStep() << "步完成，按p再来一局,按其他键退出游戏" << endl;

        char replay;
        cin >> replay;
        if (replay != 'p') {
            break;
        }
    }

    return 0;
}