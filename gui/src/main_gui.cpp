#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);//创建QApplication对象，用于处理命令行参数和事件循环

    MainWindow window;//创建MainWindow对象
    window.show();//显示窗口

    return app.exec();//执行事件循环，等待用户交互
}
