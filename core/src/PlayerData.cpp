#include "PlayerData.h"
#include <fstream>//文件读写
#include <sstream>//字符串流

namespace {
    int readJsonInt(const std::string& text, const std::string& key) {
        std::string search = "\"" + key + "\"";
        size_t pos = text.find(search);
        if (pos == std::string::npos) return 0;
        pos = text.find(':', pos + search.size());
        if (pos == std::string::npos) return 0;
        return std::stoi(text.substr(pos + 1));
    }
}

PlayerData::PlayerData():
    currentStreak(0),
    maxStreakEasy(0),
    maxStreakNormal(0),
    maxStreakHard(0),
    maxStreakHardcore(0),
    maxLevel(0)
{
    loadFromFile("playerData.json");
}

PlayerData::~PlayerData(){
    saveToFile("playerData.json");
}

void PlayerData::resetData(){
    currentStreak = 0;
    maxStreakEasy = 0;
    maxStreakNormal = 0;
    maxStreakHard = 0;
    maxStreakHardcore = 0;
    maxLevel = 0;
}

void PlayerData::updateStreak(Diff diff){
    currentStreak++;
    if (diff == Diff::easy){
        if (currentStreak > maxStreakEasy){
            maxStreakEasy = currentStreak;
        }
    }
    else if (diff == Diff::normal){
        if (currentStreak > maxStreakNormal){
            maxStreakNormal = currentStreak;
        }
    }
    else if (diff == Diff::hard){
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

int PlayerData::getMaxStreakEasy() const{
    return maxStreakEasy;
}

int PlayerData::getMaxStreakNormal() const{
    return maxStreakNormal;
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

void PlayerData::saveToFile(const std::string& filepath) const{
    std::ofstream out(filepath);
    if (!out.is_open()) return;
    out << "{\n";
    out << "    \"currentStreak\": " << currentStreak << ",\n";
    out << "    \"maxStreakEasy\": " << maxStreakEasy << ",\n";
    out << "    \"maxStreakNormal\": " << maxStreakNormal << ",\n";
    out << "    \"maxStreakHard\": " << maxStreakHard << ",\n";
    out << "    \"maxStreakHardcore\": " << maxStreakHardcore << ",\n";
    out << "    \"maxLevel\": " << maxLevel << "\n";
    out << "}\n";
}

void PlayerData::loadFromFile(const std::string& filepath){
    std::ifstream in(filepath);
    if (!in.is_open()) return;
    std::stringstream ss;
    ss << in.rdbuf();
    std::string text = ss.str();
    currentStreak = readJsonInt(text, "currentStreak");
    maxStreakEasy = readJsonInt(text, "maxStreakEasy");
    maxStreakNormal = readJsonInt(text, "maxStreakNormal");
    maxStreakHard = readJsonInt(text, "maxStreakHard");
    maxStreakHardcore = readJsonInt(text, "maxStreakHardcore");
    maxLevel = readJsonInt(text, "maxLevel");
}