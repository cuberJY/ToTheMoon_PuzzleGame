#pragma once

#include "GameMode.h"
#include <string>

class PlayerData{
public:
    PlayerData();
    ~PlayerData();

    void resetData();
    //设置连胜次数
    void updateStreak(Diff diff);
    void resetStreak();

    int getCurrentStreak() const;
    int getMaxStreakEasy() const;
    int getMaxStreakNormal() const;
    int getMaxStreakHard() const;
    int getMaxStreakHardcore() const;

    //设置关卡数据
    void setMaxLevel(int level);

    int getMaxLevel() const;

    void saveToFile(const std::string& filepath) const;
    void loadFromFile(const std::string& filepath);

private:
    int currentStreak;
    int maxStreakEasy;
    int maxStreakNormal;
    int maxStreakHard;
    int maxStreakHardcore;

    int maxLevel;
};