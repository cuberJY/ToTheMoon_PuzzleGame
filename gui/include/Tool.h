#pragma once
#include <QWidget>
#include <QString>
#include <QPushButton>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QPropertyAnimation>//用于创建属性动画
#include <QGraphicsOpacityEffect>//用于创建透明度效果
#include <QSequentialAnimationGroup>//用于创建顺序动画组
#include <QEasingCurve>//用于创建缓动曲线
#include <QRect>
#include <memory>

class Tool{
public:
    enum audio{
        bgm,
        effect
    };
    
    //获取图片路径
    static QString getImgPath(const QString& relativePath);
    //设置图片
    static void setButtonImage(QPushButton* btn, const QString& imgPath);
    static void setPuzzleBoardImage(QPushButton* btn, const QPixmap& pixmap);
    static void setBackground(QWidget* widget, const QString& imgPath);

    //获取音频路径
    static QString getAudioPath(const QString& relativePath);
    //播放音频
    static void playAudio(const QString& audioPath, audio type, bool loop = false);
    static void pauseAudio(audio type);
    static void resumeAudio(audio type);
    static void stopAudio(audio type);
    static void setAudioVolume(audio type, int volume);
    
    //创建覆盖层    
    static QWidget* createOverlay(QWidget* parent, const QRect& geometry, const QString& styleSheet, const QRegion& mask = QRegion());
    //创建透明度动画
    static std::unique_ptr<QPropertyAnimation> createOpacityAnimation(QGraphicsOpacityEffect* effect, qreal start, qreal end, int duration, QEasingCurve::Type curve);
    //创建缩放动画
    static std::unique_ptr<QPropertyAnimation> createScaleAnimation(QWidget* target, const QRect& start, const QRect& end, int duration, QEasingCurve::Type curve);
    //创建点击动画
    static void clickedAnimation(QPushButton* btn, bool isAnimation, const QString& onStyle);
    
private:
    static std::unique_ptr<QMediaPlayer> bgmPlayer;
    static std::unique_ptr<QMediaPlayer> effectPlayer;
    static std::unique_ptr<QAudioOutput> bgmAudioOutput;
    static std::unique_ptr<QAudioOutput> effectAudioOutput;
};