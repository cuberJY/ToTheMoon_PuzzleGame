#include "Home.h"
#include "MainWindow.h"
#include "Tool.h"
#include "ui_Home.h"
#include <QTimer>

Home::Home(MainWindow *parent)
    : QWidget(parent)
    , ui(new Ui::Home)
    , mainWindow(parent)
{
    ui->setupUi(this);

    QString bgPath = Tool::getImgPath("background/backgroundRandom.png");
    Tool::setBackground(this, bgPath);

    ui->levelButton->setStyleSheet(Tool::chooseButtonStyle);
    ui->randomButton->setStyleSheet(Tool::chooseButtonStyle);
    connect(ui->levelButton, &QPushButton::clicked, this, [this]() {on_levelButton_clicked();});
    connect(ui->randomButton, &QPushButton::clicked, this, [this]() {on_randomButton_clicked();});
}

Home::~Home()
{
    delete ui;
}

void Home::on_levelButton_clicked(){
    Tool::clickedAnimation(ui->levelButton, mainWindow->getIsAnimation(), Tool::chooseClickedButtonStyle);
    int delayTime;
    if (mainWindow->getIsAnimation()) delayTime = 225;
    else delayTime = 0;
    QTimer::singleShot(delayTime, this, [this](){
        emit levelModeSelected();
    });
}

void Home::on_randomButton_clicked(){
    Tool::clickedAnimation(ui->randomButton, mainWindow->getIsAnimation(), Tool::chooseClickedButtonStyle);
    int delayTime;
    if (mainWindow->getIsAnimation()) delayTime = 225;
    else delayTime = 0;
    QTimer::singleShot(delayTime, this, [this](){
        emit randomModeSelected();
    });
}

