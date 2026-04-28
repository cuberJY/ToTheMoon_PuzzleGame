#pragma once

#include "GameMode.h"

class PlayerData{
public:
    PlayerData();
    //设置连胜次数
    void updateStreak(Diff diff);
    void resetStreak();

    int getCurrentStreak() const;
    int getMaxStreakHard() const;
    int getMaxStreakHardcore() const;

    //设置关卡数据
    void setMaxLevel(int level);

    int getMaxLevel() const;

private:
    int currentStreak;
    int maxStreakHard;
    int maxStreakHardcore;

    int maxLevel;
};