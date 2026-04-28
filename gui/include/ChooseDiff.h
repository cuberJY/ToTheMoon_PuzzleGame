#ifndef CHOOSEDIFF_H
#define CHOOSEDIFF_H

#include "RandomMode.h"
#include <QWidget>

namespace Ui{
class ChooseDiff;
}
class MainWindow;
//难度选择界面类
class ChooseDiff : public QWidget
{
    Q_OBJECT

public:
    //构造函数
    explicit ChooseDiff(MainWindow *parent = nullptr);
    //析构函数
    ~ChooseDiff();

signals:
    //难度选择信号，发送选择的难度
    void difficultySelected(Diff diff);

private slots:
    //通用难度按钮点击槽函数
    void onDiffButtonClicked(Diff diff);

private:    
    Ui::ChooseDiff *ui;
    MainWindow* mainWindow;
};

#endif // CHOOSEDIFF_H
