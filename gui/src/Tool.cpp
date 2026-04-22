#include "Tool.h"
#include <QWidget>
#include <QString>
#include <QPixmap>
#include <QPalette>
#include <QPushButton>
#include <QTimer>
#include <QMediaPlayer>
#include <QAudioOutput>

QString Tool::getImgPath(const QString& path){
    return QString(":/images/%1").arg(path);
}

void Tool::setButtonImage(QPushButton* btn, const QString& imgPath){
    QPixmap pixmap(imgPath);
    if (!pixmap.isNull()){
        btn->setIcon(pixmap);
        btn->setIconSize(btn->size());
        btn->setText("");
    }
}

void Tool::setPuzzleBoardImage(QPushButton* btn, const QString& imgPath, int row, int col, int width, int height){
    QPixmap fullPixmap(imgPath);
    if (!fullPixmap.isNull()){
        //拼图块的宽度和高度
        int pieceWidth = fullPixmap.width()/width;
        int pieceHeight = fullPixmap.height()/height;
        //拼图块的坐标
        int x = col * pieceWidth;
        int y = (5-row) * pieceHeight;

        QPixmap partPixmap = fullPixmap.copy(x, y, pieceWidth, pieceHeight);
        btn->setIcon(partPixmap);
        btn->setIconSize(btn->size());
        btn->setText("");
    }
}

void Tool::setBackground(QWidget* widget, const QString& imgPath){
    QPixmap pixmap(imgPath);//QPixmap是Qt框架中用于处理和显示图像的类
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

//添加静态成员变量定义
QMediaPlayer* Tool::bgmPlayer = nullptr;
QMediaPlayer* Tool::effectPlayer = nullptr;
QAudioOutput* Tool::bgmAudioOutput = nullptr;
QAudioOutput* Tool::effectAudioOutput = nullptr;

QString Tool::getAudioPath(const QString& path){
    return QString("qrc:/audio/%1").arg(path);
}

void Tool::playAudio(const QString& audioPath, audio type, bool loop){
    QMediaPlayer* MediaPlayer = nullptr;
    
    if (type == bgm){
        if (!bgmPlayer){
            bgmPlayer = new QMediaPlayer();
            bgmAudioOutput = new QAudioOutput();
            bgmAudioOutput->setVolume(0.5);
            bgmPlayer->setAudioOutput(bgmAudioOutput);
        }
        MediaPlayer = bgmPlayer;
    }
    else if(type == effect){
        if (!effectPlayer){
            effectPlayer = new QMediaPlayer();
            effectAudioOutput = new QAudioOutput();
            effectAudioOutput->setVolume(1);
            effectPlayer->setAudioOutput(effectAudioOutput);
        }
        MediaPlayer = effectPlayer;
    }
    
    if (MediaPlayer){
        QUrl url(audioPath);
        MediaPlayer->setSource(url);
        if (loop){
            MediaPlayer->setLoops(QMediaPlayer::Infinite);
        }
        else{
            MediaPlayer->setLoops(1);
        }
        MediaPlayer->play();
    }
}

void Tool::pauseAudio(audio type){
    if (type == bgm && bgmPlayer){
        bgmPlayer->pause();
    }
    else if (type == effect && effectPlayer){
        effectPlayer->pause();
    }
}

void Tool::resumeAudio(audio type){
    if (type == bgm && bgmPlayer){
        bgmPlayer->play();
    }
    else if (type == effect && effectPlayer){
        effectPlayer->play();
    }
}

void Tool::stopAudio(audio type){
    if (type == bgm && bgmPlayer){
        bgmPlayer->stop();
        delete bgmPlayer;
        delete bgmAudioOutput;
        bgmPlayer = nullptr;
        bgmAudioOutput = nullptr;
    }
    else if (type == effect && effectPlayer){
        effectPlayer->stop();
        delete effectPlayer;
        delete effectAudioOutput;
        effectPlayer = nullptr;
        effectAudioOutput = nullptr;
    }
}

void Tool::setAudioVolume(audio type, int volume){
    // 将 0-100 的音量转换为 0.0-1.0
    qreal normalizedVolume = volume/100.0;
    if (normalizedVolume < 0.0) normalizedVolume = 0.0;
    if (normalizedVolume > 1.0) normalizedVolume = 1.0;
    
    if (type == bgm && bgmAudioOutput){
        bgmAudioOutput->setVolume(normalizedVolume);
    }
    else if (type == effect && effectAudioOutput){
        effectAudioOutput->setVolume(normalizedVolume);
    }
}
//创建覆盖层
QWidget* Tool::createOverlay(QWidget* parent, const QRect& geometry, const QString& styleSheet, const QRegion& mask){
    QWidget* overlay = new QWidget(parent);
    overlay->setGeometry(geometry);
    overlay->setStyleSheet(styleSheet);
    overlay->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    overlay->setAttribute(Qt::WA_TransparentForMouseEvents);
    if (!mask.isEmpty()){
        overlay->setMask(mask);
    }
    
    QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(overlay);
    effect->setOpacity(0.0);
    overlay->setGraphicsEffect(effect);
    
    return overlay;
}
//创建透明度动画
std::unique_ptr<QPropertyAnimation> Tool::createOpacityAnimation(QGraphicsOpacityEffect* effect, qreal start, qreal end, int duration, QEasingCurve::Type curve){
    auto anim = std::make_unique<QPropertyAnimation>(effect, "opacity");//"opacity"是透明度属性的名称，用于创建透明度动画
    anim->setStartValue(start);
    anim->setEndValue(end);
    anim->setDuration(duration);
    anim->setEasingCurve(curve);
    return anim;
}
std::unique_ptr<QPropertyAnimation> Tool::createScaleAnimation(QWidget* target, const QRect& start, const QRect& end, int duration, QEasingCurve::Type curve){
    auto anim = std::make_unique<QPropertyAnimation>(target, "geometry");//"geometry"是几何属性的名称，用于创建缩放动画
    anim->setStartValue(start);
    anim->setEndValue(end);
    anim->setDuration(duration);
    anim->setEasingCurve(curve);
    return anim;
}

void Tool::clickedAnimation(QPushButton* btn, bool isAnimation, const QString& onStyle){
    if (isAnimation){
        QString currentStyle = btn->styleSheet();
        // 应用悬停样式
        btn->setStyleSheet(onStyle);
        // 创建定时器，一段时间后恢复原样式
        QTimer::singleShot(200, [=](){
            btn->setStyleSheet(currentStyle);
        });
    }
}