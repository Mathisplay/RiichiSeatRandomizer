#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>

using namespace std;

class Player {
    private:
        int playerId;
        string playerName;
        vector<char> avaliableWinds;
        vector<int> bannedOpponents;
    public:
        bool operator==(Player const& p) const { return this->playerId == p.playerId;}
        Player(string name, vector<char> winds);
        void playWith(int p);
        void setId(int id);
        int getId();
        string getName();
        bool canBeWind(char wind);
        bool removeWind(char wind);
        void resetWinds();
        bool isPlayerIdBanned(int id);
        void addBackWind(char wind);
        vector<char> getWinds();
};

#endif