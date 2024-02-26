#include "playerList.h"

#include <iostream>
#include <algorithm>
#include <map>
#include <cstdlib>
#include <ctime>
#include <queue>

using namespace std;

map<char, int> winds = {{'n', 0}, {'e', 1}, {'s', 2}, {'w', 3}};
char winds2[4] = {'n', 'e', 's', 'w'};

PlayerList::PlayerList() {
    playerId = 0;
    windSeparatedPlayers.reserve(4);
}

void PlayerList::addPlayer(Player p) {
    p.setId(playerId + 1);
    playerId++;
    players.push_back(p);
}

bool PlayerList::removePlayer(Player p) {
    auto it = find(players.begin(), players.end(), p);
    if(it != players.end()) {
        players.erase(it);
        return true;
    }
    return false;
}

vector<vector<vector<Player>>> PlayerList::assignWinds(int rounds) {
    vector<vector<vector<Player>>> assignedPlayers;
    for(int i = 0; i < rounds; i++) {
        assignedPlayers.push_back(vector<vector<Player>>());
        for(int j = 0; j < 4; j++) {
            assignedPlayers[i].push_back(vector<Player>());
        }
    }
    srand(time(NULL));
    for(int i = 0; i < rounds; i++) {
        if(i % 4 == 0) {
            for(int j = 0; j < players.size(); j++) {
                players[j].resetWinds();
            }
        }
        random_shuffle(players.begin(), players.end());
        vector<Player> playersBackup = players;
        queue<Player> unused;
        for(int j = 0; j < players.size(); j++) {
            Player *currentPlayer = &players[j];
            if(windSeparatedPlayers[0].size() < players.size() / 4 && currentPlayer->canBeWind('n')) {
                currentPlayer->removeWind('n');
                windSeparatedPlayers[0].push_back((*currentPlayer));
            } else if(windSeparatedPlayers[1].size() < players.size() / 4 && currentPlayer->canBeWind('e')) {
                currentPlayer->removeWind('e');
                windSeparatedPlayers[1].push_back((*currentPlayer));
            } else if(windSeparatedPlayers[2].size() < players.size() / 4 && currentPlayer->canBeWind('s')) {
                currentPlayer->removeWind('s');
                windSeparatedPlayers[2].push_back((*currentPlayer));
            } else if(windSeparatedPlayers[3].size() < players.size() / 4 && currentPlayer->canBeWind('w')) {
                currentPlayer->removeWind('w');
                windSeparatedPlayers[3].push_back((*currentPlayer));
            } else {
                unused.push((*currentPlayer));
            }
        }

        int missing[4] = {players.size() / 4 - windSeparatedPlayers[0].size(), players.size() / 4 - windSeparatedPlayers[1].size(), players.size() / 4 - windSeparatedPlayers[2].size(), players.size() / 4 - windSeparatedPlayers[3].size()};
        while(unused.size() > 0) {
            auto p = find(players.begin(), players.end(), unused.front());
            unused.pop();
            vector<char> windsForP = p->getWinds();
            bool found = false;
            while(!found) {
                char randomWind = windsForP[rand() % windsForP.size()];
                if(missing[winds[randomWind]] > 0) {
                    continue;
                }
                for(int j = 0; j < windSeparatedPlayers[winds[randomWind]].size(); j++) { // write detection of missing winds and filling it
                    for(int k = 0; k < 4; k++) {
                        if(missing[k] > 0 && windSeparatedPlayers[winds[randomWind]][j].canBeWind(winds2[k])) {
                            missing[k]--;
                            auto pCopy = find(players.begin(), players.end(), windSeparatedPlayers[winds[randomWind]][j]);
                            pCopy->addBackWind(randomWind);
                            pCopy->removeWind(winds2[k]);

                            p->removeWind(randomWind);
                            windSeparatedPlayers[winds[randomWind]][j] = *p;
                            windSeparatedPlayers[k].push_back(*pCopy);
                            found = true;
                            break;
                        }
                    }
                    if(found) {
                        break;
                    }
                }
            }
        }

        if(windSeparatedPlayers[1].size() < int(players.size() / 4)) { 
            i--;
            for(int j = 0; j < 4; j++) {
                windSeparatedPlayers[j].clear();
            }
            continue;
        }

        for(int j = 0; j < 4; j++) {
            for(int k = 0; k < windSeparatedPlayers[j].size(); k++) {
                assignedPlayers[i][j].push_back(windSeparatedPlayers[j][k]);
            }
        }

        for(int j = 0; j < 4; j++) {
            windSeparatedPlayers[j].clear();
        }
    }
    return assignedPlayers;
}

bool PlayerList::isPlayerInWind(Player p, char wind) {
    return (find(windSeparatedPlayers[winds[wind]].begin(), windSeparatedPlayers[winds[wind]].end(), p) != windSeparatedPlayers[winds[wind]].end());
}

vector<Player>* PlayerList::getPlayers() {
    return &players;
}

void PlayerList::assignTables(vector<vector<vector<Player>>> windedPlayers) {
    vector<vector<vector<Player>>> tables;
    auto playersMasterBackup = players;
    srand(time(NULL));
    int masterRetries;
    do {
        masterRetries = 0;
        tables.clear();
        players = playersMasterBackup;
        for(int i = 0; i < windedPlayers.size(); i++) {
            masterRetries++;
            if(masterRetries > 100) {
                break;
            }
            bool fail = false;
            vector<Player> playersBackup = players;
            auto tablesBackup = tables;
            tables.push_back(vector<vector<Player>>());
            for(int j = 0; j < windedPlayers[i][0].size(); j++) {
                tables[i].push_back(vector<Player>());
                tables[i][j].push_back(windedPlayers[i][0][j]);
            }
            vector<Player> avaliableE = windedPlayers[i][1];
            for(int j = 0; j < tables[i].size(); j++) {
                auto currentPlayerN = find(players.begin(), players.end(), tables[i][j][0]);
                bool found = false;
                int retries = 100;
                while(!found && retries) {
                    Player randomPlayer = avaliableE[rand() % avaliableE.size()];
                    auto currentPlayerE = find(players.begin(), players.end(), randomPlayer);
                    if(!currentPlayerN->isPlayerIdBanned(randomPlayer.getId())) {
                        tables[i][j].push_back(randomPlayer);
                        currentPlayerE->playWith(currentPlayerN->getId());
                        currentPlayerN->playWith(currentPlayerE->getId());
                        avaliableE.erase(find(avaliableE.begin(), avaliableE.end(), randomPlayer));
                        found = true;
                    }
                    retries--;
                }
                if(retries == 0) {
                    fail = true;
                    break;
                }
            }
            vector<Player> avaliableS = windedPlayers[i][2];
            for(int j = 0; j < tables[i].size(); j++) {
                auto currentPlayerN = find(players.begin(), players.end(), tables[i][j][0]);
                auto currentPlayerE = find(players.begin(), players.end(), tables[i][j][1]);
                bool found = false;
                int retries = 100;
                while(!found && retries) {
                    Player randomPlayer = avaliableS[rand() % avaliableS.size()];
                    auto currentPlayerS = find(players.begin(), players.end(), randomPlayer);
                    if(!currentPlayerN->isPlayerIdBanned(randomPlayer.getId()) && !currentPlayerE->isPlayerIdBanned(randomPlayer.getId())) {
                        tables[i][j].push_back(randomPlayer);
                        currentPlayerS->playWith(currentPlayerN->getId());
                        currentPlayerS->playWith(currentPlayerE->getId());
                        currentPlayerN->playWith(currentPlayerS->getId());
                        currentPlayerE->playWith(currentPlayerS->getId());
                        avaliableS.erase(find(avaliableS.begin(), avaliableS.end(), randomPlayer));
                        found = true;
                    }
                    retries--;
                }
                if(retries == 0) {
                    fail = true;
                    break;
                }
            }
            vector<Player> avaliableW = windedPlayers[i][3];
            for(int j = 0; j < tables[i].size(); j++) {
                auto currentPlayerN = find(players.begin(), players.end(), tables[i][j][0]);
                auto currentPlayerE = find(players.begin(), players.end(), tables[i][j][1]);
                auto currentPlayerS = find(players.begin(), players.end(), tables[i][j][2]);
                bool found = false;
                int retries = 100;
                while(!found && retries) {
                    Player randomPlayer = avaliableW[rand() % avaliableW.size()];
                    auto currentPlayerW = find(players.begin(), players.end(), randomPlayer);
                    if(!currentPlayerN->isPlayerIdBanned(randomPlayer.getId()) && !currentPlayerE->isPlayerIdBanned(randomPlayer.getId()) && !currentPlayerS->isPlayerIdBanned(randomPlayer.getId())) {
                        tables[i][j].push_back(randomPlayer);
                        currentPlayerW->playWith(currentPlayerN->getId());
                        currentPlayerW->playWith(currentPlayerE->getId());
                        currentPlayerW->playWith(currentPlayerS->getId());
                        currentPlayerN->playWith(currentPlayerW->getId());
                        currentPlayerE->playWith(currentPlayerW->getId());
                        currentPlayerS->playWith(currentPlayerW->getId());
                        avaliableW.erase(find(avaliableW.begin(), avaliableW.end(), randomPlayer));
                        found = true;
                    }
                    retries--;
                }
                if(retries == 0) {
                    fail = true;
                    break;
                }
            }
            if(fail) {
                players = playersBackup;
                tables = tablesBackup;
                i--;
                continue;
            }
        }
    } while(masterRetries > 100);

    for(int i = 0; i < tables.size(); i++) {
        cout << "Round " << i + 1 << ":" << endl;
        for(int j = 0; j < tables[i].size(); j++) {
            cout << "---Table" << j + 1 << "---" << endl;
            for(int k = 0; k < tables[i][j].size(); k++) {
                cout << winds2[k] << ": " << tables[i][j][k].getName() << endl;
            }
        }
    }
}