#ifndef CHOOSEDIFF_H
#define CHOOSEDIFF_H

#include <QWidget>

namespace Ui {
class ChooseDiff;
}

//难度选择界面类
class ChooseDiff : public QWidget
{
    Q_OBJECT

public:
    //构造函数
    explicit ChooseDiff(QWidget *parent = nullptr);
    //析构函数
    ~ChooseDiff();

signals:
    //难度选择信号，发送选择的难度
    void difficultySelected(int diff);

private:
    Ui::ChooseDiff *ui;//UI界面指针
};

#endif // CHOOSEDIFF_H
