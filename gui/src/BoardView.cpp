#include "BoardView.h"
#include "BoardController.h"
#include "MainWindow.h"
#include "ModeManager.h"
#include "Tool.h"
#include "ui_BoardView.h"
#include <memory>

//构造函数，初始化棋盘界面
BoardView::BoardView(MainWindow *parent):
    QWidget(parent),
    ui(new Ui::BoardView()),
    controller(new BoardController()),
    mainWindow(parent),
    animationTimer(new QTimer(this)),
    currentAnimationIndex(0),
    isAnimating(false)
{
    ui->setupUi(this);

    //设置背景图片
    QString bgPath = Tool::getImgPath("background/backgroundBoard.png");
    Tool::setBackground(this, bgPath);

    //初始化棋盘格子按钮数组
    cellButtons = {
        {ui->cell00, ui->cell01, ui->cell02, ui->cell03, ui->cell04, ui->cell05},
        {ui->cell10, ui->cell11, ui->cell12, ui->cell13, ui->cell14, ui->cell15},
        {ui->cell20, ui->cell21, ui->cell22, ui->cell23, ui->cell24, ui->cell25},
        {ui->cell30, ui->cell31, ui->cell32, ui->cell33, ui->cell34, ui->cell35},
        {ui->cell40, ui->cell41, ui->cell42, ui->cell43, ui->cell44, ui->cell45},
        {ui->cell50, ui->cell51, ui->cell52, ui->cell53, ui->cell54, ui->cell55}
    };

    //初始化动画定时器
    connect(animationTimer, &QTimer::timeout, this, &BoardView::animateNextCell);

    //使用lambda表达式连接行、列按钮信号到通用槽函数
    QPushButton* rowBtns[] = {ui->rowBtn0, ui->rowBtn1, ui->rowBtn2, ui->rowBtn3, ui->rowBtn4, ui->rowBtn5};
    QPushButton* colBtns[] = {ui->colBtn0, ui->colBtn1, ui->colBtn2, ui->colBtn3, ui->colBtn4, ui->colBtn5};
    for (int i=0; i<6; i++){
        connect(rowBtns[i], &QPushButton::clicked, this, [this, i]() {onRowButtonClicked(i);});
        connect(colBtns[i], &QPushButton::clicked, this, [this, i]() {onColButtonClicked(i);});
    }

    //控制翻转按钮设置图片
    for (int i=0; i<6; i++){
        QString rowBtnPath = Tool::getImgPath(QString("buttons/rowBtn%1.png").arg(i));
        Tool::setButtonImage(rowBtns[i], rowBtnPath);
        QString colBtnPath = Tool::getImgPath(QString("buttons/colBtn%1.png").arg(i));
        Tool::setButtonImage(colBtns[i], colBtnPath);
    }
    Tool::setButtonImage(ui->mdBtn, Tool::getImgPath("buttons/mdBtn.png"));
    Tool::setButtonImage(ui->sdBtn, Tool::getImgPath("buttons/sdBtn.png"));

    //功能按钮设置
    ui->backBtn->setStyleSheet(menuButtonStyle);
    ui->answerBtn->setStyleSheet(menuButtonStyle);
    ui->restartBtn->setStyleSheet(menuButtonStyle);
}

//析构函数，释放资源
BoardView::~BoardView(){
    delete controller;
    delete ui;
}

//设置游戏难度
void BoardView::setDifficulty(ModeManager::diff diff){
    controller->setDifficulty(diff);
}

//开始游戏，初始化棋盘
void BoardView::startGame(){
    controller->startGame();

    ModeManager::BoardConfig boardConfig = controller->getBoardConfig();

    //初始化难度标签
    QString diffText;
    switch (controller->getDifficulty()) {
        case ModeManager::diff::easy: diffText = "简单"; break;
        case ModeManager::diff::normal: diffText = "普通"; break;
        case ModeManager::diff::hard: diffText = "困难"; break;
        case ModeManager::diff::hardcore: diffText = "极限"; break;
    }
    ui->diffLabel->setText(diffText);
    ui->diffLabel->setStyleSheet(diffTextStyle);
    //根据难度显示/隐藏对角线按钮
    ui->mdBtn->setVisible(boardConfig.isMD);
    ui->sdBtn->setVisible(boardConfig.isSD);
    //显示连胜统计标签
    ui->streakLabel->setVisible(controller->getDifficulty() >= ModeManager::diff::hard);
    //清空消息标签
    ui->messageLabel->clear();
    //启用所有操作按钮
    setButtonsEnabled(true, true);

    QPushButton* rowBtns[] = {ui->rowBtn0, ui->rowBtn1, ui->rowBtn2, ui->rowBtn3, ui->rowBtn4, ui->rowBtn5};
    QPushButton* colBtns[] = {ui->colBtn0, ui->colBtn1, ui->colBtn2, ui->colBtn3, ui->colBtn4, ui->colBtn5};

    for (int i=0; i<6; i++){
        rowBtns[i]->setVisible(true);
        colBtns[i]->setVisible(true);
        for (int j=0; j<6; j++){
            cellButtons[i][j]->setVisible(true);
            cellButtons[i][j]->setEnabled(true);
        }
    }
    //禁用部分行列
    for (int i=boardConfig.rows; i<6; i++){
        rowBtns[i]->setEnabled(false);
        for (int j=0; j<6; j++){
            cellButtons[i][j]->setEnabled(false);
        }
    }
    for (int j=boardConfig.cols; j<6; j++){
        colBtns[j]->setEnabled(false);
        for (int i=0; i<6; i++){
            cellButtons[i][j]->setEnabled(false);
        }
    }

    //更新棋盘显示
    updateBoardDisplay();
}

//通用行按钮点击槽函数
void BoardView::onRowButtonClicked(int row){
    previousBoard = controller->getBoard();
    controller->turnRow(row);
    if (mainWindow->getIsEffectPlaying()){
        Tool::playAudio(Tool::getAudioPath("effect/turn.mp3"), Tool::effect, false);
        Tool::setAudioVolume(Tool::effect, 50);
    }
    startTurnAnimation();
}
//通用列按钮点击槽函数
void BoardView::onColButtonClicked(int col){    
    previousBoard = controller->getBoard();
    controller->turnCol(col);
    if (mainWindow->getIsEffectPlaying()){
        Tool::playAudio(Tool::getAudioPath("effect/turn.mp3"), Tool::effect, false);
        Tool::setAudioVolume(Tool::effect, 50);
    }
    startTurnAnimation();
}
//翻转主对角线点击槽函数
void BoardView::on_mdBtn_clicked(){    
    previousBoard = controller->getBoard();
    controller->turnMD();
    if (mainWindow->getIsEffectPlaying()){
        Tool::playAudio(Tool::getAudioPath("effect/turn.mp3"), Tool::effect, false);
        Tool::setAudioVolume(Tool::effect, 50);
    }
    startTurnAnimation();
}
//翻转副对角线点击槽函数
void BoardView::on_sdBtn_clicked(){    
    previousBoard = controller->getBoard();
    controller->turnSD();
    if (mainWindow->getIsEffectPlaying()){
        Tool::playAudio(Tool::getAudioPath("effect/turn.mp3"), Tool::effect, false);
        Tool::setAudioVolume(Tool::effect, 50);
    }
    startTurnAnimation();
}
//显示答案
void BoardView::on_answerBtn_clicked(){
    Tool::clickedAnimation(ui->answerBtn, mainWindow->getIsAnimation(), menuClickedButtonStyle);
    if (!(controller->isGameFinished())){
        controller->resetStreak();
    }
    QString answer = controller->getBestSolveString();
    int delayTime;
    if (mainWindow->getIsAnimation()) delayTime = 210;
    else delayTime = 0;
    QTimer::singleShot(delayTime, this, [this, answer](){
        ui->messageLabel->setText(answer);
        ui->messageLabel->setStyleSheet(messageTextStyle);
    });
}
//重新开始游戏
void BoardView::on_restartBtn_clicked(){
    Tool::clickedAnimation(ui->restartBtn, mainWindow->getIsAnimation(), menuClickedButtonStyle);
    int delayTime;
    if (mainWindow->getIsAnimation()) delayTime = 250;
    else delayTime = 0;
    QTimer::singleShot(delayTime, this, [this](){
        controller->restartGame();
        startGame();
    });
}
//返回菜单
void BoardView::on_backBtn_clicked(){
    Tool::clickedAnimation(ui->backBtn, mainWindow->getIsAnimation(), menuClickedButtonStyle);
    int delayTime;
    if (mainWindow->getIsAnimation()) delayTime = 300;
    else delayTime = 0;
    QTimer::singleShot(delayTime, this, [this](){
        emit backToMenu();
    });
}

//更新棋盘显示
void BoardView::updateBoardDisplay(){
    std::vector<std::vector<bool>> board = controller->getBoard();
    int row = controller->getBoardConfig().rows;
    int col = controller->getBoardConfig().cols;

    //加载拼图正面和反面大图路径
    QString cellOnPath = Tool::getImgPath("board/cellOn.png");
    QString cellOffPath = Tool::getImgPath("board/cellOff.png");
    
    //更新每个格子的图片
    for (int i=0; i<6; i++){
        for (int j=0; j<6; j++){
            //确保索引不越界
            if (i < row && j < col){
                if (board[i][j]){
                    //激活状态（值为1） 显示正面图片
                    Tool::setPuzzleBoardImage(cellButtons[i][j], cellOnPath, i, j, 6, 6);
                }
                else{
                    //关闭状态（值为0） 显示反面图片
                    Tool::setPuzzleBoardImage(cellButtons[i][j], cellOffPath, i, j, 6, 6);
                }
            }
            else{
                //超出棋盘范围的格子显示默认正面
                Tool::setPuzzleBoardImage(cellButtons[i][j], cellOnPath, i, j, 6, 6);
            }
        }
    }

    updateStepDisplay();
    
    //更新连胜统计显示
    if (controller->getDifficulty() >= ModeManager::diff::hard){
        updateStreakDisplay();
    }
}

//更新步数标签
void BoardView::updateStepDisplay(){
    ui->stepLabel->setText(
        QString("%1\n\n%2")
        .arg(controller->getPlayerStep(), 2, 10, '0')
        .arg(controller->getInitialMinStep(), 2, 10, '0')
    );
    ui->stepLabel->setStyleSheet(stepTextStyle);
}

//更新连胜统计显示
void BoardView::updateStreakDisplay(){
    QString streakText = 
        QString("连胜    %1\n最高    %2")
        .arg(controller->getCurrentStreak())
        .arg(controller->getMaxStreak(controller->getDifficulty()));
    ui->streakLabel->setText(streakText);
    ui->streakLabel->setStyleSheet(streakTextStyle);
}

//检查游戏状态
void BoardView::checkGameState(){
    //检查是否胜利
    if (controller->isGameFinished()){
        ui->messageLabel->setText(
            QString("游戏胜利！\n共 %1 步完成 (最佳: %2)")
            .arg(controller->getPlayerStep())
            .arg(controller->getInitialMinStep())
        );
        ui->messageLabel->setStyleSheet(messageTextStyle);
        ui->messageLabel->setStyleSheet("color: rgba(30, 255, 30, 0.7);");
        setButtonsEnabled(false, true);
        if (mainWindow->getIsEffectPlaying()) Tool::playAudio(Tool::getAudioPath("effect/finish.mp3"), Tool::effect, false);
        if (mainWindow->getIsAnimation()) startFinishAnimation();
    }
    //检查是否失败
    else if (controller->isGameFailed()){
        ui->messageLabel->setText("步数超过限制\n游戏失败！");
        ui->messageLabel->setStyleSheet(messageTextStyle);
        ui->messageLabel->setStyleSheet("color: rgba(255, 50, 0, 0.7);");
        setButtonsEnabled(false, true);
    }
}

//设置操作按钮的启用状态
void BoardView::setButtonsEnabled(bool enabled, bool setVisual){
    QPushButton* rowBtns[] = {ui->rowBtn0, ui->rowBtn1, ui->rowBtn2, ui->rowBtn3, ui->rowBtn4, ui->rowBtn5};
    QPushButton* colBtns[] = {ui->colBtn0, ui->colBtn1, ui->colBtn2, ui->colBtn3, ui->colBtn4, ui->colBtn5};
    //启用/禁用行和列按钮
    for (int i=0; i<6; i++){
        if (setVisual){
            rowBtns[i]->setEnabled(enabled);
            colBtns[i]->setEnabled(enabled);
        } 
        else{
            rowBtns[i]->setAttribute(Qt::WA_TransparentForMouseEvents, !enabled);
            colBtns[i]->setAttribute(Qt::WA_TransparentForMouseEvents, !enabled);
        }
    }
    //启用/禁用对角线按钮
    if (setVisual){
        ui->mdBtn->setEnabled(enabled);
        ui->sdBtn->setEnabled(enabled);
    }
    else{
        ui->mdBtn->setAttribute(Qt::WA_TransparentForMouseEvents, !enabled);
        ui->sdBtn->setAttribute(Qt::WA_TransparentForMouseEvents, !enabled);
    }
    //功能按钮始终保持启用状态
    ui->answerBtn->setEnabled(true);
    ui->restartBtn->setEnabled(true);
    ui->backBtn->setEnabled(true);
}

//翻转动画
void BoardView::startTurnAnimation(){
    if (mainWindow->getIsAnimation()){
        isAnimating = true;
        setButtonsEnabled(false, false);

        cellsToAnimate.clear();
        currentAnimationIndex = 0;
        
        std::vector<std::vector<bool>> currentBoard = controller->getBoard();
        int rows = controller->getBoardConfig().rows;
        int cols = controller->getBoardConfig().cols;
        
        //逐个翻转
        for (int j=0; j<cols; j++){
            for (int i=0; i<rows; i++){
                if (i < rows && j < cols && previousBoard[i][j] != currentBoard[i][j]){
                    cellsToAnimate.push_back(std::make_pair(i, j));
                }
            }
        }
        
        if (!cellsToAnimate.empty()){
            animationTimer->start(175);
        }
        else{
            updateBoardDisplay();
            checkGameState();
            setButtonsEnabled(true, false);
            isAnimating = false;
        }
    }
    else{
        updateBoardDisplay();
        checkGameState();
    }
}
//更新单个cell的显示
void BoardView::updateSingleCell(int i, int j){
    std::vector<std::vector<bool>> board = controller->getBoard();
    
    QString cellOnPath = Tool::getImgPath("board/cellOn.png");
    QString cellOffPath = Tool::getImgPath("board/cellOff.png");
    
    if (i < static_cast<int>(board.size()) && j < static_cast<int>(board[i].size())){
        if (board[i][j]){
            Tool::setPuzzleBoardImage(cellButtons[i][j], cellOnPath, i, j, 6, 6);
        }
        else{
            Tool::setPuzzleBoardImage(cellButtons[i][j], cellOffPath, i, j, 6, 6);
        }
    }
}
//为cell添加动画
void BoardView::addAnimation(QPushButton* btn){
    if (!btn) return;

    //创建一个黑色透明的覆盖层
    QWidget* overlay = Tool::createOverlay(btn, btn->rect(), "background-color: #000000;");
    QGraphicsOpacityEffect* opacityEffect = static_cast<QGraphicsOpacityEffect*>(overlay->graphicsEffect());
    overlay->show();
    
    //创建顺序动画组
    auto seqGroup = std::make_unique<QSequentialAnimationGroup>(overlay);
    
    //创建顺序动画组
    seqGroup->addAnimation(Tool::createOpacityAnimation(opacityEffect, 0.0, 0.6, 100, QEasingCurve::InQuad).release());
    
    //宽度缩小动画
    auto shrinkAnim = std::make_unique<QPropertyAnimation>(overlay, "geometry");
    QRect startRect = overlay->geometry();
    QRect endRect = QRect(
        startRect.center().x(), startRect.top(),
        0, startRect.height()
    );
    shrinkAnim->setStartValue(startRect);
    shrinkAnim->setEndValue(endRect);
    shrinkAnim->setDuration(200);
    shrinkAnim->setEasingCurve(QEasingCurve::InOutQuad);
    seqGroup->addAnimation(shrinkAnim.release());

    //宽度放大动画
    auto expandAnim = std::make_unique<QPropertyAnimation>(overlay, "geometry");
    QRect expandStartRect = QRect(
        startRect.center().x(), startRect.top(),
        0, startRect.height()
    );
    QRect expandEndRect = startRect;
    expandAnim->setStartValue(expandStartRect);
    expandAnim->setEndValue(expandEndRect);
    expandAnim->setDuration(200);
    expandAnim->setEasingCurve(QEasingCurve::InOutQuad);
    seqGroup->addAnimation(expandAnim.release());

    //淡出动画
    seqGroup->addAnimation(Tool::createOpacityAnimation(opacityEffect, 0.6, 0.0, 100, QEasingCurve::OutQuad).release());
    
    //动画结束后删除覆盖层
    connect(seqGroup.get(), &QSequentialAnimationGroup::finished, overlay, &QWidget::deleteLater);
    
    //开始动画
    seqGroup->start(QAbstractAnimation::DeleteWhenStopped);
    seqGroup.release();
}
//下一个cell动画函数
void BoardView::animateNextCell(){
    if (currentAnimationIndex <static_cast<int>(cellsToAnimate.size())){
        int i = cellsToAnimate[currentAnimationIndex].first;
        int j = cellsToAnimate[currentAnimationIndex].second;

        updateSingleCell(i, j);//更新cell显示
        addAnimation(cellButtons[i][j]);//添加动画效果
        
        currentAnimationIndex++;
    }
    else{
        animationTimer->stop();
        updateBoardDisplay();//更新步数标签
        checkGameState();//检查游戏状态
        setButtonsEnabled(true, false);//重新启用按钮
        isAnimating = false;
    }
}

//完成动画
void BoardView::startFinishAnimation(){
    //开始动画
    auto parallelGroup = std::make_unique<QParallelAnimationGroup>(this);
    
    // 执行边框动画
    QSequentialAnimationGroup* borderGroup = borderAnimation();
    if (borderGroup) {
        parallelGroup->addAnimation(borderGroup);
    }
    // 执行全屏动画
    QSequentialAnimationGroup* shineGroup = shineAnimation();
    if (shineGroup) {
        parallelGroup->addAnimation(shineGroup);
    }
    
    //动画结束后清理
    connect(parallelGroup.get(), &QParallelAnimationGroup::finished, parallelGroup.get(), &QObject::deleteLater);
    
    parallelGroup->start(QAbstractAnimation::DeleteWhenStopped);
    parallelGroup.release();
}

QSequentialAnimationGroup* BoardView::borderAnimation(){
    //计算棋盘的边界
    QRect boardRect;
    auto calculateButtonBounds = [&boardRect, this](QPushButton* btn){
        if (btn){
            QPoint topLeft = btn->mapTo(this, QPoint(0,0));
            QRect btnRect(topLeft, btn->size());
            if (boardRect.isEmpty()){
                boardRect = btnRect;
            }
            else{
                boardRect = boardRect.united(btnRect);
            }
        }
    };
    for (const auto& row : cellButtons){
        for (QPushButton* btn : row){
            calculateButtonBounds(btn);
        }
    }
    if (boardRect.isEmpty()) return nullptr;

    //创建边框动画
    QWidget* borderBox = new QWidget(this);
    borderBox->setStyleSheet("border: 1px solid #ffffff; background-color: transparent;");
    borderBox->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    borderBox->setAttribute(Qt::WA_TransparentForMouseEvents);
    
    //添加透明度效果
    auto borderEffect = std::make_unique<QGraphicsOpacityEffect>(borderBox);
    borderEffect->setOpacity(0.0);
    borderBox->setGraphicsEffect(borderEffect.get());
    QGraphicsOpacityEffect* borderEffectPtr = borderEffect.release();

    //边框动画参数
    QRect startRect = boardRect;
    borderBox->setGeometry(startRect);
    borderBox->show();

    //结束矩形（以棋盘中心向外扩展）
    int expandSize = 250;
    QPoint center = startRect.center();
    QRect endRect = QRect(
        center.x() - (startRect.width() + expandSize) / 2,
        center.y() - (startRect.height() + expandSize) / 2,
        startRect.width() + expandSize,
        startRect.height() + expandSize
    );

    //创建边框动画组
    auto borderGroup = std::make_unique<QSequentialAnimationGroup>();
    borderGroup->addAnimation(Tool::createOpacityAnimation(borderEffectPtr, 0.0, 0.5, 300, QEasingCurve::InQuad).release());
    
    //创建边框宽度动画
    auto borderWidthAnim = std::make_unique<QPropertyAnimation>();
    borderWidthAnim->setStartValue(10);
    borderWidthAnim->setEndValue(60);
    borderWidthAnim->setDuration(700);
    borderWidthAnim->setEasingCurve(QEasingCurve::OutQuad);
    
    //连接边框宽度变化信号
    connect(borderWidthAnim.get(), &QPropertyAnimation::valueChanged, [borderBox](const QVariant& value){
        int width = value.toInt();
        borderBox->setStyleSheet(
            QString("border: %1px solid #ffffff; background-color: transparent;")
            .arg(width)
        );
    });
    
    //几何动画（缩放）
    auto scaleAnim = Tool::createScaleAnimation(borderBox, startRect, endRect, 1000, QEasingCurve::OutQuad);
    
    //创建并行动画组（同时执行边框宽度和缩放动画）
    auto parallelAnim = std::make_unique<QParallelAnimationGroup>();
    parallelAnim->addAnimation(borderWidthAnim.release());
    parallelAnim->addAnimation(scaleAnim.release());
    borderGroup->addAnimation(parallelAnim.release());
    
    borderGroup->addAnimation(Tool::createOpacityAnimation(borderEffectPtr, 0.5, 0.0, 300, QEasingCurve::OutQuad).release());

    //动画结束后清理
    connect(borderGroup.get(), &QSequentialAnimationGroup::finished, borderBox, &QWidget::deleteLater);
    
    return borderGroup.release();
}

QSequentialAnimationGroup* BoardView::shineAnimation(){
//创建全屏动画
    QWidget* overlay = Tool::createOverlay(this, rect(), "background-color: #ffffff;");
    QGraphicsOpacityEffect* overlayEffect = static_cast<QGraphicsOpacityEffect*>(overlay->graphicsEffect());
    overlay->show();

    //创建全屏动画组
    auto overlayGroup = std::make_unique<QSequentialAnimationGroup>();
    
    overlayGroup->addAnimation(Tool::createOpacityAnimation(overlayEffect, 0.0, 0.9, 200, QEasingCurve::InQuad).release());
    overlayGroup->addAnimation(Tool::createOpacityAnimation(overlayEffect, 0.9, 0.0, 200, QEasingCurve::OutCubic).release());

    //动画结束后清理
    connect(overlayGroup.get(), &QSequentialAnimationGroup::finished, overlay, &QWidget::deleteLater);
    
    return overlayGroup.release();
}
