#include "PlayerData.h"

PlayerData::PlayerData():
    currentStreak(0),
    maxStreakHard(0),
    maxStreakHardcore(0){
}
    
void PlayerData::updateStreak(ModeManager::diff diff){
    currentStreak++;
    if (diff == ModeManager::diff::hard){
        if (currentStreak > maxStreakHard){
            maxStreakHard = currentStreak;
        }
    }
    else if (diff == ModeManager::diff::hardcore){
        if (currentStreak > maxStreakHardcore){
            maxStreakHardcore = currentStreak;
        }
    }
}

void PlayerData::resetStreak(){
    currentStreak = 0;
}

int PlayerData::getCurrentStreak() const{
    return currentStreak;
}

int PlayerData::getMaxStreakHard() const{
    return maxStreakHard;
}

int PlayerData::getMaxStreakHardcore() const{
    return maxStreakHardcore;
}   
