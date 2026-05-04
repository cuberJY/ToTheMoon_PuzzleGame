#include "BoardView.h"
#include "BoardController.h"
#include "MainWindow.h"
#include "Tool.h"
#include "ui_BoardView.h"
#include "RandomMode.h"
#include "LevelMode.h"
#include <random>
#include <memory>

//构造函数，初始化拼图界面
BoardView::BoardView(MainWindow *parent):
    QWidget(parent),
    ui(new Ui::BoardView()),
    controller(nullptr),
    mainWindow(parent),
    animationTimer(new QTimer(this)),
    currentAnimationIndex(0),
    isAnimating(false),
    countdownTimer(new QTimer(this)),
    remainingTime(0)
{
    ui->setupUi(this);

    //设置背景图片
    QString bgPath = Tool::getImgPath("background/backgroundBoard.png");
    Tool::setBackground(this, bgPath);

    cellOnPixmap.load(Tool::getImgPath("board/cellOn.png"));
    cellOffPixmap.load(Tool::getImgPath("board/cellOff.png"));
    //预加载拼图块图片
    preloadPiecePixmaps();

    //初始化拼图格子按钮数组
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
    //初始化倒计时定时器
    connect(countdownTimer, &QTimer::timeout, this, &BoardView::onCountdownTick);

    //使用lambda表达式连接行、列按钮信号到通用槽函数
    QPushButton* rowBtns[] = {ui->rowBtn0, ui->rowBtn1, ui->rowBtn2, ui->rowBtn3, ui->rowBtn4, ui->rowBtn5};
    QPushButton* colBtns[] = {ui->colBtn0, ui->colBtn1, ui->colBtn2, ui->colBtn3, ui->colBtn4, ui->colBtn5};
    for (int i=0; i<6; i++){
        connect(rowBtns[i], &QPushButton::clicked, this, [this, i]() {onRowButtonClicked(i);});
        connect(colBtns[i], &QPushButton::clicked, this, [this, i]() {onColButtonClicked(i);});
    }

    //设置控制翻转按钮图片
    for (int i=0; i<6; i++){
        QString rowBtnPath = Tool::getImgPath(QString("buttons/rowBtn%1.png").arg(i));
        Tool::setButtonImage(rowBtns[i], rowBtnPath);
        QString colBtnPath = Tool::getImgPath(QString("buttons/colBtn%1.png").arg(i));
        Tool::setButtonImage(colBtns[i], colBtnPath);
    }
    Tool::setButtonImage(ui->mdBtn, Tool::getImgPath("buttons/mdBtn.png"));
    Tool::setButtonImage(ui->sdBtn, Tool::getImgPath("buttons/sdBtn.png"));

    //设置功能按钮样式
    ui->backBtn->setStyleSheet(Tool::menuButtonStyle);
    ui->functionBtn1->setStyleSheet(Tool::menuButtonStyle);
    ui->functionBtn2->setStyleSheet(Tool::menuButtonStyle);

    //禁用所有格子按钮的键盘焦点
    for (auto& row : cellButtons){
        for (QPushButton* cell : row){
            cell->setFocusPolicy(Qt::NoFocus);
        }
    }
    //设置自定义焦点链顺序: sdBtn → rowBtns → mdBtn → colBtns → functionBtns
    setTabOrder(ui->sdBtn, rowBtns[0]);
    for (int i=5; i>0; i--){
        setTabOrder(rowBtns[i], rowBtns[i-1]);
    }
    setTabOrder(rowBtns[0], ui->mdBtn);
    setTabOrder(ui->mdBtn, colBtns[0]);
    for (int i=0; i<5; i++){
        setTabOrder(colBtns[i], colBtns[i+1]);
    }
    setTabOrder(colBtns[5], ui->functionBtn1);
    setTabOrder(ui->functionBtn1, ui->functionBtn2);
    setTabOrder(ui->functionBtn2, ui->backBtn);
}

//析构函数，释放资源
BoardView::~BoardView(){
    delete ui;
}

void BoardView::setController(BoardController* c){
    controller = c;
}
//设置游戏模式
void BoardView::setGameMode(GameMode* mode){
    controller->setGameMode(mode);
}
//获取游戏模式类型
ModeType BoardView::getCurrentMode(){
    return controller->getCurrentMode();
}
//设置游戏难度
void BoardView::setDiff(Diff diff){
    controller->setDiff(diff);
}
//设置游戏等级
void BoardView::setLevel(int level){
    controller->setLevel(level);
}

//开始游戏，初始化拼图
void BoardView::startGame(){
    controller->startGame();

    BoardConfig boardConfig = controller->getBoardConfig();

    //预加载和缓存图片
    if (controller->getCurrentMode() == ModeType::Random){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 16);
        int n = dis(gen);

        if (n==16){
            cellOnPixmap.load(Tool::getImgPath("board/cellOn.png"));
        }
        else{
            cellOnPixmap.load(Tool::getImgPath(QString("board/level%1.png").arg(n, 2, 10, '0')));
        }
    }
    else if (controller->getCurrentMode() == ModeType::Level){
        cellOnPixmap.load(Tool::getImgPath(
            QString("board/level%1.png")
            .arg(controller->getCurrentLevel()+1, 2, 10, '0')
        ));
    }
    preloadPiecePixmaps();

    //初始化标签
    QString labelText;
    if (getCurrentMode() == ModeType::Random){
        switch (controller->getCurrentDiff()) {
            case Diff::easy: labelText = "简单"; break;
            case Diff::normal: labelText = "普通"; break;
            case Diff::hard: labelText = "困难"; break;
            case Diff::hardcore: labelText = "极限"; break;
        }
    }
    else if (getCurrentMode() == ModeType::Level){
        labelText = QString("第%1关").arg(controller->getCurrentLevel()+1);
    }
    ui->titleLabel->setText(labelText);
    ui->titleLabel->setStyleSheet(Tool::titleTextStyle);
    ui->countDownLabel->setText("倒计时:");
    ui->countDownLabel->setStyleSheet(Tool::countDownTextStyle);
    updateRemStepLabel();

    //初始化倒计时
    if (controller->isCountdownEnabled()){
        remainingTime = boardConfig.limitTime;
        setTimeLabel();
        ui->timeLabel->setStyleSheet(Tool::timeTextStyle);
        ui->timeLabel->setVisible(true);
        countdownTimer->start(1000);
    }
    else{
        ui->countDownLabel->setVisible(false);
        ui->timeLabel->setVisible(false);
        countdownTimer->stop();
    }

    //显示/隐藏剩余步数标签
    ui->remstepLabel->setVisible(boardConfig.limitStep >= 0);
    //根据难度显示/隐藏对角线按钮
    ui->sdBtn->setVisible(boardConfig.isSD);
    //显示连胜统计标签
    ui->streakLabel->setVisible(getCurrentMode() == ModeType::Random && (boardConfig.limitStep >= 0 || boardConfig.limitTime >= 0));
    //清空消息标签
    ui->messageLabel->clear();

    //启用所有操作按钮
    setButtonsEnabled(true, true);
    //显示所有行、列按钮和拼图格子
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

    //根据当前模式设置返回按钮文本
    if (getCurrentMode() == ModeType::Random){
        ui->functionBtn1->setText("显示答案");
        ui->functionBtn2->setText("再来一局");
        ui->backBtn->setText("切换难度");
    }
    else if (getCurrentMode() == ModeType::Level){
        ui->functionBtn1->setText("上一关");
        ui->functionBtn2->setText("下一关");
        ui->backBtn->setText("切换关卡");
    }

    //更新拼图显示
    updateBoardDisplay();
}

//通用行按钮点击槽函数
void BoardView::onRowButtonClicked(int row){
    previousBoard = controller->getBoard();
    controller->turnRow(row);
    if (mainWindow->getIsEffectPlaying() && mainWindow->getIsAnimation()){
        Tool::playAudio(Tool::getAudioPath("effect/turn.mp3"), Tool::effect, false);
        Tool::setAudioVolume(Tool::effect, 50);
    }
    startTurnAnimation();
}
//通用列按钮点击槽函数
void BoardView::onColButtonClicked(int col){    
    previousBoard = controller->getBoard();
    controller->turnCol(col);
    if (mainWindow->getIsEffectPlaying() && mainWindow->getIsAnimation()){
        Tool::playAudio(Tool::getAudioPath("effect/turn.mp3"), Tool::effect, false);
        Tool::setAudioVolume(Tool::effect, 50);
    }
    startTurnAnimation();
}
//翻转主对角线点击槽函数
void BoardView::on_mdBtn_clicked(){    
    previousBoard = controller->getBoard();
    controller->turnMD();
    if (mainWindow->getIsEffectPlaying() && mainWindow->getIsAnimation()){
        Tool::playAudio(Tool::getAudioPath("effect/turn.mp3"), Tool::effect, false);
        Tool::setAudioVolume(Tool::effect, 50);
    }
    startTurnAnimation();
}
//翻转副对角线点击槽函数
void BoardView::on_sdBtn_clicked(){    
    previousBoard = controller->getBoard();
    controller->turnSD();
    if (mainWindow->getIsEffectPlaying() && mainWindow->getIsAnimation()){
        Tool::playAudio(Tool::getAudioPath("effect/turn.mp3"), Tool::effect, false);
        Tool::setAudioVolume(Tool::effect, 50);
    }
    startTurnAnimation();
}
//显示答案/上一关槽函数
void BoardView::on_functionBtn1_clicked(){
    Tool::clickedAnimation(ui->functionBtn1, mainWindow->getIsAnimation(), Tool::menuClickedButtonStyle);
    int delayTime;
    if (mainWindow->getIsAnimation()) delayTime = 210;
    else delayTime = 0;

    if (getCurrentMode() == ModeType::Random){
        if (!controller->isGameFinished()){
            controller->resetStreak();
            updateStreakDisplay();
        }
        QString answer = controller->getBestSolveString();
        QTimer::singleShot(delayTime, this, [this, answer](){
            ui->messageLabel->setText(answer);
            ui->messageLabel->setStyleSheet(Tool::messageTextStyle);
        });
    }
    else if (getCurrentMode() == ModeType::Level){
        QTimer::singleShot(delayTime, this, [this](){
            controller->prevLevel();
            startGame();
        });
    }
}
//重新开始/下一关游戏槽函数
void BoardView::on_functionBtn2_clicked(){
    Tool::clickedAnimation(ui->functionBtn2, mainWindow->getIsAnimation(), Tool::menuClickedButtonStyle);
    int delayTime;
    if (mainWindow->getIsAnimation()) delayTime = 210;
    else delayTime = 0;

    
    QTimer::singleShot(delayTime, this, [this](){
        if (getCurrentMode() == ModeType::Random){
            controller->restartGame();
            startGame();
        }
        else if (getCurrentMode() == ModeType::Level){
            if (controller->isGameFailed()){
                controller->restartGame();
                startGame();
            }
            else if (controller->isGameFinished() || controller->getCurrentLevel() < controller->getPlayerMaxLevel()){
                controller->nextLevel();
                startGame();
            } 
            else{
                ui->messageLabel->setText("当前关卡未完成");
                ui->messageLabel->setStyleSheet(Tool::messageTextStyle);
                ui->messageLabel->setStyleSheet("color: rgba(255, 127, 0, 0.7); font-size: 20px;");
            }
        }
    });
}
//返回菜单槽函数
void BoardView::on_backBtn_clicked(){
    Tool::clickedAnimation(ui->backBtn, mainWindow->getIsAnimation(), Tool::menuClickedButtonStyle);
    countdownTimer->stop();
    int delayTime;
    if (mainWindow->getIsAnimation()) delayTime = 300;
    else delayTime = 0;
    QTimer::singleShot(delayTime, this, [this](){
        emit backToMenu();
    });
}
//倒计时
void BoardView::onCountdownTick(){
    if (mainWindow->getIsAnimation()) remainingTime--;
    else remainingTime -= 2;
    setTimeLabel();
}

//预加载拼图块图片
void BoardView::preloadPiecePixmaps(){
    //计算拼图块的宽度和高度
    int onPieceWidth = cellOnPixmap.width() / 6;
    int onPieceHeight = cellOnPixmap.height() / 6;
    int offPieceWidth = cellOffPixmap.width() / 6;
    int offPieceHeight = cellOffPixmap.height() / 6;
    
    //预加载所有拼图块
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 6; col++) {
            int xOn = col * onPieceWidth;
            int yOn = (5 - row) * onPieceHeight;//row从0到5，但图片从上到下，需要倒序处理
            cellOnPiecePixmaps[row][col] = cellOnPixmap.copy(xOn, yOn, onPieceWidth, onPieceHeight);
            
            int xOff = col * offPieceWidth;
            int yOff = (5 - row) * offPieceHeight;
            cellOffPiecePixmaps[row][col] = cellOffPixmap.copy(xOff, yOff, offPieceWidth, offPieceHeight);
        }
    }
}

//更新拼图显示
void BoardView::updateBoardDisplay(){
    //获取当前拼图状态
    std::vector<std::vector<bool>> board = controller->getBoard();
    BoardConfig boardConfig = controller->getBoardConfig();
    int row = boardConfig.rows;
    int col = boardConfig.cols;

    //更新每个格子的图片（使用缓存图片）
    for (int i=0; i<6; i++){
        for (int j=0; j<6; j++){
            //确保索引不越界
            if (i < row && j < col){
                if (board[i][j]){
                    //激活状态（值为1）显示正面图片
                    Tool::setPuzzleBoardImage(cellButtons[i][j], cellOnPiecePixmaps[i][j]);
                }
                else{
                    //关闭状态（值为0）显示反面图片
                    Tool::setPuzzleBoardImage(cellButtons[i][j], cellOffPiecePixmaps[i][j]);
                }
            }
            else{
                //超出拼图范围的格子显示默认正面
                Tool::setPuzzleBoardImage(cellButtons[i][j], cellOnPiecePixmaps[i][j]);
            }
        }
    }
    //更新步数显示
    updateStepDisplay();
    updateRemStepLabel();
    
    //更新连胜统计显示
    if (boardConfig.limitStep >= 0 || boardConfig.limitTime >= 0){
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
    ui->stepLabel->setStyleSheet(Tool::stepTextStyle);
}

//更新连胜统计显示
void BoardView::updateStreakDisplay(){
    QString streakText = 
        QString("连胜    %1\n最高    %2")
        .arg(controller->getCurrentStreak())
        .arg(controller->getMaxStreak(controller->getCurrentDiff()));
    ui->streakLabel->setText(streakText);
    ui->streakLabel->setStyleSheet(Tool::streakTextStyle);
}
//更新剩余步数标签
void BoardView::updateRemStepLabel(){
    ui->remstepLabel->setText(
        QString("  剩余\n  %1步")
        .arg(controller->getRemStep(), 2, 10, '0')
    );
    ui->remstepLabel->setStyleSheet(Tool::remstepTextStyle);
}
//更新时间标签
void BoardView::setTimeLabel(){
    if (!controller->isCountdownEnabled()) return;
    if (mainWindow->getIsAnimation()){
        ui->timeLabel->setText(
            QString("%1:%2")
            .arg(remainingTime / 60, 2, 10, '0')
            .arg(remainingTime % 60, 2, 10, '0')
        );
    }
    else{
        ui->timeLabel->setText(
            QString("%1:%2")
            .arg((remainingTime/2) / 60, 2, 10, '0')
            .arg((remainingTime/2) % 60, 2, 10, '0')
        );
    }

    if (remainingTime <= 10){
        ui->timeLabel->setStyleSheet("color: rgba(255, 127, 0, 0.9); font-family:'Terminal'; font-size: 16px;");
    }
    if (remainingTime <= 0){
        countdownTimer->stop();
        controller->setTimedOut(true);
        checkGameState();
    }
}

//检查游戏状态
void BoardView::checkGameState(){
    //检查是否胜利
    if (controller->isGameFinished()){
        countdownTimer->stop();
        ui->messageLabel->setText(
            QString("游戏胜利！\n共 %1 步完成 (最佳: %2)")
            .arg(controller->getPlayerStep())
            .arg(controller->getInitialMinStep())
        );
        ui->messageLabel->setStyleSheet(Tool::messageTextStyle);
        ui->messageLabel->setStyleSheet("color: rgba(30, 255, 30, 0.7); font-size: 20px;");
        setButtonsEnabled(false, true);
        if (mainWindow->getIsEffectPlaying()) Tool::playAudio(Tool::getAudioPath("effect/finish.mp3"), Tool::effect, false);
        if (mainWindow->getIsAnimation() && remainingTime != 0) startFinishAnimation();
    }
    //检查是否失败
    else if (controller->isGameFailed()){
        countdownTimer->stop();
        if (getCurrentMode() == ModeType::Level && controller->isGameFailed()){
            ui->functionBtn2->setText("重新开始");
        }

        if(controller->isTimedOut()){
            ui->messageLabel->setText("时间到\n游戏失败！");
        }
        else{
            ui->messageLabel->setText("步数超过限制\n游戏失败！");
        }
        ui->messageLabel->setStyleSheet(Tool::messageTextStyle);
        ui->messageLabel->setStyleSheet("color: rgba(255, 127, 0, 0.7); font-size: 20px;");
        setButtonsEnabled(false, true);
    }
}

//设置操作按钮的启用状态（是否启用，是否在视觉上变灰）
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
            rowBtns[i]->blockSignals(!enabled);
            colBtns[i]->blockSignals(!enabled);
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
        ui->mdBtn->blockSignals(!enabled);
        ui->sdBtn->blockSignals(!enabled);
    }

    //启用/禁用功能按钮
    ui->functionBtn1->setEnabled(true);
    ui->functionBtn2->setEnabled(true);
    ui->backBtn->setEnabled(true);
    if (getCurrentMode() == ModeType::Level){
        if (controller->getCurrentLevel() <= 0){
            ui->functionBtn1->setEnabled(false);
        }
        if (controller->getPlayerMaxLevel() >= controller->getTotalLevel()){
            ui->functionBtn2->setEnabled(false);
        }
    }
}

//翻转动画
void BoardView::startTurnAnimation(){
    if (mainWindow->getIsAnimation()){
        isAnimating = true;
        setButtonsEnabled(false, false);//禁用所有操作按钮

        cellsToAnimate.clear();//清空待翻转的格子列表
        currentAnimationIndex = 0;//重置动画索引

        //获取当前拼图状态
        std::vector<std::vector<bool>> currentBoard = controller->getBoard();
        int rows = controller->getBoardConfig().rows;
        int cols = controller->getBoardConfig().cols;
        
        //逐个翻转
        for (int j=0; j<cols; j++){
            for (int i=0; i<rows; i++){
                if (i < rows && j < cols && previousBoard[i][j] != currentBoard[i][j]){//如果当前格子的状态与上一帧不同
                    cellsToAnimate.push_back(std::make_pair(i, j));//将当前格子的坐标添加到待翻转列表
                }
            }
        }
        
        if (!cellsToAnimate.empty()){
            animationTimer->start(200);//启动动画定时器，每***ms更新一次动画
        }
        else{
            updateBoardDisplay();
            checkGameState();
            setButtonsEnabled(true, false);//启用所有操作按钮
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
    
    if (i < static_cast<int>(board.size()) && j < static_cast<int>(board[i].size())){//判断坐标是否在拼图范围内
        if (board[i][j]){
            //使用缓存的cellOn拼图块
            Tool::setPuzzleBoardImage(cellButtons[i][j], cellOnPiecePixmaps[i][j]);
        }
        else{
            //使用缓存的cellOff拼图块
            Tool::setPuzzleBoardImage(cellButtons[i][j], cellOffPiecePixmaps[i][j]);
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
    
    //淡入动画
    seqGroup->addAnimation(Tool::createOpacityAnimation(opacityEffect, 0.0, 0.5, 100, QEasingCurve::InQuad).release());
    
    //宽度缩小动画
    auto shrinkAnim = std::make_unique<QPropertyAnimation>(overlay, "geometry");//创建QPropertyAnimation对象，用于动画覆盖层的几何属性
    QRect startRect = overlay->geometry();//获取当前覆盖层的矩形区域
    QRect endRect = QRect(//创建矩形（中心与当前覆盖层矩形相同，顶部与当前覆盖层矩形顶部相同，宽度为0，高度与当前覆盖层矩形相同）
        startRect.center().x(), startRect.top(),
        0, startRect.height()
    );
    shrinkAnim->setStartValue(startRect);//设置动画起始值为当前覆盖层矩形
    shrinkAnim->setEndValue(endRect);//设置动画结束值为宽度为0的矩形
    shrinkAnim->setDuration(200);//设置动画持续时间
    shrinkAnim->setEasingCurve(QEasingCurve::InOutQuad);//设置动画曲线为二次函数
    seqGroup->addAnimation(shrinkAnim.release());//添加宽度缩小动画到顺序组

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
    seqGroup->addAnimation(Tool::createOpacityAnimation(opacityEffect, 0.5, 0.0, 100, QEasingCurve::OutQuad).release());
    
    //动画结束后删除覆盖层
    connect(seqGroup.get(), &QSequentialAnimationGroup::finished, overlay, &QWidget::deleteLater);
    
    //开始动画
    seqGroup->start(QAbstractAnimation::DeleteWhenStopped);
    seqGroup.release();
}
//下一个cell动画函数
void BoardView::animateNextCell(){
    if (currentAnimationIndex <static_cast<int>(cellsToAnimate.size())){
        //获取当前待翻转的cell的坐标
        int i = cellsToAnimate[currentAnimationIndex].first;
        int j = cellsToAnimate[currentAnimationIndex].second;
        updateSingleCell(i, j);//更新cell显示
        addAnimation(cellButtons[i][j]);//添加动画效果
        
        currentAnimationIndex++;//更新当前动画索引
    }
    else{
        animationTimer->stop();//停止动画定时器
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
    
    //执行边框动画
    QSequentialAnimationGroup* borderGroup = borderAnimation();
    if (borderGroup) {
        parallelGroup->addAnimation(borderGroup);
    }
    //执行全屏动画
    QSequentialAnimationGroup* shineGroup = shineAnimation();
    if (shineGroup) {
        parallelGroup->addAnimation(shineGroup);
    }
    
    //动画结束后清理
    connect(parallelGroup.get(), &QParallelAnimationGroup::finished, parallelGroup.get(), &QObject::deleteLater);
    
    parallelGroup->start(QAbstractAnimation::DeleteWhenStopped);//开始并行动画组
    parallelGroup.release();//释放并行动画组
}
//边框动画
QSequentialAnimationGroup* BoardView::borderAnimation(){
    //计算拼图的边界
    QRect boardRect;
    auto calculateButtonBounds = [&boardRect, this](QPushButton* btn){
        if (btn){
            QPoint topLeft = btn->mapTo(this, QPoint(0,0));//将按钮的左上角映射到父窗口坐标系
            QRect btnRect(topLeft, btn->size());//创建按钮的矩形区域
            if (boardRect.isEmpty()){
                boardRect = btnRect;//如果拼图矩形为空，直接赋值为按钮矩形
            }
            else{
                boardRect = boardRect.united(btnRect);//否则，合并拼图矩形和按钮矩形
            }
        }
    };
    for (const auto& row : cellButtons){//遍历每一行的按钮
        for (QPushButton* btn : row){
            calculateButtonBounds(btn);//计算按钮的边界
        }
    }
    if (boardRect.isEmpty()) return nullptr;

    //创建边框动画
    QWidget* borderBox = new QWidget(this);
    borderBox->setStyleSheet("border: 1px solid #ffffff; background-color: transparent;");//设置边框样式为白色1像素线，背景透明
    borderBox->setAttribute(Qt::WA_TransparentForMouseEvents);//设置边框为透明，鼠标事件传递给子窗口
    
    //添加透明度效果
    auto borderEffect = std::make_unique<QGraphicsOpacityEffect>(borderBox);
    borderEffect->setOpacity(0.0);//设置边框透明度为0，初始时不可见
    borderBox->setGraphicsEffect(borderEffect.get());//将边框透明度效果应用到边框窗口
    QGraphicsOpacityEffect* borderEffectPtr = borderEffect.release();

    //边框动画参数
    QRect startRect = boardRect;
    borderBox->setGeometry(startRect);//设置边框窗口的几何形状为初始矩形
    borderBox->show();//显示边框窗口

    //结束矩形（以拼图中心向外扩展）
    int expandSize = 250;//边框扩展大小
    QPoint center = startRect.center();//获取初始矩形的中心坐标
    QRect endRect = QRect(//计算结束矩形的坐标
        center.x() - (startRect.width() + expandSize) / 2,
        center.y() - (startRect.height() + expandSize) / 2,
        startRect.width() + expandSize,
        startRect.height() + expandSize
    );

    //创建边框动画组
    auto borderGroup = std::make_unique<QSequentialAnimationGroup>();
    borderGroup->addAnimation(Tool::createOpacityAnimation(borderEffectPtr, 0.0, 0.5, 300, QEasingCurve::InQuad).release());
    
    //创建边框宽度动画
    auto borderWidthAnim = std::make_unique<QPropertyAnimation>(borderBox, "borderWidth");//创建边框宽度动画
    borderWidthAnim->setStartValue(10);//设置初始边框宽度***px
    borderWidthAnim->setEndValue(60);//设置结束边框宽度***px
    borderWidthAnim->setDuration(700);//设置动画持续时间***ms
    borderWidthAnim->setEasingCurve(QEasingCurve::OutQuad);//设置动画曲线为二次函数
    
    //连接边框宽度变化信号
    connect(borderWidthAnim.get(), &QPropertyAnimation::valueChanged, [borderBox](const QVariant& value){
        int width = value.toInt();
        borderBox->setStyleSheet(//设置边框样式为白色width像素线，背景透明
            QString("border: %1px solid #ffffff; background-color: transparent;")
            .arg(width)
        );
    });
    
    //几何动画（缩放）
    auto scaleAnim = Tool::createScaleAnimation(borderBox, startRect, endRect, 1000, QEasingCurve::OutQuad);
    
    //创建并行动画组（同时执行边框宽度和缩放动画）
    auto parallelAnim = std::make_unique<QParallelAnimationGroup>();
    parallelAnim->addAnimation(borderWidthAnim.release());//添加边框宽度动画
    parallelAnim->addAnimation(scaleAnim.release());//添加缩放动画
    borderGroup->addAnimation(parallelAnim.release());//添加并行动画组
    borderGroup->addAnimation(Tool::createOpacityAnimation(borderEffectPtr, 0.5, 0.0, 300, QEasingCurve::OutQuad).release());

    //动画结束后清理
    connect(borderGroup.get(), &QSequentialAnimationGroup::finished, borderBox, &QWidget::deleteLater);
    
    return borderGroup.release();
}
//全屏闪光动画
QSequentialAnimationGroup* BoardView::shineAnimation(){
//创建全屏动画
    QWidget* overlay = Tool::createOverlay(this, rect(), "background-color: #ffffff;");
    QGraphicsOpacityEffect* overlayEffect = static_cast<QGraphicsOpacityEffect*>(overlay->graphicsEffect());//获取全屏窗口的透明度效果
    overlay->show();

    //创建全屏动画组
    auto overlayGroup = std::make_unique<QSequentialAnimationGroup>();
    //添加全屏动画
    overlayGroup->addAnimation(Tool::createOpacityAnimation(overlayEffect, 0.0, 0.9, 200, QEasingCurve::InQuad).release());
    overlayGroup->addAnimation(Tool::createOpacityAnimation(overlayEffect, 0.9, 0.0, 200, QEasingCurve::OutCubic).release());

    //动画结束后清理
    connect(overlayGroup.get(), &QSequentialAnimationGroup::finished, overlay, &QWidget::deleteLater);
    
    return overlayGroup.release();
}
