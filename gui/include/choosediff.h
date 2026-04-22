#ifndef CHOOSEDIFF_H
#define CHOOSEDIFF_H

#include "ModeManager.h"
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
    void difficultySelected(ModeManager::diff diff);

private slots:
    //通用难度按钮点击槽函数
    void onDiffButtonClicked(ModeManager::diff diff);

private:    
    Ui::ChooseDiff *ui;//UI界面指针
    MainWindow* mainWindow;//主窗口指针

    QString chooseButtonStyle = "background-color: rgba(255, 255, 255, 0.1); color: rgba(255, 255, 255, 0.7); font-family:'Century Schoolbook'; font-size: 32px;";
    QString chooseClickedButtonStyle = "background-color: rgba(0, 0, 0, 0.1); color: rgba(255, 255, 255, 0.8); font-family:'Century Schoolbook'; font-size: 32px;";
    QString LabelStyle = "color: rgba(255, 255, 255, 0.7); font-family:'Century Schoolbook'; font-size: 8px;";
};

#endif // CHOOSEDIFF_H
