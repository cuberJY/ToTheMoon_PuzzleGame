#pragma once

#include "ModeManager.h"

class PlayerData{
public:
    PlayerData();
    
    void updateStreak(ModeManager::diff diff);
    void resetStreak();

    int getCurrentStreak() const;
    int getMaxStreakHard() const;
    int getMaxStreakHardcore() const;

private:
    int currentStreak;
    int maxStreakHard;
    int maxStreakHardcore;
};