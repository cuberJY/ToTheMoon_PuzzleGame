#include "ChooseLevel.h"
#include "MainWindow.h"
#include "Tool.h"
#include "ui_ChooseLevel.h"
#include <QTimer>

ChooseLevel::ChooseLevel(MainWindow *parent)
    : QWidget(parent)
    , ui(new Ui::ChooseLevel)
    , mainWindow(parent)
{
    ui->setupUi(this);

    QString bgPath = Tool::getImgPath("background/backgroundHome.png");
    Tool::setBackground(this, bgPath);

    QPushButton* levelBtns[] = {
        ui->level01, ui->level02, ui->level03, ui->level04, ui->level05,
        ui->level06, ui->level07, ui->level08, ui->level09, ui->level10,
        ui->level11, ui->level12, ui->level13, ui->level14, ui->level15
    };

    for (int i=0; i<15; i++){
        levelBtns[i]->setStyleSheet(Tool::chooseButtonStyle);
        connect(levelBtns[i], &QPushButton::clicked, this, [this, i]() {onLevelButtonClicked(i);});
    }

    for (int i=0; i<15; i++){
        setTabOrder(levelBtns[i], levelBtns[i+1]);
    }
}

ChooseLevel::~ChooseLevel()
{
    delete ui;
}

void ChooseLevel::refresh(){
    QPushButton* levelBtns[] ={
        ui->level01, ui->level02, ui->level03, ui->level04, ui->level05,
        ui->level06, ui->level07, ui->level08, ui->level09, ui->level10,
        ui->level11, ui->level12, ui->level13, ui->level14, ui->level15
    };
    int maxLevel = mainWindow->getPlayerMaxLevel();
    
    for (int i=0; i<15; i++){
        if (i <= maxLevel){
            levelBtns[i]->setEnabled(true);
            levelBtns[i]->setStyleSheet(Tool::chooseButtonStyle);
        }
        else{
            levelBtns[i]->setEnabled(false);
            levelBtns[i]->setStyleSheet(Tool::chooseDisabledButtonStyle);
        }
    }
    
}
void ChooseLevel::onLevelButtonClicked(int level){
    QPushButton* levelBtns[] = {
        ui->level01, ui->level02, ui->level03, ui->level04, ui->level05,
        ui->level06, ui->level07, ui->level08, ui->level09, ui->level10,
        ui->level11, ui->level12, ui->level13, ui->level14, ui->level15
    };
    QPushButton* clickedBtn = levelBtns[level];
    if (clickedBtn){
        Tool::clickedAnimation(clickedBtn, mainWindow->getIsAnimation(), Tool::chooseClickedButtonStyle);
    }

    int delayTime;
    if (mainWindow->getIsAnimation()) delayTime = 225;
    else delayTime = 0;
    QTimer::singleShot(delayTime, this, [this, level](){
        emit levelSelected(level);
    });
}
