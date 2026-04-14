#include "board.h"
#include "ui_board.h"
#include "ModeManager.h"

#include <vector>
#include <QCoreApplication>
#include <QPixmap>
#include <QDir>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QEasingCurve>
#include <QWidget>
#include <QColor>
#include <QTimer>

QString getImgPath(const QString& filename) {
    //获取应用程序可执行文件所在目录
    QString appDir = QCoreApplication::applicationDirPath();
    QString resourcesPath = appDir + "/../.."  + "/resources/";

    return resourcesPath + filename;
}

//设置按钮图片
void setBoardImage(QPushButton* btn, const QString& imgPath){
    QPixmap pixmap(imgPath);//QPixmap是Qt框架中用于处理和显示图像的类
    if (!pixmap.isNull()){//确认图片加载成功
        btn->setIcon(pixmap);
        btn->setIconSize(btn->size());
        btn->setText("");
    }
}

//设置背景图片
void setWidgetBackground(QWidget* widget, const QString& imgPath){
    QPixmap pixmap(imgPath);
    if (!pixmap.isNull()){
        // 创建一个与窗口大小匹配的新 pixmap
        QPixmap scaledPixmap = pixmap.scaled(widget->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        
        QPalette palette;
        // 使用 scaledPixmap 作为背景，并设置为 NoRepeat
        palette.setBrush(QPalette::Window, QBrush(scaledPixmap));
        widget->setPalette(palette);
        widget->setAutoFillBackground(true);
    }
}

//裁剪并设置拼图按钮图片
void setPuzzleBoardImage(QPushButton* btn, const QString& imgPath, int row, int col, int width, int height){
    QPixmap fullPixmap(imgPath);
    if (!fullPixmap.isNull()){
        int pieceWidth = fullPixmap.width()/width;
        int pieceHeight = fullPixmap.height()/height;
        int x = col * pieceWidth;
        int y = (5-row) * pieceHeight;
        QPixmap partPixmap = fullPixmap.copy(x, y, pieceWidth, pieceHeight);
        btn->setIcon(partPixmap);
        btn->setIconSize(btn->size());
        btn->setText("");
    }
}

//构造函数，初始化棋盘界面
Board::Board(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Board)
    , puzzleBoard(nullptr)
    , manager(nullptr)
    , animationTimer(nullptr)
    , currentAnimationIndex(0)
    , isAnimating(false)
{
    ui->setupUi(this);

    //设置背景图片
    QString bgPath = getImgPath("backgroundBoard.png");
    setWidgetBackground(this, bgPath);

    //初始化棋盘格子按钮数组
    cellButtons = {
        {ui->cell00, ui->cell01, ui->cell02, ui->cell03, ui->cell04, ui->cell05},
        {ui->cell10, ui->cell11, ui->cell12, ui->cell13, ui->cell14, ui->cell15},
        {ui->cell20, ui->cell21, ui->cell22, ui->cell23, ui->cell24, ui->cell25},
        {ui->cell30, ui->cell31, ui->cell32, ui->cell33, ui->cell34, ui->cell35},
        {ui->cell40, ui->cell41, ui->cell42, ui->cell43, ui->cell44, ui->cell45},
        {ui->cell50, ui->cell51, ui->cell52, ui->cell53, ui->cell54, ui->cell55}
    };

    //初始化ModeManager
    manager = new ModeManager();
    manager->setMode(0);

    //初始化动画定时器
    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &Board::animateNextCell);

    //使用lambda表达式连接行按钮信号到通用槽函数
    QPushButton* rowBtns[] = {ui->rowBtn0, ui->rowBtn1, ui->rowBtn2, ui->rowBtn3, ui->rowBtn4, ui->rowBtn5};
    for (int i=0; i<6; i++){
        connect(rowBtns[i], &QPushButton::clicked, this, [this, i]() {onRowButtonClicked(i);});
    }

    //使用lambda表达式连接列按钮信号到通用槽函数
    QPushButton* colBtns[] = {ui->colBtn0, ui->colBtn1, ui->colBtn2, ui->colBtn3, ui->colBtn4, ui->colBtn5};
    for (int i=0; i<6; i++){
        connect(colBtns[i], &QPushButton::clicked, this, [this, i]() {onColButtonClicked(i);});
    }

    //拼图图案路径
    QString cellOnPath = getImgPath("cellOn.png");
    QString cellOffPath = getImgPath("cellOff.png");

    //控制翻转按钮设置图片
    for (int i=0; i<6; i++){
        QString rowBtnPath = getImgPath(QString("rowBtn%1.png").arg(i));
        setBoardImage(rowBtns[i], rowBtnPath);
    }
    for (int i=0; i<6; i++){
        QString colBtnPath = getImgPath(QString("colBtn%1.png").arg(i));
        setBoardImage(colBtns[i], colBtnPath);
    }
    setBoardImage(ui->mdBtn, getImgPath("mdBtn.png"));
    setBoardImage(ui->sdBtn, getImgPath("sdBtn.png"));

    //功能按钮设置图片
    //setBoardImage(ui->backBtn, getImgPath("backBtn.png"));
    //setBoardImage(ui->answerBtn, getImgPath("answerBtn.png"));
    //setBoardImage(ui->restartBtn, getImgPath("restartBtn.png"));
    ui->backBtn->setStyleSheet("background-color: rgba(255, 255, 255, 0.05); color: rgba(255, 255, 255, 0.7); font-family:'Terminal'; font-size: 12px;");
    ui->answerBtn->setStyleSheet("background-color: rgba(255, 255, 255, 0.05); color: rgba(255, 255, 255, 0.7); font-family:'Terminal'; font-size: 12px;");
    ui->restartBtn->setStyleSheet("background-color: rgba(255, 255, 255, 0.05); color: rgba(255, 255, 255, 0.7); font-family:'Terminal'; font-size: 12px;");

    //加载拼图正面大图
    QPixmap fullOnPixmap(cellOnPath);
    QPixmap fullOffPixmap(cellOffPath);
    
    if (!fullOnPixmap.isNull() && !fullOffPixmap.isNull()){
        int pieceWidth = fullOnPixmap.width()/6;//每个格子的宽度（假设大图宽度是6的倍数）
        int pieceHeight = fullOnPixmap.height()/6;//每个格子的高度（假设大图高度是6的倍数）

        // 为每个格子设置正面和反面的大图
        for (int i=0; i<6; i++){
            for (int j=0; j<6; j++){
                int x = j * pieceWidth;//裁剪区域的x坐标
                int y = i * pieceHeight;//裁剪区域的y坐标
                
                // 正面：从正面大图中裁剪
                QPixmap frontPart = fullOnPixmap.copy(x, y, pieceWidth, pieceHeight);
                // 反面：从反面大图中裁剪
                QPixmap backPart = fullOffPixmap.copy(x, y, pieceWidth, pieceHeight);
                
                // 保存正面和反面图片到按钮的属性中
                cellButtons[i][j]->setProperty("frontPixmap", QVariant::fromValue(frontPart));
                cellButtons[i][j]->setProperty("backPixmap", QVariant::fromValue(backPart));
                
                // 初始显示正面（状态为1）
                setPuzzleBoardImage(cellButtons[i][j], cellOnPath, i, j, 6, 6);
            }
        }
    }
}

//析构函数，释放资源
Board::~Board()
{
    if (puzzleBoard){
        delete puzzleBoard;
    }
    if (manager){
        delete manager;
    }
    delete ui;
}

//设置游戏难度
void Board::setDifficulty(int diff)
{
    if (manager){
        manager->setRandomDiff(diff);
    }
}

//开始游戏，初始化棋盘
void Board::startGame()
{
    //释放旧的棋盘对象
    if (puzzleBoard){
        delete puzzleBoard;
    }

    //创建新的棋盘
    puzzleBoard = new PuzzleBoard (manager->createBoard());

    //初始化难度标签
    QString diffText;
    switch (manager->getRandomDiff()) {
        case 0: diffText = "简单"; break;
        case 1: diffText = "普通"; break;
        case 2: diffText = "困难"; break;
        case 3: diffText = "极限"; break;
    }
    ui->diffLabel->setText(diffText);
    ui->diffLabel->setStyleSheet("color: rgba(255, 255, 255, 0.7); font-family:'Terminal'; font-size: 20px;");

    //更新步数标签
    if (manager->getRandomDiff() >= 2){
        ui->stepLabel->setText(
            QString("%1\n\n%2")
            .arg(puzzleBoard->getPlayerStep())
            .arg(puzzleBoard->getInitialMinStep())
        );
        ui->stepLabel->setStyleSheet("color: rgba(255, 255, 255, 0.5); font-family:'Century Schoolbook'; font-size: 28px;");
    }

    int rows = 6;
    int cols = 6;

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

    //根据难度显示/隐藏对角线按钮
    ui->mdBtn->setVisible(true);
    ui->sdBtn->setVisible(manager->getRandomDiff() >= 2);

    //显示连胜统计标签
    ui->streakLabel->setVisible(manager->getRandomDiff() >= 2);

    //清空消息标签
    ui->messageLabel->clear();

    //启用所有操作按钮
    setButtonsEnabled(true, true);

    //禁用部分行列
    for (int i=puzzleBoard->getRow(); i<6; i++){
        rowBtns[i]->setEnabled(false);
        for (int j=0; j<6; j++){
            cellButtons[i][j]->setEnabled(false);
        }
    }
    for (int j=puzzleBoard->getCol(); j<6; j++){
        colBtns[j]->setEnabled(false);
        for (int i=0; i<6; i++){
            cellButtons[i][j]->setEnabled(false);
        }
    }

    //更新棋盘显示
    updateBoardDisplay();
}

//通用行按钮点击槽函数
void Board::onRowButtonClicked(int row)
{
    if (!puzzleBoard || isAnimating) return;
    
    previousBoard = puzzleBoard->getBoard();
    puzzleBoard->turnRow(row);
    startAnimation();
}

//通用列按钮点击槽函数
void Board::onColButtonClicked(int col)
{
    if (!puzzleBoard || isAnimating) return;
    
    previousBoard = puzzleBoard->getBoard();
    puzzleBoard->turnCol(col);
    startAnimation();
}

//翻转主对角线
void Board::on_mdBtn_clicked()
{
    if (!puzzleBoard || isAnimating) return;
    
    previousBoard = puzzleBoard->getBoard();
    puzzleBoard->turnMD();
    startAnimation();
}

//翻转副对角线
void Board::on_sdBtn_clicked()
{
    if (!puzzleBoard || isAnimating) return;
    
    previousBoard = puzzleBoard->getBoard();
    puzzleBoard->turnSD();
    startAnimation();
}

//开始执行动画
void Board::startAnimation()
{
    isAnimating = true;
    setButtonsEnabled(false, false);

    cellsToAnimate.clear();
    currentAnimationIndex = 0;
    
    std::vector<std::vector<bool>> currentBoard = puzzleBoard->getBoard();
    int rows = puzzleBoard->getRow();
    int cols = puzzleBoard->getCol();
    
    //逐个翻转
    for (int j=0; j<6; j++){
        for (int i=0; i<6; i++){
            if (i < rows && j < cols && previousBoard[i][j] != currentBoard[i][j]) {
                cellsToAnimate.push_back(std::make_pair(i, j));
            }
        }
    }
    
    if (!cellsToAnimate.empty()){
        animationTimer->start(100);
    }
    else{
        updateBoardDisplay();
        checkGameState();
        setButtonsEnabled(true, false);
        isAnimating = false;
    }
}

//下一个cell动画槽函数
void Board::animateNextCell()
{
    if (currentAnimationIndex < cellsToAnimate.size()){
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

//更新单个cell的显示
void Board::updateSingleCell(int i, int j)
{
    if (!puzzleBoard) return;
    
    std::vector<std::vector<bool>> board = puzzleBoard->getBoard();
    
    QString cellOnPath = getImgPath("cellOn.png");
    QString cellOffPath = getImgPath("cellOff.png");
    
    if (i < board.size() && j < board[i].size()) {
        if (board[i][j]) {
            setPuzzleBoardImage(cellButtons[i][j], cellOnPath, i, j, 6, 6);
        } else {
            setPuzzleBoardImage(cellButtons[i][j], cellOffPath, i, j, 6, 6);
        }
    }
}

//显示答案
void Board::on_answerBtn_clicked()
{
    if (!puzzleBoard) return;
    std::string answer = puzzleBoard->getBestSolveString();
    ui->messageLabel->setText(QString::fromStdString(answer));
    ui->messageLabel->setStyleSheet("color: rgba(255, 255, 255, 0.7); font-family:'Terminal'; font-size: 16px;");
}

//重新开始游戏
void Board::on_restartBtn_clicked()
{
    if (!isAnimating && !puzzleBoard->isFinished()){
        manager->resetStreak();
    }
    startGame();
}

//返回菜单
void Board::on_backBtn_clicked()
{
    emit backToMenu();
}

//更新棋盘显示
void Board::updateBoardDisplay()
{
    if (!puzzleBoard) return;

    std::vector<std::vector<bool>> board = puzzleBoard->getBoard();
    int row = 6;
    int col = 6;

    //加载拼图正面和反面大图路径
    QString cellOnPath = getImgPath("cellOn.png");
    QString cellOffPath = getImgPath("cellOff.png");
    
    //更新每个格子的图片
    for (int i=0; i<row; i++){
        for (int j=0; j<col; j++){
            //确保索引不越界
            if (i < board.size() && j < board[i].size()) {
                if (board[i][j]) {
                    // 激活状态（值为1）：显示正面图片
                    setPuzzleBoardImage(cellButtons[i][j], cellOnPath, i, j, 6, 6);
                } else {
                    // 关闭状态（值为0）：显示反面图片
                    setPuzzleBoardImage(cellButtons[i][j], cellOffPath, i, j, 6, 6);
                }
            } else {
                // 超出棋盘范围的格子显示默认状态（正面）
                setPuzzleBoardImage(cellButtons[i][j], cellOnPath, i, j, 6, 6);
            }
        }
    }

    //更新步数标签
    ui->stepLabel->setText(
        QString("%1\n\n%2")
        .arg(puzzleBoard->getPlayerStep())
        .arg(puzzleBoard->getInitialMinStep())
    );
    ui->stepLabel->setStyleSheet("color: rgba(255, 255, 255, 0.5); font-family:'Century Schoolbook'; font-size: 28px;");
    
    //更新连胜统计显示
    if (manager->getRandomDiff() >= 2){
        updateStreakDisplay();
        ui->streakLabel->setStyleSheet("color: rgba(0, 200, 255, 0.5); font-family:'Terminal'; font-size: 18px;");
    }
}

//检查游戏状态
void Board::checkGameState()
{
    if (!puzzleBoard) return;

    //检查是否胜利
    if (puzzleBoard->isFinished()){
        ui->messageLabel->setText(
            QString("游戏胜利！\n共 %1 步完成 (最佳: %2)")//"%n"表示第n个参数
            .arg(puzzleBoard->getPlayerStep())
            .arg(puzzleBoard->getInitialMinStep())
        );
        ui->messageLabel->setStyleSheet("color: rgba(30, 255, 30, 0.7); font-family:'Terminal'; font-size: 16px;");
        
        //更新连胜统计
        if (manager->getRandomDiff() >= 2){
            manager->updateStreak();
            updateStreakDisplay();
        }
        
        setButtonsEnabled(false, true);
    }
    //检查是否失败
    else if (manager->getRandomDiff() == 2){
        if (puzzleBoard->getPlayerStep() >= puzzleBoard->getInitialMinStep()+2) {
            ui->messageLabel->setText("步数超过限制\n游戏失败！");
            ui->messageLabel->setStyleSheet("color: rgba(255, 50, 0, 0.7); font-family:'Terminal'; font-size: 16px;");
            manager->resetStreak();
            updateStreakDisplay();
            setButtonsEnabled(false, true);
        }
    }
    else if (manager->getRandomDiff() == 3){
        if (puzzleBoard->getPlayerStep() >= puzzleBoard->getInitialMinStep()) {
            ui->messageLabel->setText("步数超过限制\n游戏失败！");
            ui->messageLabel->setStyleSheet("color: rgba(255, 50, 0, 0.7); font-family:'Terminal'; font-size: 16px;");
            manager->resetStreak();
            updateStreakDisplay();
            setButtonsEnabled(false, true);
        }
    }
}

//更新连胜统计显示
void Board::updateStreakDisplay()
{
    if (!manager) return;
    QString streakText = 
        QString("连胜    %1\n最高    %2")
        .arg(manager->getCurrentStreak())
        .arg(manager->getMaxStreak());
    ui->streakLabel->setText(streakText);
    ui->streakLabel->setStyleSheet("color: rgba(0, 200, 255, 0.5); font-family:'Terminal'; font-size: 18px;");
}

//设置操作按钮的启用状态
void Board::setButtonsEnabled(bool enabled, bool setVisual){
    //行按钮
    QPushButton* rowBtns[] = {ui->rowBtn0, ui->rowBtn1, ui->rowBtn2, ui->rowBtn3, ui->rowBtn4, ui->rowBtn5};
    //列按钮
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

//为cell添加动画
void Board::addAnimation(QPushButton* btn){
    if (!btn) return;

    //创建一个黑色透明的覆盖层
    QWidget* overlay = new QWidget(btn);
    overlay->setGeometry(btn->rect());
    overlay->setStyleSheet("background-color: rgba(0, 0, 0, 0.5);");
    overlay->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    overlay->setAttribute(Qt::WA_TransparentForMouseEvents);
    overlay->show();
    
    //创建顺序动画组
    QSequentialAnimationGroup* seqGroup = new QSequentialAnimationGroup(overlay);
    
    //淡入动画
    QPropertyAnimation* fadeInAnim = new QPropertyAnimation(overlay, "windowOpacity");
    fadeInAnim->setStartValue(0.0);
    fadeInAnim->setEndValue(1.0);
    fadeInAnim->setDuration(100);
    fadeInAnim->setEasingCurve(QEasingCurve::InQuad);
    seqGroup->addAnimation(fadeInAnim);
    
    //宽度缩小动画
    QPropertyAnimation* shrinkAnim = new QPropertyAnimation(overlay, "geometry");
    QRect startRect = overlay->geometry();
    QRect endRect = QRect(
        startRect.center().x(), startRect.top(),
        0, startRect.height()
    );
    shrinkAnim->setStartValue(startRect);
    shrinkAnim->setEndValue(endRect);
    shrinkAnim->setDuration(200);
    shrinkAnim->setEasingCurve(QEasingCurve::InOutQuad);
    seqGroup->addAnimation(shrinkAnim);

    //宽度放大动画
    QPropertyAnimation* expandAnim = new QPropertyAnimation(overlay, "geometry");
    QRect expandStartRect = QRect(
        startRect.center().x(), startRect.top(),
        0, startRect.height()
    );
    QRect expandEndRect = startRect;
    expandAnim->setStartValue(expandStartRect);
    expandAnim->setEndValue(expandEndRect);
    expandAnim->setDuration(200);
    expandAnim->setEasingCurve(QEasingCurve::InOutQuad);
    seqGroup->addAnimation(expandAnim);

    //淡出动画
    QPropertyAnimation* fadeOutAnim = new QPropertyAnimation(overlay, "windowOpacity");
    fadeOutAnim->setStartValue(1.0);
    fadeOutAnim->setEndValue(0.0);
    fadeOutAnim->setDuration(100);
    fadeOutAnim->setEasingCurve(QEasingCurve::OutQuad);
    seqGroup->addAnimation(fadeOutAnim);
    
    //动画结束后删除覆盖层
    connect(seqGroup, &QSequentialAnimationGroup::finished, overlay, &QWidget::deleteLater);
    
    //开始动画
    seqGroup->start(QAbstractAnimation::DeleteWhenStopped);
}