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
    enum audio{//音频类型枚举
        bgm,
        effect
    };
//图片   
    //获取图片路径
    static QString getImgPath(const QString& relativePath);
    //设置图片
    static void setButtonImage(QPushButton* btn, const QString& imgPath);
    static void setPuzzleBoardImage(QPushButton* btn, const QPixmap& pixmap);
    static void setBackground(QWidget* widget, const QString& imgPath);
//音频
    //获取音频路径
    static QString getAudioPath(const QString& relativePath);
    //播放音频
    static void playAudio(const QString& audioPath, audio type, bool loop = false);
    static void pauseAudio(audio type);
    static void resumeAudio(audio type);
    static void stopAudio(audio type);
    static void setAudioVolume(audio type, int volume);
//动画
    //创建覆盖层    
    static QWidget* createOverlay(QWidget* parent, const QRect& geometry, const QString& styleSheet, const QRegion& mask = QRegion());
    //创建透明度动画
    static std::unique_ptr<QPropertyAnimation> createOpacityAnimation(QGraphicsOpacityEffect* effect, qreal start, qreal end, int duration, QEasingCurve::Type curve);
    //创建缩放动画
    static std::unique_ptr<QPropertyAnimation> createScaleAnimation(QWidget* target, const QRect& start, const QRect& end, int duration, QEasingCurve::Type curve);
    //创建点击动画
    static void clickedAnimation(QPushButton* btn, bool isAnimation, const QString& onStyle);
//文字样式
    //模式选择
    static inline const QString chooseButtonStyle = "background-color: rgba(255, 255, 255, 0.1); color: rgba(255, 255, 255, 0.7); font-family:'Century Schoolbook'; font-size: 32px;";
    static inline const QString chooseClickedButtonStyle = "background-color: rgba(0, 0, 0, 0.1); color: rgba(255, 255, 255, 0.8); font-family:'Century Schoolbook'; font-size: 32px;";
    static inline const QString chooseDisabledButtonStyle = "background-color: rgba(50, 50, 50, 0.3); color: rgba(255, 255, 255, 0.3); font-family:'Century Schoolbook'; font-size: 32px;";
    //设置菜单
    static inline const QString settingsOnButtonStyle = "background-color: rgba(255, 255, 255, 0.5); color: rgba(0, 0, 0, 0.6); font-family:'Terminal'; font-size: 12px;";
    static inline const QString settingsOffButtonStyle = "background-color: rgba(255, 255, 255, 0.1); color: rgba(255, 255, 255, 0.7); font-family:'Terminal'; font-size: 12px;";
    static inline const QString settingsClickedButtonStyle = "background-color: rgba(0, 0, 0, 0.75); color: rgba(255, 255, 255, 0.8); font-family:'Terminal'; font-size: 12px;";
    //说明
    static inline const QString LabelStyle = "color: rgba(255, 255, 255, 0.7); font-family:'Century Schoolbook'; font-size: 8px;";
    //按钮
    static inline const QString menuButtonStyle = "background-color: rgba(255, 255, 255, 0.1); color: rgba(255, 255, 255, 0.7); font-family:'Terminal'; font-size: 12px;";
    static inline const QString menuClickedButtonStyle = "background-color: rgba(0, 0, 0, 0.1); color: rgba(255, 255, 255, 0.8); font-family:'Terminal'; font-size: 12px;";
    //文本
    static inline const QString titleTextStyle = "color: rgba(255, 255, 255, 0.7); font-family:'Terminal'; font-size: 20px;";
    static inline const QString remstepTextStyle = "color: rgba(255, 255, 255, 0.7); font-family:'Terminal'; font-size: 16px;";
    static inline const QString countDownTextStyle = "color: rgba(255, 255, 255, 0.7); font-family:'Terminal'; font-size: 12px;;";
    static inline const QString timeTextStyle = "color: rgba(255, 255, 255, 0.7); font-family:'Terminal'; font-size: 16px;";
    static inline const QString messageTextStyle = "color: rgba(255, 255, 255, 0.7); font-family:'Terminal'; font-size: 16px;";
    static inline const QString stepTextStyle = "color: rgba(255, 255, 255, 0.5); font-family:'Century Schoolbook'; font-size: 28px;";
    static inline const QString streakTextStyle = "color: rgba(255, 255, 255, 0.7); font-family:'Terminal'; font-size: 20px;";

private:
    static std::unique_ptr<QMediaPlayer> bgmPlayer;
    static std::unique_ptr<QMediaPlayer> effectPlayer;
    static std::unique_ptr<QAudioOutput> bgmAudioOutput;
    static std::unique_ptr<QAudioOutput> effectAudioOutput;
};