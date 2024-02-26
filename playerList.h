#ifndef PLAYER_LIST_H
#define PLAYER_LIST_H

#include <vector>

#include "player.h"

using namespace std;

class PlayerList
{
    private:
        int playerId;
        vector<Player> players;
        vector<vector<Player>> windSeparatedPlayers;
    public:
        PlayerList();
        void addPlayer(Player p);
        bool removePlayer(Player p);
        vector<vector<vector<Player>>> assignWinds(int rounds);
        bool isPlayerInWind(Player p, char wind);
        vector<Player>* getPlayers();
        void assignTables(vector<vector<vector<Player>>> windedPlayers);
};

#endif