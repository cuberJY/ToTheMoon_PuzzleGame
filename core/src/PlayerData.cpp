#include "PlayerData.h"

PlayerData::PlayerData():
    currentStreak(0),
    maxStreakHard(0),
    maxStreakHardcore(0),
    maxLevel(0){
}

void PlayerData::updateStreak(Diff diff){
    currentStreak++;
    if (diff == Diff::hard){
        if (currentStreak > maxStreakHard){
            maxStreakHard = currentStreak;
        }
    }
    else if (diff == Diff::hardcore){
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

void PlayerData::setMaxLevel(int level){
    if (level > maxLevel){
        maxLevel = level;
    }
}

int PlayerData::getMaxLevel() const{
    return maxLevel;
}